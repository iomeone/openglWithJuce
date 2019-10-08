/*
  ==============================================================================

    SpriteBase.h
    Created: 8 Oct 2019 7:07:23pm
    Author:  user

  ==============================================================================
*/

#pragma once

class UniformsBase
{
public:
	UniformsBase(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
	{
		model = createUniform(openGLContext, shader, "model");
		view = createUniform(openGLContext, shader, "view");
		projection = createUniform(openGLContext, shader, "projection");
	}

	ScopedPointer<OpenGLShaderProgram::Uniform>  model{ nullptr }, view{ nullptr }, projection{ nullptr };

public:
	static OpenGLShaderProgram::Uniform* createUniform(OpenGLContext& openGLContext,
		OpenGLShaderProgram& shader,
		const char* uniformName)
	{
		if (openGLContext.extensions.glGetUniformLocation(shader.getProgramID(), uniformName) < 0)
			return nullptr;

		return new OpenGLShaderProgram::Uniform(shader, uniformName);
	}
};



class SpriteBase
{
public:
	SpriteBase(OpenGLContext& openGLContext, Camera& camera, glm::mat4 model = glm::mat4(1.0f)) :VBO(0), VAO(0),
		_openGLContext(openGLContext),
		_camera(camera),
		_model(model)
	{
	}

	void setModel(glm::mat4 model)
	{
		_model = model;
	}


	~SpriteBase()
	{
		_openGLContext.extensions.glDeleteVertexArrays(1, &VAO);
		_openGLContext.extensions.glDeleteBuffers(1, &VBO);
	}

	virtual void initPre() {};
	virtual void initBuffer() = 0;
	virtual void drawPost() = 0;
	virtual UniformsBase * getUniformBase() = 0;

	void init()
	{
		initPre();
		_openGLContext.extensions.glGenVertexArrays(1, &VAO);
		_openGLContext.extensions.glGenBuffers(1, &VBO);


		_openGLContext.extensions.glBindVertexArray(VAO);

		_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);


		initBuffer();


		_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
		_openGLContext.extensions.glBindVertexArray(0);
	}

	void draw()
	{
		_openGLContext.extensions.glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		if (getUniformBase()->view)
		{
			glm::mat4 view = _camera.getViewMat();
			getUniformBase()->view->setMatrix4(glm::value_ptr(view), 1, GL_FALSE);
		}

		if (getUniformBase()->projection)
		{
			glm::mat4 projection;
			projection = glm::perspective(_camera.getZoom(), _screenWidth / _screenHeight, 0.1f, 100.0f);
			getUniformBase()->projection->setMatrix4(glm::value_ptr(projection), 1, GL_FALSE);
		}

		if (getUniformBase()->model)
		{
			getUniformBase()->model->setMatrix4(glm::value_ptr(_model), 1, GL_FALSE);
		}

		drawPost();


		return;
	}
	void setScreenWidthAndHeight(float w, float h)
	{
		_screenWidth = w;
		_screenHeight = h;
	}

public:

	float _screenWidth{ 1200.f };
	float _screenHeight{ 800.f };

	GLuint VBO, VAO;

	OpenGLContext& _openGLContext;
	Camera& _camera;

	glm::mat4 _model;
};