#pragma once
#include "Texture.h"
#include "Vertex.h"
#include <vector>
#include "Shader.h"
#include "gl/glew.h"
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;
	Mesh(std::vector<Vertex>_vertices, std::vector<Texture> _textures, std::vector<unsigned int> _indices);
	Mesh(float* vertices);
	void draw(Shader  * const shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

