#include "Renderer.h"

using namespace ENGINE;

Renderer::Renderer() : projection(0), shaderManager(nullptr)
{

}


Renderer::~Renderer() 
{
	delete shaderManager;
	shaderManager = nullptr;
	OnDestroy();
}

// Initialize
void Renderer::Initialize(Window* window) {
	// Shader manager
	shaderManager = new ResourceManager<Shader>;	

	blur = new Shader("Shaders/blur.vs", "Shaders/blur.fs");
	bloom = new Shader("Shaders/bloom.vs", "Shaders/bloom.fs");
	screen = new Shader("Shaders/screen.vs", "Shaders/screen.fs");

	blur->Use();
	blur->setInt("image", 0);
	bloom->Use();
	bloom->setInt("scene", 0);
	bloom->setInt("bloomBlur", 1);
	screen->Use();
	screen->setInt("screenTexture", 0);

	SetUpFrameBuffers(window);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_BLEND);
	//0glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// PreRender sets the projection matrix and clear colour for opengl
void Renderer::PreRender(Window* window, Camera* camera, bool splitscreen, bool postprocess) {
	if (splitscreen) {
		projection = glm::perspective(glm::radians(camera->Zoom), ((float)window->GetWidth() / 2) / (float)window->GetHeight(), 0.1f, 100.0f);
	}
	else if (!splitscreen) {
		projection = glm::perspective(glm::radians(camera->Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
	}

	glEnable(GL_DEPTH_TEST);


	if (useBloom) {
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	}
	
	if (blackAndWhite) {
		if (!useBloom) {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}

// Render sets up the shaders then renders lights, objects, and the skybox
void Renderer::Render(Camera* camera, std::vector<GameObject*> objectList, std::vector<Light*> dirLightList,
																		   std::vector<Light*> pointLightList,
																		   std::vector<Light*> spotLightList) 
{
	// Update the view matrix based on the camera
	glm::mat4 view = camera->GetViewMatrix();

	// loop through the object list
	if (objectList.size() != NULL) {
		unsigned int objectListSize = objectList.size();
		for (unsigned int i = 0; i < objectListSize; ++i) {
			if (shaderManager->get(objectList.at(i)->GetShader()) != NULL) {
				// loop through the light object list (don't worry too much about how this works cause it's probably all going to change)
				if (dirLightList.size() != NULL) {
					unsigned int dirLightListSize = dirLightList.size();
					for (unsigned int j = 0; j < dirLightListSize; ++j) {
						// directional lights
						std::string light = "dirLight[" + std::to_string(j) + "].";
						Shader* dirLightShader = shaderManager->get(objectList.at(i)->GetShader());
						dirLightShader->Use();
						dirLightShader->setVec3(light + "direction", dirLightList[j]->lightComponent->direction);
						dirLightShader->setVec3(light + "ambient", dirLightList[j]->lightComponent->ambient);
						dirLightShader->setVec3(light + "diffuse", dirLightList[j]->lightComponent->diffuse);
						dirLightShader->setVec3(light + "specular", dirLightList[j]->lightComponent->specular);
					}
				}	
				if (pointLightList.size() != NULL) {
					unsigned int pointLightListSize = pointLightList.size();
					for (unsigned int j = 0; j < pointLightListSize; ++j) {
						// point lights
						std::string light = "pointLight[" + std::to_string(j) + "].";
						Shader* pointLightShader = shaderManager->get(objectList.at(i)->GetShader());
						pointLightShader->Use();
						pointLightShader->setVec3(light + "position", pointLightList[j]->GetWorldPosition());
						pointLightShader->setVec3(light + "ambient", pointLightList[j]->lightComponent->ambient);
						pointLightShader->setVec3(light + "diffuse", pointLightList[j]->lightComponent->diffuse);
						pointLightShader->setVec3(light + "specular", pointLightList[j]->lightComponent->specular);
						pointLightShader->setFloat(light + "constant", pointLightList[j]->lightComponent->constant);
						pointLightShader->setFloat(light + "linear", pointLightList[j]->lightComponent->linear);
						pointLightShader->setFloat(light + "quadratic", pointLightList[j]->lightComponent->quadratic);
					}
				}
				// local positions
				//glm::mat4 localModel = glm::mat4();
				//localModel = glm::translate(localModel, objectList.at(i)->GetLocalPosition());
				//localModel = glm::scale(localModel, objectList.at(i)->GetLocalScale());
				//localModel = glm::rotate(localModel, objectList.at(i)->GetLocalRotationAngle(), objectList.at(i)->GetLocalRotation());
				// world positions
				glm::mat4 worldModel = glm::mat4();
				worldModel = glm::translate(worldModel, objectList.at(i)->GetWorldPosition());
				worldModel = glm::scale(worldModel, objectList.at(i)->GetWorldScale());
				worldModel = glm::rotate(worldModel, objectList.at(i)->GetWorldRotationAngle(), objectList.at(i)->GetWorldRotation());
				// Object shader
				Shader* shader = shaderManager->get(objectList.at(i)->GetShader());
				shader->Use();
				shader->setMat4("model", worldModel);
				shader->setMat4("projection", projection);
				shader->setMat4("view", view);
				shader->setVec3("viewPos", camera->Position);
				// Render object
				objectList.at(i)->Render(shaderManager->get(objectList.at(i)->GetShader()));
			}
		}
	}
}

// PostRender swaps the frame buffer
void Renderer::PostRender(Window* window, bool splitscreen, bool postprocess) {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (useBloom) {
		UseBloom();
	}
	
	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	if (blackAndWhite) {
		RenderBlackAndWhite();
		//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		//UseBloom();
	}	
	
	//glBindVertexArray(quadVAO);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//if (postprocess) {
		
	//}
	//if (postprocess) {
		
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBlitFramebuffer(0, 0, 853, 480, 0, 0, window->GetWidth(), window->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//Swap the buffers to show the back buffer that this frame was rendered on
	//SDL_GL_SwapWindow(window->GetWindow());
}

void Renderer::RenderSkybox(Skybox* sceneSkybox, Camera* camera) {
	Skybox* skybox = sceneSkybox;
	if (skybox != NULL) {
		glDepthFunc(GL_LEQUAL);
		shaderManager->get(skybox->GetShader())->Use();
		shaderManager->get(skybox->GetShader())->setInt("skybox", 0);

		glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix

		shaderManager->get(skybox->GetShader())->setMat4("view", view);
		shaderManager->get(skybox->GetShader())->setMat4("projection", projection);

		skybox->Render();

		glDepthFunc(GL_LESS);
	}
}

// Shader manager
ResourceManager<Shader>* Renderer::GetShaderManager() {
	return shaderManager;
}

// Clean up
void Renderer::OnDestroy() {
	projection = glm::mat4();
}

void Renderer::RenderQuad()
{
	if (quadVAO == 0)
	{

		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


		
	}
	
	glBindVertexArray(quadVAO);	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	quadVAO = 0;
}

void Renderer::SetUpFrameBuffers(Window* window) {
	
	// configure (floating point) framebuffers
	// ---------------------------------------	
	hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	// create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
	colorBuffers[2];
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	
	// create and attach depth buffer (renderbuffer)
	rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->GetWidth(), window->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	attachments[0] = { GL_COLOR_ATTACHMENT0 };
	attachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ping-pong-framebuffer for blurring
	pingpongFBO[2];
	pingpongColorbuffers[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
	
	// framebuffer configuration
	// -------------------------
	framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window->GetWidth(), window->GetHeight()); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void Renderer::UseBloom() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	blur->Use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		blur->setInt("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		RenderQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	bloom->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	bloom->setInt("bloom", true);
	bloom->setFloat("exposure", 1.0f);
	RenderQuad();
}

void Renderer::RenderBlackAndWhite() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	glClear(GL_DEPTH_BUFFER_BIT);
	screen->Use();
	if (quadVAO == 0)
	{
		/*
		float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		*/
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
								 // positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};
		/*
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		*/
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	}

	//glBindVertexArray(quadVAO);	
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glBindVertexArray(0);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	quadVAO = 0;
}