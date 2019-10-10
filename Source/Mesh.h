/*
  ==============================================================================

    Mesh.h
    Created: 9 Oct 2019 6:58:59pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>


#include "Camera.h"
#include "SpriteBase.h"
#include "TextureCache.h"
using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
	OpenGLTextureEx* tex{ nullptr };
    string type;
    string path;
};


struct Material {
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Ambient;
	float Shininess;
};

class UniformMesh : public UniformsBase
{
public:
	UniformMesh(OpenGLContext& openGLContext, OpenGLShaderProgram& shader) :
		UniformsBase(openGLContext, shader)
	{
		material_ambient = createUniform(openGLContext, shader, "material.ambient");
		material_diffuse = createUniform(openGLContext, shader, "material.diffuse");
		material_specular = createUniform(openGLContext, shader, "material.specular");
		material_shininess = createUniform(openGLContext, shader, "material.shininess");
	}

	void setTextureUniformIndex(String uniformName, GLint textureUnitIndex )
	{
		auto u = _UniformMap.find(uniformName);
		if ( u != _UniformMap.end())
		{
			u->second->set(textureUnitIndex);
		}
		else
		{
			auto nu = createUniform(_openglContext, _shader, uniformName.getCharPointer());
			
			if (nu)
			{
				nu->set(textureUnitIndex);
				_UniformMap[uniformName] = nu;
			}
			//else
			//	jassertfalse;

			DBG("new texture: " << uniformName);
		}
	}

	
	~UniformMesh()
	{		 
		for (auto i : _UniformMap) delete i.second;
	}

	std::map<String, OpenGLShaderProgram::Uniform* > _UniformMap;


	ScopedPointer<OpenGLShaderProgram::Uniform> material_ambient{ nullptr },  material_diffuse{ nullptr }, material_specular{ nullptr }, material_shininess{ nullptr };
};




class Mesh : public SpriteBaseEx {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

	Material _material;

	OpenGLContext& _openglContext;
	
    /*  Functions  */
    // constructor
    Mesh(OpenGLContext& openglContext, Camera& camera, vector<Vertex>& vertices, vector<unsigned int>& indices, vector<Texture>& textures, Material& material) :
		SpriteBaseEx(openglContext, camera),
		_openglContext(openglContext)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
		this->_material = material;
    }

   

	virtual void setupTexture() override
	{
		for (auto texture : textures)
		{
			texture.tex = TextureCache::getTexture(texture.path);
			jassert(texture.tex);
		}
	}

    /*  Functions    */
    // initializes all the buffer objects/arrays
	virtual void initBuffer() override
    {
        // load data into vertex buffers
		_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
		_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		_openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		_openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
		_openGLContext.extensions.glEnableVertexAttribArray(0);
		_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
		_openGLContext.extensions.glEnableVertexAttribArray(1);
		_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
		_openGLContext.extensions.glEnableVertexAttribArray(2);
		_openGLContext.extensions.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
		_openGLContext.extensions.glEnableVertexAttribArray(3);
		_openGLContext.extensions.glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
		_openGLContext.extensions.glEnableVertexAttribArray(4);
		_openGLContext.extensions.glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    }


	virtual void bindTexture() override
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			_openGLContext.extensions.glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

			// now set the sampler to the correct texture unit
			uniformMesh->setTextureUniformIndex((name + number).c_str(), i);

			// and finally bind the texture
			textures[i].tex = TextureCache::getTexture(textures[i].path);
			textures[i].tex->bind();

		}
	}


	virtual void drawPost()	override
	{
		// draw mesh
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	virtual UniformsBase * getUniformBase() override
	{
		return (UniformsBase*)uniformMesh.get();
	}

	void setUniformEnv( OpenGLShaderProgram *shader)
	{
		uniformMesh.reset(new UniformMesh(_openGLContext, *shader));
	}


	std::unique_ptr<UniformMesh> uniformMesh{ nullptr };
	 ;

};