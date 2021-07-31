#pragma once
#include "Mesh.h"
#include "stb_image.h"
#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
	
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh>    meshes;
	std::string directory;
	bool gammaCorrection;
	Model(std::string const& path, bool gamma = false) ;
	void Draw(Shader * shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].draw(shader);
	}

private:
	void processNode(aiNode* node,const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void loadModel(std::string const & path);

};

