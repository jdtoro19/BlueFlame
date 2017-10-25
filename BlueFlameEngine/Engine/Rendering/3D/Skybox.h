#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <glew/GL/glew.h>
#include <vector>
#include <SDL\SDL_image.h>
#include <iostream>

class Skybox
{
public:
	Skybox();
	~Skybox();

	unsigned int LoadTextures(std::vector<char*> faces);

	void Update(const float deltaTime);
	void Render();

private:	
	void LoadMesh();

	unsigned int VAO, VBO, textureID;

	std::vector<char*> faces;
};

#endif

