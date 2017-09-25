#include "Renderer.h"

using namespace ENGINE;

Renderer::Renderer()
{
	//Instantitate the Shader class, linking the vertex and fragment shaders together that we linked
	shader = new Shader("Shaders/default.vs", "Shaders/default.fs");
	lightShader = new Shader("Shaders/lamp.vs", "Shaders/lamp.fs");
	modelShader = new Shader("Shaders/model.vs", "Shaders/model.fs");
}


Renderer::~Renderer()
{
	delete shader;
	shader = nullptr;
}

void Renderer::Render(Window* window, Camera* camera, glm::mat4 projection)
{
	projection = glm::perspective(glm::radians(camera->Zoom), 1280.0f / 720.0f, 0.1f, 100.0f);

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera->GetViewMatrix();

	
	modelShader->Use();

	modelShader->setMat4("projection", projection);
	modelShader->setMat4("view", view);
	modelShader->setVec3("viewPos", camera->Position);

	lightShader->Use();

	lightShader->setMat4("projection", projection);
	lightShader->setMat4("view", view);

	//Calling this function signals to OpenGL to switch to using this as the shader
	//for vertex and fragment calculations
	//Everything drawn after this point will use this specific shader
	shader->Use();

	//glm::mat4 worldModel;

	//shader->setMat4("model", worldModel);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	shader->setVec3("viewPos", camera->Position); // camera

	/*
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	*/
	if (lightObjectList.size() != NULL) {
		for (size_t i = 0; i < lightObjectList.size(); ++i) {
			if (lightObjectList[i]->lightComponent->GetLightType() == LightComponent::Light_Type::DIRECTIONAL) {
				// directional light
				shader->Use();
				shader->setVec3("dirLight.direction", lightObjectList[i]->lightComponent->direction);
				shader->setVec3("dirLight.ambient", lightObjectList[i]->lightComponent->ambient);
				shader->setVec3("dirLight.diffuse", lightObjectList[i]->lightComponent->diffuse);
				shader->setVec3("dirLight.specular", lightObjectList[i]->lightComponent->specular);
				modelShader->Use();
				modelShader->setVec3("dirLight.direction", lightObjectList[i]->lightComponent->direction);
				modelShader->setVec3("dirLight.ambient", lightObjectList[i]->lightComponent->ambient);
				modelShader->setVec3("dirLight.diffuse", lightObjectList[i]->lightComponent->diffuse);
				modelShader->setVec3("dirLight.specular", lightObjectList[i]->lightComponent->specular);
			}
			else if (lightObjectList[i]->lightComponent->GetLightType() == LightComponent::Light_Type::POINTLIGHT) {
				// point light 
				shader->setVec3("pointLights[0].position", lightObjectList[i]->GetWorldPosition());
				shader->setVec3("pointLights[0].ambient", lightObjectList[i]->lightComponent->ambient);
				shader->setVec3("pointLights[0].diffuse", lightObjectList[i]->lightComponent->diffuse);
				shader->setVec3("pointLights[0].specular", lightObjectList[i]->lightComponent->specular);
				shader->setFloat("pointLights[0].constant", lightObjectList[i]->lightComponent->constant);
				shader->setFloat("pointLights[0].linear", lightObjectList[i]->lightComponent->linear);
				shader->setFloat("pointLights[0].quadratic", lightObjectList[i]->lightComponent->quadratic);
			}
		}
	}	
	
	/*
	// point light 2
	shader->setVec3("pointLights[1].position", pointLightPositions[1]);
	shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("pointLights[1].constant", 1.0f);
	shader->setFloat("pointLights[1].linear", 0.09);
	shader->setFloat("pointLights[1].quadratic", 0.032);
	// point light 3
	shader->setVec3("pointLights[2].position", pointLightPositions[2]);
	shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("pointLights[2].constant", 1.0f);
	shader->setFloat("pointLights[2].linear", 0.09);
	shader->setFloat("pointLights[2].quadratic", 0.032);
	// point light 4
	shader->setVec3("pointLights[3].position", pointLightPositions[3]);
	shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("pointLights[3].constant", 1.0f);
	shader->setFloat("pointLights[3].linear", 0.09);
	shader->setFloat("pointLights[3].quadratic", 0.032);
	
	// spotLight
	shader->setVec3("spotLight.position", 0.0f, 0.0f, -5.0f);
	shader->setVec3("spotLight.direction", 0.0f, 0.0f, 0.0f);
	shader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("spotLight.constant", 1.0f);
	shader->setFloat("spotLight.linear", 0.09);
	shader->setFloat("spotLight.quadratic", 0.032);
	shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	*/
	
	if (objectList.size() != NULL) {
		for (size_t i = 0; i < objectList.size(); ++i) {
			// local positions
			glm::mat4 localModel = glm::mat4();
			localModel = glm::translate(localModel, objectList.at(i)->GetLocalPosition());
			localModel = glm::rotate(localModel, objectList.at(i)->GetLocalRotationAngle(), objectList.at(i)->GetLocalRotation());
			localModel = glm::scale(localModel, objectList.at(i)->GetLocalScale());
			// world positions
			glm::mat4 worldModel = glm::mat4();
			worldModel = glm::translate(worldModel, objectList.at(i)->GetWorldPosition());
			worldModel = glm::rotate(worldModel, objectList.at(i)->GetWorldRotationAngle(), objectList.at(i)->GetWorldRotation());
			worldModel = glm::scale(worldModel, objectList.at(i)->GetWorldScale());
			// final positions
			if (objectList.at(i)->GetIsModel()) {
				modelShader->Use();
				modelShader->setMat4("model", worldModel * objectList.at(i)->modelMatrix * localModel);
				objectList.at(i)->Render(modelShader);
			}
			else {
				shader->Use();
				shader->setMat4("model", worldModel * localModel);
				objectList.at(i)->Render(shader);
			}			
		}
	}

	//Swap the buffers to show the back buffer that this frame was rendered on
	SDL_GL_SwapWindow(window->GetWindow());
}

void Renderer::AddObject(std::vector<GameObject*> list) {
	objectList = list;
}

void Renderer::ClearObjects() {
	objectList.clear();
	lightObjectList.clear();
}

void Renderer::AddLightObject(std::vector<Light*> list) {
	lightObjectList = list;
}
