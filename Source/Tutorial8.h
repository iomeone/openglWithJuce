#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ShaderProgram.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace T8 {
	const String tutorialLink = "learnopengl.com/Lighting/Basic-Lighting";

	const String lightVertexFilename = "T8ColorsVertex.h";
	const String lightFragmentFileName = "T8ColorsFragment.h";

	const String lampVertexFilename = "T8LampVertex.h";
	const String lampFragmentFileName = "T8LampFragment.h";


	struct Uniforms
	{
		Uniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
		{
			model = createUniform(openGLContext, shader, "model");
			view = createUniform(openGLContext, shader, "view");
			projection = createUniform(openGLContext, shader, "projection");
			objectColor = createUniform(openGLContext, shader, "objectColor");
			lightColor = createUniform(openGLContext, shader, "lightColor");
		}

		ScopedPointer<OpenGLShaderProgram::Uniform>  model{ nullptr }, view{ nullptr }, projection{ nullptr }, objectColor{ nullptr }, lightColor{ nullptr };


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

#pragma pack(1)
	struct Vertex {
		struct Position {
			float x;
			float y;
			float z;
		};

		Position position;

		Vertex(float x, float y, float z)
		{
			position.x = x;
			position.y = y;
			position.z = z;


		}
	};
#pragma pack()

	class Sprite
	{
	public:
		Sprite(OpenGLContext& openGLContext, Camera& camera, glm::mat4 model = glm::mat4(1.0f)) :VBO(0), VAO(0),
			_openGLContext(openGLContext),
			_camera(camera),
			_model(model)
		{
		}

		void setModel(glm::mat4 model)
		{
			_model = model;
		}


		~Sprite()
		{
			_openGLContext.extensions.glDeleteVertexArrays(1, &VAO);
			_openGLContext.extensions.glDeleteBuffers(1, &VBO);
		}

		void init()
		{
			_openGLContext.extensions.glGenVertexArrays(1, &VAO);
			_openGLContext.extensions.glGenBuffers(1, &VBO);

			Vertex vertices[] = {
				{-0.5f, -0.5f, -0.5f},
				{ 0.5f, -0.5f, -0.5f},
				{ 0.5f,  0.5f, -0.5f},
				{ 0.5f,  0.5f, -0.5f},
				{-0.5f,  0.5f, -0.5f},
				{-0.5f, -0.5f, -0.5f},

				{-0.5f, -0.5f,  0.5f},
				{ 0.5f, -0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f},
				{-0.5f,  0.5f,  0.5f},
				{-0.5f, -0.5f,  0.5f},

				{-0.5f,  0.5f,  0.5f},
				{-0.5f,  0.5f, -0.5f},
				{-0.5f, -0.5f, -0.5f},
				{-0.5f, -0.5f, -0.5f},
				{-0.5f, -0.5f,  0.5f},
				{-0.5f,  0.5f,  0.5f},

				{ 0.5f,  0.5f,  0.5f},
				{ 0.5f,  0.5f, -0.5f},
				{ 0.5f, -0.5f, -0.5f},
				{ 0.5f, -0.5f, -0.5f},
				{ 0.5f, -0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f},

				{-0.5f, -0.5f, -0.5f},
				{ 0.5f, -0.5f, -0.5f},
				{ 0.5f, -0.5f,  0.5f},
				{ 0.5f, -0.5f,  0.5f},
				{-0.5f, -0.5f,  0.5f},
				{-0.5f, -0.5f, -0.5f},

				{-0.5f,  0.5f, -0.5f},
				{ 0.5f,  0.5f, -0.5f},
				{ 0.5f,  0.5f,  0.5f},
				{ 0.5f,  0.5f,  0.5f},
				{-0.5f,  0.5f,  0.5f},
				{-0.5f,  0.5f, -0.5f}
			};

			_openGLContext.extensions.glBindVertexArray(VAO);

			_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);


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
			if (_uniforms->view)
			{
				glm::mat4 view = _camera.getViewMat();
				_uniforms->view->setMatrix4(glm::value_ptr(view), 1, GL_FALSE);
			}

			if (_uniforms->projection)
			{
				glm::mat4 projection;
				projection = glm::perspective(_camera.getZoom(), _screenWidth / _screenHeight, 0.1f, 100.0f);
				_uniforms->projection->setMatrix4(glm::value_ptr(projection), 1, GL_FALSE);
			}

			if (_uniforms->model)
			{
				_uniforms->model->setMatrix4(glm::value_ptr(_model), 1, GL_FALSE);
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);


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

		std::unique_ptr<Uniforms> _uniforms;

		glm::mat4 _model;

	};



	class Tutorial8 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{
	public:
		//==============================================================================
		Tutorial8() :
			_spriteLight(openGLContext, _camera, glm::mat4(1.0f)),
			_spriteLamp(openGLContext, _camera)
		{
			setWantsKeyboardFocus(true);
			_camera.setCameraPosZ(5);
			auto m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 1.0f, 2.0f)), glm::vec3(.2f));
			_spriteLamp.setModel(m);

			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
			openGLContext.setComponentPaintingEnabled(true);

			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
			addAndMakeVisible(_lblCompileInfo.get());


			setSize(800, 600);
		}
		~Tutorial8()
		{
			shutdownOpenGL();
		}

		//==============================================================================
		void initialise() override
		{
			File f = File::getCurrentWorkingDirectory();
			macPath(f);

			auto vertexFileLight = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(lightVertexFilename);
			auto fragmentFileLight = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(lightFragmentFileName);
			_shaderProgramLight.reset(new ShaderProgram(openGLContext, vertexFileLight.getFullPathName(), fragmentFileLight.getFullPathName()));


			auto vertexFileLamp = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(lampVertexFilename);
			auto fragmentFileLamp = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(lampFragmentFileName);
			_shaderProgramLamp.reset(new ShaderProgram(openGLContext, vertexFileLamp.getFullPathName(), fragmentFileLamp.getFullPathName()));


			_spriteLight.init();
			_spriteLamp.init();

		}
		void shutdown() override
		{
			_shaderProgramLight->stopThread(1000);
			_shaderProgramLight.reset();
		}

		void updateShader(ShaderProgram* shaderProg, Sprite& sprite)
		{
			auto res = shaderProg->updateShader();
			if (res >= 0)
			{
				if (res == 1)
				{
					sprite.setUniformEnv(openGLContext, shaderProg->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
				{
					_lblCompileInfo->setText(shaderProg->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);
				}
			}
		}
		void render() override
		{
			if (!OpenGLHelpers::isContextActive())
				return;


			updateShader(_shaderProgramLight.get(), _spriteLight);
			updateShader(_shaderProgramLamp.get(), _spriteLamp);

			OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);


			if (_shaderProgramLight->_shader)
			{
				_shaderProgramLight->_shader->use();


				if (_spriteLight._uniforms)
				{
					if (_spriteLight._uniforms->lightColor)
					{
						_spriteLight._uniforms->lightColor->set(1.0f, 0.5f, 0.31f);
					}
					if (_spriteLight._uniforms->objectColor)
					{
						_spriteLight._uniforms->objectColor->set(1.0f, 1.0f, 1.0f);
					}
				}

				_spriteLight.draw();
			}


			if (_shaderProgramLamp->_shader)
			{
				_shaderProgramLamp->_shader->use();
				_spriteLamp.draw();
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

			auto areaToggle = r.removeFromRight(proportionOfWidth(0.5));

			_spriteLight.setScreenWidthAndHeight(getBounds().getWidth(), getBounds().getHeight());
			_spriteLamp.setScreenWidthAndHeight(getBounds().getWidth(), getBounds().getHeight());
		}

		void buttonClicked(Button* buttonThatWasClicked) override
		{

		}

		void sliderValueChanged(Slider* sliderThatWasMoved) override
		{

		}
		bool keyPressed(const KeyPress& key) override
		{
			float cameraSpeed = 0.5f; //
			if (key == 'w' || key == 'W')
			{
				_camera.ProcessKeyboard(Camera_Movement::FORWARD, cameraSpeed);

			}
			if (key == 's' || key == 'S')
			{
				_camera.ProcessKeyboard(Camera_Movement::BACKWARD, cameraSpeed);
			}
			if (key == 'a' || key == 'A')
			{
				_camera.ProcessKeyboard(Camera_Movement::LEFT, cameraSpeed);

			}
			if (key == 'd' || key == 'D')
			{
				_camera.ProcessKeyboard(Camera_Movement::RIGHT, cameraSpeed);
			}
			return false;
		}

		bool firstMouse{ true };
		void mouseMove(const MouseEvent& e) override
		{

			static float lastX = 0., lastY = 0.;

			float xpos = e.getPosition().x;
			float ypos = e.getPosition().y;

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

			lastX = xpos;
			lastY = ypos;


			_camera.ProcessMouseMovement(xoffset, yoffset);

		}


		void mouseEnter(const MouseEvent& event) override
		{
			firstMouse = true;
		}



		void mouseExit(const MouseEvent& event) override
		{
			firstMouse = true;
		}

		void mouseWheelMove(const MouseEvent& event,
			const MouseWheelDetails& wheel) override
		{
			_camera.ProcessMouseScroll(wheel.deltaY * 10.);
		}



	private:
		//==============================================================================
		// Your private member variables go here...
		Camera _camera;
		Sprite _spriteLight;
		Sprite _spriteLamp;

		std::unique_ptr<ShaderProgram> _shaderProgramLight;
		std::unique_ptr<ShaderProgram> _shaderProgramLamp;
		std::unique_ptr<Label> _lblCompileInfo{ nullptr };
		bool init{ false };


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial8)
	};
}