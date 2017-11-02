#include "Renderer.h"

using namespace ENGINE;

Renderer::Renderer() : projection(0), skybox(nullptr), shaderManager(nullptr)
{

}


Renderer::~Renderer() 
{
	delete skybox;
	skybox = nullptr;
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

	glEnable(GL_DEPTH_TEST);

	

	//SetUpFrameBuffers(window);
	
}

// PreRender sets the projection matrix and clear colour for opengl
void Renderer::PreRender(Window* window, Camera* camera, bool splitscreen) {
	if (splitscreen) {
		projection = glm::perspective(glm::radians(camera->Zoom), ((float)window->GetWidth() / 2) / (float)window->GetHeight(), 0.1f, 100.0f);
	}
	else if (!splitscreen) {
		projection = glm::perspective(glm::radians(camera->Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
}

// Render sets up the shaders then renders lights, objects, and the skybox
void Renderer::Render(Camera* camera, std::vector<GameObject*> objectList, std::vector<Light*> dirLightList,
																		   std::vector<Light*> pointLightList,
																		   std::vector<Light*> spotLightList) 
{
	blur->Use();
	blur->setInt("image", 0);
	bloom->Use();
	bloom->setInt("scene", 0);
	bloom->setInt("bloomBlur", 1);

	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the view matrix based on the camera
	glm::mat4 view = camera->GetViewMatrix();
	
	// Skybox
	RenderSkybox(camera);

	// loop through the object list
	if (objectList.size() != NULL) {
		for (size_t i = 0; i < objectList.size(); ++i) {
			if (shaderManager->get(objectList.at(i)->GetShader()) != NULL) {
				// loop through the light object list (don't worry too much about how this works cause it's probably all going to change)
				if (dirLightList.size() != NULL) {
				for (size_t j = 0; j < dirLightList.size(); ++j) {
				// directional lights
				std::string light = "dirLight[" + std::to_string(j) + "].";
				shaderManager->get(objectList.at(i)->GetShader())->Use();
				shaderManager->get(objectList.at(i)->GetShader())->setVec3(light + "direction", dirLightList[j]->lightComponent->direction);
				shaderManager->get(objectList.at(i)->GetShader())->setVec3(light + "ambient", dirLightList[j]->lightComponent->ambient);
				shaderManager->get(objectList.at(i)->GetShader())->setVec3(light + "diffuse", dirLightList[j]->lightComponent->diffuse);
				shaderManager->get(objectList.at(i)->GetShader())->setVec3(light + "specular", dirLightList[j]->lightComponent->specular);
				}
				}	
				if (pointLightList.size() != NULL) {
				for (size_t j = 0; j < pointLightList.size(); ++j) {
				// point lights
				std::string light = "pointLight[" + std::to_string(j) + "].";;
				shaderManager->get(objectList.at(i)->GetShader())->Use();
				shaderManager->get(objectList.at(i)->GetShader())->setVec3(light + "position", pointLightList[j]->GetWorldPosition());
				shaderManager->get(objectList.at(i)->GetShader())->setVec3(light + "ambient", pointLightList[j]->lightComponent->ambient);
				shaderManager->get(objectList.at(i)->GetShader())->setVec3(light + "diffuse", pointLightList[j]->lightComponent->diffuse);
				shaderManager->get(objectList.at(i)->GetShader())->setVec3(light + "specular", pointLightList[j]->lightComponent->specular);
				shaderManager->get(objectList.at(i)->GetShader())->setFloat(light + "constant", pointLightList[j]->lightComponent->constant);
				shaderManager->get(objectList.at(i)->GetShader())->setFloat(light + "linear", pointLightList[j]->lightComponent->linear);
				shaderManager->get(objectList.at(i)->GetShader())->setFloat(light + "quadratic", pointLightList[j]->lightComponent->quadratic);
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
				shaderManager->get(objectList.at(i)->GetShader())->Use();
				shaderManager->get(objectList.at(i)->GetShader())->setMat4("model", worldModel);
				shaderManager->get(objectList.at(i)->GetShader())->setMat4("projection", projection);
				shaderManager->get(objectList.at(i)->GetShader())->setMat4("view", view);
				shaderManager->get(objectList.at(i)->GetShader())->setVec3("viewPos", camera->Position);
				// Render object
				objectList.at(i)->Render(shaderManager->get(objectList.at(i)->GetShader()));
			}
		}
	}
	/*
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	blur->Use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		blur->setInt("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		renderQuad();
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
	renderQuad();
	*/
}

// PostRender swaps the frame buffer
void Renderer::PostRender(Window* window) {
	//Swap the buffers to show the back buffer that this frame was rendered on
	SDL_GL_SwapWindow(window->GetWindow());
}

// Setting and rendering skybox
void Renderer::SetSkybox(Skybox* skybox) {
	this->skybox = skybox;
}

void Renderer::RenderSkybox(Camera* camera) {
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

void Renderer::renderQuad()
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
}