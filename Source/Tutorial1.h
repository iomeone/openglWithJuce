/*
  ==============================================================================

    engine.h
    Created: 26 Sep 2019 10:12:57am
    Author:  user

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ShaderProgram.h"



class Sprite
{
public:
	Sprite(OpenGLContext& openGLContext) :VBO(0), VAO(0), EBO(0),
		_openGLContext(openGLContext)
	{	
	}


	~Sprite()
	{
		_openGLContext.extensions.glDeleteVertexArrays(1, &VAO);
		_openGLContext.extensions.glDeleteBuffers(1, &VBO);
		_openGLContext.extensions.glDeleteBuffers(1, &EBO);
	}



	void init(float x, float y, float width, float height)
	{

			_openGLContext.extensions.glGenVertexArrays(1, &VAO);
			_openGLContext.extensions.glGenBuffers(1, &VBO);
			_openGLContext.extensions.glGenBuffers(1, &EBO);

		float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		_openGLContext.extensions.glBindVertexArray(VAO);

		_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
		_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		_openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		_openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		_openGLContext.extensions.glEnableVertexAttribArray(0);
	 
		_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);

		_openGLContext.extensions.glBindVertexArray(0);
	}

	//void setShader(OpenGLShaderProgram * shader)
	//{
	//	attributes = nullptr;
	//	attributes = new Attributes(_openGLContext, *shader);

	//	uniforms = new Uniforms(_openGLContext, *shader);
	//}

	//void setColor(float r, float g, float b, float a)
	//{
	//	if (uniforms)
	//	{
	//		if (uniforms->lightPosition != nullptr)
	//			uniforms->lightPosition->set(r, g, b, a);
	//	}
	//}
	//void setTexture(GLint i)
	//{
	//	if (uniforms)
	//	{
	//		if (uniforms->texture != nullptr)
	//			uniforms->texture->set(i);
	//	}
	//}

	void draw()
	{

		_openGLContext.extensions.glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//_openGLContext.extensions.glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		 
		return;
		//if (attributes == nullptr || uniforms == nullptr)
		//	return;
		//if (!attributes->getStatus() || !uniforms->getStatus())
		//	return;


		//attributes->enable(_openGLContext);
		//double curTime = Time::getMillisecondCounterHiRes();

		//setColor(sin(curTime), 0, 1, 1);

		//setTexture(0);
		//	i = ::GetLastError();
			_openGLContext.extensions.glEnableVertexAttribArray(0);

			_openGLContext.extensions.glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);// sizeof(vertexData)/sizeof(float)/2

			_openGLContext.extensions.glDisableVertexAttribArray(0);
										 	//attributes->disable(_openGLContext);
										 	
		_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
		//	i = ::GetLastError();
	}

public:

	GLuint VBO, VAO, EBO;

	OpenGLContext& _openGLContext;
	

};




class Tutorial1 : public OpenGLAppComponent
{
public:
	//==============================================================================
	Tutorial1() : _sprite(openGLContext)
	{
		setSize(800, 600);
	}
	~Tutorial1()
	{

		shutdownOpenGL();
	}

	//==============================================================================
	void initialise() override
	{
		File f = File::getCurrentWorkingDirectory();
		auto vertexFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("vertex.h");
		auto fragmentFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("fragment.h");
		_shaderProgram.reset(new ShaderProgram(openGLContext, vertexFile.getFullPathName(), fragmentFile.getFullPathName()));

		_sprite.init(0.0, 0.0, 0.5, 0.5);
	}
	void shutdown() override
	{
		_shaderProgram->stopThread(1000);
		_shaderProgram.reset();
	}
	void render() override
	{
		bool isActive = OpenGLHelpers::isContextActive();
		if (!isActive)
			return;
	
		_shaderProgram->updateShader();
		OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);


		
		if (_shaderProgram->_shader)
		{
			_shaderProgram->_shader->use();
			_sprite.draw();
		}
			
		
	}

	//==============================================================================
	void paint(Graphics& g) override
	{
	
	}
	void resized() override
	{
		
	}

private:
	//==============================================================================
	// Your private member variables go here...

	Sprite _sprite;
	std::unique_ptr<ShaderProgram> _shaderProgram;
	bool init{ false };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial1)
};



 


 