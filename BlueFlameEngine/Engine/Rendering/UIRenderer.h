#pragma once
#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "../Graphics/Shader.h" 
#include "../Core/Window.h" 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "2D/TextUI.h"

namespace ENGINE {

	class UIRenderer
	{
	private:
		std::map<GLchar, Character> Characters;
		GLuint VAO, VBO;

		Shader* textShader;

		float width;
		float height;

		glm::mat4 projection;

	public:
		UIRenderer();
		~UIRenderer();

		void Initialize(Window* window);
		void Draw(Window* window, std::vector<UIObject*> uiObjectList);

		void DebugText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);	

		float GetWidth();
		float GetHeight();
	};

}
#endif

