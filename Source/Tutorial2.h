/*
  ==============================================================================

    Tutorial2.h
    Created: 26 Sep 2019 11:36:18am
    Author:  user

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ShaderProgram.h"




struct Uniforms
{
	Uniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
	{
		ourColor = createUniform(openGLContext, shader, "uniformColor");
	}
 
	ScopedPointer<OpenGLShaderProgram::Uniform> ourColor{ nullptr };
	

private:
	static OpenGLShaderProgram::Uniform* createUniform(OpenGLContext& openGLContext,
		OpenGLShaderProgram& shader,
		const char* uniformName)
	{
		if (openGLContext.extensions.glGetUniformLocation(shader.getProgramID(), uniformName) < 0)
			return nullptr;

		return new OpenGLShaderProgram::Uniform(shader, uniformName);
	}
};




class SpriteTutorial2
{
public:
	SpriteTutorial2(OpenGLContext& openGLContext, bool& useUniform) :VBO(0), VAO(0), EBO(0),
		_openGLContext(openGLContext),
		_useUniform(useUniform)
	{
	}


	~SpriteTutorial2()
	{
		_openGLContext.extensions.glDeleteVertexArrays(1, &VAO);
		_openGLContext.extensions.glDeleteBuffers(1, &VBO);
		_openGLContext.extensions.glDeleteBuffers(1, &EBO);
	}



	void init()
	{

		_openGLContext.extensions.glGenVertexArrays(1, &VAO);
		_openGLContext.extensions.glGenBuffers(1, &VBO);
		_openGLContext.extensions.glGenBuffers(1, &EBO);

		float vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f // top left 
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

		// position attribute
		_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		_openGLContext.extensions.glEnableVertexAttribArray(0);
		// color attribute
		_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		_openGLContext.extensions.glEnableVertexAttribArray(1);



		_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);

		_openGLContext.extensions.glBindVertexArray(0);
	}

	void setUniformEnv(OpenGLContext& ogc, OpenGLShaderProgram *shader)
	{
		 _uniforms.reset(new Uniforms(ogc, *shader));
	}

	void draw()
	{

		_openGLContext.extensions.glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

		if (_uniforms->ourColor && _useUniform)
		{
			float x = abs(cos(juce::Time::getCurrentTime().getMilliseconds() / 500.f));
			_uniforms->ourColor->set(0.f, x, 0.0f, 1.0f);
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//_openGLContext.extensions.glBindVertexArray(0); // no need to unbind it every time 
		return;
	}

public:

	GLuint VBO, VAO, EBO;

	OpenGLContext& _openGLContext;

	bool& _useUniform;


	std::unique_ptr<Uniforms> _uniforms;

};




class Tutorial2 : public OpenGLAppComponent ,
	public Button::Listener
{
public:
	//==============================================================================
	Tutorial2() : _sprite(openGLContext, _useUniform)
	{
		openGLContext.setComponentPaintingEnabled(true);

		_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
		addAndMakeVisible(_lblCompileInfo.get());


		_useUniformToggleButton.reset(new ToggleButton("Use Uniform"));
		addAndMakeVisible(_useUniformToggleButton.get());
		_useUniformToggleButton->addListener(this);
		



		setSize(800, 600);
	}
	~Tutorial2()
	{

		shutdownOpenGL();
	}

	//==============================================================================
	void initialise() override
	{
		File f = File::getCurrentWorkingDirectory();
		macPath(f);
		auto vertexFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile("vertexT2.h");
		auto fragmentFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile("fragmentT2.h");
		_shaderProgram.reset(new ShaderProgram(openGLContext, vertexFile.getFullPathName(), fragmentFile.getFullPathName()));

		_sprite.init();
	}
	void shutdown() override
	{
		_shaderProgram->stopThread(1000);
		_shaderProgram.reset();
	}
	void render() override
	{
		if (!OpenGLHelpers::isContextActive())
			return;
		auto res = _shaderProgram->updateShader();
		if (res >=  0 )
		{
			if (res == 1)
			{
				_sprite.setUniformEnv(openGLContext, _shaderProgram->_shader);
			}
			

			const MessageManagerLock mmLock;
			if (mmLock.lockWasGained())
			{
				_lblCompileInfo->setText(_shaderProgram->getCompileResult() + " \nlearnopengl.com/Getting-started/Shaders", NotificationType::dontSendNotification);
			}
		}

		OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT);

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
		auto r = getLocalBounds();
		if (_lblCompileInfo)
		{
			_lblCompileInfo->setBounds(r.removeFromTop(proportionOfHeight(0.05000f)));
		}

		auto areaToggle = r.removeFromRight(proportionOfWidth(0.1));

		_useUniformToggleButton->setBounds(areaToggle.removeFromTop(proportionOfHeight(0.05)));
		

	}

	void buttonClicked(Button* buttonThatWasClicked)
	{

		if (buttonThatWasClicked == _useUniformToggleButton.get())
		{
			_useUniform = _useUniformToggleButton->getToggleState();
		}

		//[UserbuttonClicked_Post]
		//[/UserbuttonClicked_Post]
	}

private:
	//==============================================================================
	// Your private member variables go here...

	SpriteTutorial2 _sprite;
	std::unique_ptr<ShaderProgram> _shaderProgram;
	std::unique_ptr<Label> _lblCompileInfo{ nullptr };
	bool init{ false };

	bool _useUniform{ false };
	bool _useAttribute{ false };

	std::unique_ptr<ToggleButton> _useUniformToggleButton;



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial2)
};
