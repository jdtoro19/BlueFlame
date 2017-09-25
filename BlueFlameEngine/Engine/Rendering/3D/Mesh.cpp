#include "Mesh.h"

using namespace ENGINE;

Mesh::Mesh(std::vector<Vertex>* vertList_)
	: VBO(0), VAO(0), vertexList(std::vector<Vertex>()) {

	vertexList = *vertList_;

	GenerateBuffers();
}

Mesh::~Mesh() {
	//Delete the VAO & VBO from the GPU and clear their data
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::GenerateBuffers() {

	//Generate the VAO & VBO
	//Pass in the VAO & VBO variable as a pointer to be filled by the GPU
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind the VAO and VBO to the GPU to activate them for use
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Fill the VBO that you activated above with your vertex data
	//(type of Buffer, size of array, pointer to the first item in array, type of draw call)
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(Vertex), &vertexList[0], GL_STATIC_DRAW);

	//Enable a vertex attribute array
	//This will be where we define that data we are passing to our VBO
	glEnableVertexAttribArray(0);

	//Assign thd data to the specific attribute pointer you just made
	//(attribute pointer, number of variables, type of variable, 
	//is it normalized?, size of step to next vert, pointer to the spot in the Vertex this data is stored)
	//POSITION
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//The offset specifies where in our structure these specific variables are located
	//NORMAL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	//COLOR
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

	//Clear the vertex array and the buffer so no one else can access it or push to it
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Render() {
	//Bind the VAO that you want to use for drawing
	glBindVertexArray(VAO);

	//Draw the array stored in the bound VAO
	//(type of render, start of array, end of array)
	glDrawArrays(GL_TRIANGLES, 0, vertexList.size());

	//Clear the vertex array for future use
	glBindVertexArray(0);
}