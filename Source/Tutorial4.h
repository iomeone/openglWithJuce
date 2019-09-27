#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ShaderProgram.h"
#include "TextureCache.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct UniformsTutorial4
{
	UniformsTutorial4(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
	{
		transform = createUniform(openGLContext, shader, "transform");
		ourTextureBox = createUniform(openGLContext, shader, "ourTextureBox");
		ourTextureFace = createUniform(openGLContext, shader, "ourTextureFace");
	}

	ScopedPointer<OpenGLShaderProgram::Uniform> transform{ nullptr }, ourTextureBox{ nullptr }, ourTextureFace{ nullptr };


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




class SpriteTutorial4
{
public:
	SpriteTutorial4(OpenGLContext& openGLContext, bool& useUniform) :VBO(0), VAO(0), EBO(0),
		_openGLContext(openGLContext),
		_useUniform(useUniform)
	{
	}


	~SpriteTutorial4()
	{
		_openGLContext.extensions.glDeleteVertexArrays(1, &VAO);
		_openGLContext.extensions.glDeleteBuffers(1, &VBO);
		_openGLContext.extensions.glDeleteBuffers(1, &EBO);
	}



	void init(String BoxPath, String awsomeFacePath)
	{
		_pTextureBox = TextureCache::getTexture(BoxPath);
		_pTextureFace = TextureCache::getTexture(awsomeFacePath);

		if (!_pTextureBox || !_pTextureFace)
		{
			return;
		}

		_openGLContext.extensions.glGenVertexArrays(1, &VAO);
		_openGLContext.extensions.glGenBuffers(1, &VBO);
		_openGLContext.extensions.glGenBuffers(1, &EBO);

		float vertices[] = {
			// positions          // colors           // texture coords
			  0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			  0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			 -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			 -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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
		_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		_openGLContext.extensions.glEnableVertexAttribArray(0);
		// color attribute
		_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		_openGLContext.extensions.glEnableVertexAttribArray(1);

		_openGLContext.extensions.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		_openGLContext.extensions.glEnableVertexAttribArray(2);

		_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);

		_openGLContext.extensions.glBindVertexArray(0);
	}

	void setUniformEnv(OpenGLContext& ogc, OpenGLShaderProgram *shader)
	{
		_uniforms.reset(new UniformsTutorial4(ogc, *shader));
	}

	void draw()
	{
		_openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
		if (_pTextureBox)
			_pTextureBox->bind();
		else return;

		_openGLContext.extensions.glActiveTexture(GL_TEXTURE1);
		if (_pTextureFace)
			_pTextureFace->bind();
		else return;

		if (_uniforms->ourTextureBox)
			_uniforms->ourTextureBox->set(0);
		if (_uniforms->ourTextureFace)
			_uniforms->ourTextureFace->set(1);


		_openGLContext.extensions.glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

		if (_uniforms->transform)
		{
			static float radian = 0.;
			radian += 0.05;

			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 identityMatrix = glm::mat4(1.0f);
			trans = glm::rotate(trans, glm::radians(radian), glm::vec3(0.0, 0.0, 1.0));
			trans = glm::scale(trans, glm::vec3(0.8, 0.8, 0.8));

			if (_useUniform)
				_uniforms->transform->setMatrix4(glm::value_ptr(trans), 1, GL_FALSE);
			else
				_uniforms->transform->setMatrix4(glm::value_ptr(identityMatrix), 1, GL_FALSE);


		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
		return;
	}

public:

	GLuint VBO, VAO, EBO;

	OpenGLContext& _openGLContext;

	bool& _useUniform;
	OpenGLTexture* _pTextureBox{ nullptr }, *_pTextureFace{ nullptr };

	std::unique_ptr<UniformsTutorial4> _uniforms;

};




class Tutorial4 : public OpenGLAppComponent,
	public Button::Listener
{
public:
	//==============================================================================
	Tutorial4() : _sprite(openGLContext, _useUniform)
	{
		openGLContext.setComponentPaintingEnabled(true);

		_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
		addAndMakeVisible(_lblCompileInfo.get());


		_useUniformToggleButton.reset(new ToggleButton("Use Uniform"));
		addAndMakeVisible(_useUniformToggleButton.get());
		_useUniformToggleButton->addListener(this);




		setSize(800, 600);
	}
	~Tutorial4()
	{
		shutdownOpenGL();
	}

	//==============================================================================
	void initialise() override
	{
		File f = File::getCurrentWorkingDirectory();
		auto vertexFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile("vertexT4.h");
		auto fragmentFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile("fragmentT4.h");
		_shaderProgram.reset(new ShaderProgram(openGLContext, vertexFile.getFullPathName(), fragmentFile.getFullPathName()));

		auto textureBoxFile = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("container.jpg").getFullPathName();
		auto textureAwsomeFace = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("awesomeface.png").getFullPathName();

		_sprite.init(textureBoxFile, textureAwsomeFace);
	}
	void shutdown() override
	{
		_shaderProgram->stopThread(1000);
		_shaderProgram.reset();
		TextureCache::clear();
	}
	void render() override
	{
		if (!OpenGLHelpers::isContextActive())
			return;
		auto res = _shaderProgram->updateShader();
		if (res >= 0)
		{
			if (res == 1)
			{
				_sprite.setUniformEnv(openGLContext, _shaderProgram->_shader);
			}

			const MessageManagerLock mmLock;
			if (mmLock.lockWasGained())
			{
				_lblCompileInfo->setText(_shaderProgram->getCompileResult() + " \nlearnopengl.com/Getting-started/Textures", NotificationType::dontSendNotification);
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

	SpriteTutorial4 _sprite;
	std::unique_ptr<ShaderProgram> _shaderProgram;
	std::unique_ptr<Label> _lblCompileInfo{ nullptr };
	bool init{ false };

	bool _useUniform{ false };
	bool _useAttribute{ false };

	std::unique_ptr<ToggleButton> _useUniformToggleButton;



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial4)
};
