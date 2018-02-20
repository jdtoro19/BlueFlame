#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component.h"
#include "../Rendering/3D/Mesh.h"
#include "../Graphics/Shader.h"

namespace ENGINE {

	class RenderComponent : public Component
	{
	public:
		enum Render_Type {
			NONE,
			CUBE,
			QUAD
		};

		RenderComponent();
		~RenderComponent();

		void SetRenderType(Render_Type renderType);
		void SetColour(float r, float g, float b);
		void Update();
		void Render(Shader* shader);

		std::vector<Vertex> getVertexList() { return vertexList; }

		void CanRender(bool b) { canRender = b; }

	private:
		Vertex v;
		std::vector<Vertex> vertexList;
		glm::vec3 colour;
		Render_Type renderType;
		Mesh* mesh;
		bool canRender = true;
	};
}
#endif

