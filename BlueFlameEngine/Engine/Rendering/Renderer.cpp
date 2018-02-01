#include "Renderer.h"

using namespace ENGINE;

Renderer::Renderer() : shaderManager(nullptr), blur(nullptr), bloom(nullptr), screen(nullptr) {}

Renderer::~Renderer() 
{
	OnDestroy();
}

// Initialize
void Renderer::Initialize(Window* window) {
	// Shader manager
	shaderManager = new ResourceManager<Shader>;	

	// framebuffer shaders
	blur = new Shader("Shaders/blur.vs", "Shaders/blur.fs");
	bloom = new Shader("Shaders/bloom.vs", "Shaders/bloom.fs");
	screen = new Shader("Shaders/screen.vs", "Shaders/screen.fs");

	// shader options
	blur->Use();
	blur->setInt("image", 0);
	bloom->Use();
	bloom->setInt("scene", 0);
	bloom->setInt("bloomBlur", 1);
	screen->Use();
	screen->setInt("screenTexture", 0);	

	SetUpFrameBuffers(window);
	SetUpQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// PreRender sets the projection matrix and clear colour for opengl
void Renderer::PreRender(Window* window, Camera* camera, bool splitscreen) {
	if (splitscreen) {
		projection = glm::perspective(glm::radians(camera->Zoom), ((float)window->GetWidth() / 2) / (float)window->GetHeight(), 0.1f, 100.0f);
	}
	else if (!splitscreen) {
		projection = glm::perspective(glm::radians(camera->Zoom), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 100.0f);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glEnable(GL_DEPTH_TEST);	

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
}

// Render sets up the shaders then renders objects
void Renderer::Render(Camera* camera, std::vector<GameObject*> objectList, std::vector<Light*> dirLightList,
																		   std::vector<Light*> pointLightList,
																		   std::vector<Light*> spotLightList) 
{
	// update the view matrix based on the camera
	glm::mat4 view = camera->GetViewMatrix();

	// loop through the object list if it is not empty
	if (objectList.size() != NULL) {
		// get the size of the object list
		unsigned int objectListSize = objectList.size();
		for (unsigned int i = 0; i < objectListSize; ++i) {
			// Make sure object as a shader
			if (shaderManager->get(objectList.at(i)->GetShader()) != NULL) {
				// Get object shader
				Shader* shader = shaderManager->get(objectList.at(i)->GetShader());
				shader->Use();
				// loop through the light object lists
				// directional lights
				if (dirLightList.size() != NULL) {
					unsigned int dirLightListSize = dirLightList.size();
					for (unsigned int j = 0; j < dirLightListSize; ++j) {
						shader->setVec3("dirLight[" + std::to_string(j) + "].direction", dirLightList[j]->lightComponent->direction);
						shader->setVec3("dirLight[" + std::to_string(j) + "].ambient", dirLightList[j]->lightComponent->ambient);
						shader->setVec3("dirLight[" + std::to_string(j) + "].diffuse", dirLightList[j]->lightComponent->diffuse);
						shader->setVec3("dirLight[" + std::to_string(j) + "].specular", dirLightList[j]->lightComponent->specular);
					}
				}
				// point lights
				if (pointLightList.size() != NULL) {
					unsigned int pointLightListSize = pointLightList.size();
					for (unsigned int j = 0; j < pointLightListSize; ++j) {
						shader->setVec3("pointLight[" + std::to_string(j) + "].position", pointLightList[j]->GetWorldPosition());
						shader->setVec3("pointLight[" + std::to_string(j) + "].ambient", pointLightList[j]->lightComponent->ambient);
						shader->setVec3("pointLight[" + std::to_string(j) + "].diffuse", pointLightList[j]->lightComponent->diffuse);
						shader->setVec3("pointLight[" + std::to_string(j) + "].specular", pointLightList[j]->lightComponent->specular);
						shader->setFloat("pointLight[" + std::to_string(j) + "].constant", pointLightList[j]->lightComponent->constant);
						shader->setFloat("pointLight[" + std::to_string(j) + "].linear", pointLightList[j]->lightComponent->linear);
						shader->setFloat("pointLight[" + std::to_string(j) + "].quadratic", pointLightList[j]->lightComponent->quadratic);
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
				shader->setMat4("model", worldModel);
				shader->setMat4("projection", projection);
				shader->setMat4("view", view);
				shader->setVec3("viewPos", camera->Position);				
				// Render object
				objectList.at(i)->Render(shader);
			}
		}
	}
}

// PostRender swaps the frame buffer
void Renderer::PostRender(Window* window, bool splitscreen) 
{	
	PostProcess();
}

void Renderer::RenderSkybox(Skybox* sceneSkybox, Camera* camera) {
	// make sure skybox is not null
	if (sceneSkybox != NULL) {
		// remove translation from the view matrix
		glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));

		glDepthFunc(GL_LEQUAL);
		
		// get skybox shader
		Shader* shader = shaderManager->get(sceneSkybox->GetShader());
		shader->Use();
		shader->setInt("skybox", 0);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		// render skybox
		sceneSkybox->Render();

		glDepthFunc(GL_LESS);
	}
}

// Shader manager
ResourceManager<Shader>* Renderer::GetShaderManager() {
	return shaderManager;
}

// Clean up
void Renderer::OnDestroy() {
	delete shaderManager;
	shaderManager = nullptr;
	delete blur;
	blur = nullptr;
	delete bloom;
	bloom = nullptr;
	delete screen;
	screen = nullptr;
}

void Renderer::RenderQuad()
{
	// Renders a quad(square) to draw framebuffer onto
	glBindVertexArray(quadVAO);	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Renderer::SetUpFrameBuffers(Window* window) 
{	
	// set up frame buffers

	// framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// 2 color buffers; one for the normal scene and one for blurring
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	
	// create and attach depth buffer
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->GetWidth(), window->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments to use for rendering 
	attachments[0] = { GL_COLOR_ATTACHMENT0 };
	attachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// framebuffer for blurring (using a technique called ping-pong)
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window->GetWidth(), window->GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// check if framebuffers are complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}	
}

void Renderer::UseBloom() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	bool horizontal = true, first_iteration = true;
	blur->Use();
	for (unsigned int i = 0; i < blurAmount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		blur->setInt("horizontal", horizontal);
		// bind texture of other framebuffer (or scene if first iteration)
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  
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
	// not used cause hdr is disabled (i think it makes everything look washed out)
	//bloom->setFloat("exposure", 1.0f);	
	RenderQuad();
}

void Renderer::PostProcess() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// disable depth test so quad is shown
	glDisable(GL_DEPTH_TEST);
	// clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// screen options
	screen->Use();
	screen->setInt("inverted", inverted);
	screen->setInt("greyscale", greyscale);
	screen->setInt("kernel", kernel);

	// bind colour buffer onto quad
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);	
	RenderQuad();

	// Bloom
	if (useBloom && !inverted && !greyscale && !kernel) {
		UseBloom();
	}
}

void Renderer::SetUpQuad() {
	// quad (square) to render on screen
	float quadVertices[] = { 
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

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