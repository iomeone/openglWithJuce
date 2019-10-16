/*
  ==============================================================================

    Model.h
    Created: 9 Oct 2019 6:52:02pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "Camera.h"

#include "Mesh.h"
using namespace std;

class Model
{
public:
	/*  Model Data */
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;
	OpenGLContext& _openglContext;
	Camera& _camera;

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	Model( OpenGLContext& openglContext, Camera& camera, bool gamma = false) : gammaCorrection(gamma), _openglContext(openglContext), _camera(camera) {}
	void load(string const &path)
	{
		loadModel(path);
	}

	void init()
	{
		for (auto& mesh : meshes)	mesh.init();
	}

	void setModel(glm::mat4& m)
	{
		for (auto& mesh : meshes)
			mesh.setModel(m);
	}

	// draws the model, and thus all its meshes
	void Draw(glm::vec3 viewPos, glm::vec3 lightPos, glm::vec3 lightColor, float time = 0.0f)
	{

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // increase the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

		for (auto& mesh : meshes)
		{
			if (mesh.uniformMesh->time)
			{
				mesh.uniformMesh->time->set(time);
			}
			if (mesh.uniformMesh->light_ambient)
			{
				mesh.uniformMesh->light_ambient->set(ambientColor.r, ambientColor.g, ambientColor.b);
			}
			if (mesh.uniformMesh->light_diffuse)
			{
				mesh.uniformMesh->light_diffuse->set(diffuseColor.r, diffuseColor.g, diffuseColor.b);
			}
			if (mesh.uniformMesh->light_specular)
			{
				mesh.uniformMesh->light_specular->set(1.0f, 1.0f, 1.0f);
			}

			if (mesh.uniformMesh->light_position)
			{
				mesh.uniformMesh->light_position->set(lightPos.x, lightPos.y, lightPos.z);
			}

			if (mesh.uniformMesh->viewPos)
				mesh.uniformMesh->viewPos->set(viewPos.x, viewPos.y, viewPos.z);

			if(mesh.uniformMesh->material_ambient)
			mesh.uniformMesh->material_ambient->set( mesh._material.Ambient.r, mesh._material.Ambient.g, mesh._material.Ambient.b);

			if (mesh.uniformMesh->material_diffuse)
			mesh.uniformMesh->material_diffuse->set(mesh._material.Diffuse.r, mesh._material.Diffuse.g, mesh._material.Diffuse.b);

			if (mesh.uniformMesh->material_specular)
			mesh.uniformMesh->material_specular->set(mesh._material.Specular.r, mesh._material.Specular.g, mesh._material.Specular.b);

			if (mesh.uniformMesh->material_shininess)
			mesh.uniformMesh->material_shininess->set(mesh._material.Shininess);


			mesh.draw();
		}

	}

	void setUniformEnv(OpenGLShaderProgram *shader)	
	{	
		for (auto& mesh : meshes)	mesh.setUniformEnv(shader);	
	}


	void setScreenWidthAndHeight(float w, float h)
	{
		for (auto& mesh : meshes)
			mesh.setScreenWidthAndHeight(w, h);
	}

private:
	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string const &path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace );
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// data to fill
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			if (mesh->mTangents)
			{
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
			}

			vertex.Tangent = vector;
			// bitangent
			if (mesh->mBitangents)
			{
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
			}
			vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		auto m = loadMaterial(material);
		// return a mesh object created from the extracted mesh data
		return Mesh(_openglContext, _camera, vertices, indices, textures, m);
	}

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			String textureFullPath = str.C_Str();
		 
			if (!File::isAbsolutePath(str.C_Str()))
			{
				File f(directory);
				textureFullPath = f.getParentDirectory().getChildFile(str.C_Str()).getFullPathName();
			}
			
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.type = typeName;
				texture.path = textureFullPath.toStdString();
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}


	Material loadMaterial(aiMaterial* mat) {
		Material material;
		aiColor3D color(0.f, 0.f, 0.f);
		float shininess;

		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.Diffuse = glm::vec3(color.r, color.b, color.g);

		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material.Ambient = glm::vec3(color.r, color.b, color.g);

		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material.Specular = glm::vec3(color.r, color.b, color.g);

		mat->Get(AI_MATKEY_SHININESS, shininess);
		material.Shininess = shininess;

		return material;
	}
};
