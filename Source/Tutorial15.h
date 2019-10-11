/*
  ==============================================================================

    Tutorial15.h
    Created: 10 Oct 2019 11:06:24am
    Author:  user

  ==============================================================================
*/



#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ShaderProgram.h"
#include "Model.h"

#ifdef _WIN64
#pragma comment (lib, "../../ThirdParty/assimp/lib/assimp-vc141-mtd.lib")
#elif __APPLE_
#   error "todo: build mac version of assimp and setup"
#endif


namespace T15 {
	const String tutorialLink = "learnopengl.com/Model-Loading/Model";
	const String vertexFilename = "T15ModelVertex.h";
	const String fragmentFileName = "T15ModelFragment.h";


	const String lampVertexFilename = "T14LampVertex.h";
	const String lampFragmentFileName = "T14LampFragment.h";


#pragma pack(1)
	struct Vertex {
		struct Position {
			float x;
			float y;
			float z;
		};
		struct Normal
		{
			float nx;
			float ny;
			float nz;
		};
		struct UV {
			float u;
			float v;
		};

		Position position;
		Normal normal;
		UV uv;

		Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		{
			position.x = x;
			position.y = y;
			position.z = z;

			normal.nx = nx;
			normal.ny = ny;
			normal.nz = nz;

			uv.u = u;
			uv.v = v;
		}
	};
#pragma pack()

	Vertex vertices[] = {
		// positions          // normals           // texture coords
		{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f},
		{0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f},
		{0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f},
		{0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f},
		{-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f},

		{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f},
		{0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f},
		{0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f},
		{0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f},
		{-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f},

		{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		{-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f},
		{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f},
		{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f},

		{0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		{0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f},
		{0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f},
		{0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f},

		{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f},
		{0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f},
		{0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f},
		{0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f},
		{-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f},
		{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f},

		{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f},
		{0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f},
		{0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f},
		{0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f},
		{-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f},
		{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f}

	};


	class SpriteLamp : public SpriteBase
	{
	public:
		SpriteLamp(OpenGLContext& openglContext, Camera& camera) : SpriteBase(openglContext, camera)
		{

		}
		virtual void setupTexture() override{}  // light don't have any texture
		virtual void initBuffer() override
		{
			_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);


			_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
			_openGLContext.extensions.glEnableVertexAttribArray(1);

			_openGLContext.extensions.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
			_openGLContext.extensions.glEnableVertexAttribArray(2);
		}

		virtual void bindTexture() override{}  // light don't have any texture
		virtual void drawPost() override
		{
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		virtual UniformsBase * getUniformBase() override
		{
			jassert(_defaultUniform);  // you need call setUniformEnv
			return (UniformsBase*)_defaultUniform.get(); // we use default uniform
		}


	};





	class Tutorial15 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{
	public:
		//==============================================================================
		Tutorial15() : _model(openGLContext, _camera)/* : _sprite(openGLContext, _camera, _useCircle)*/
					, _lamp(openGLContext, _camera)
		{
			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
			openGLContext.setComponentPaintingEnabled(true);

			setWantsKeyboardFocus(true);

			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
			addAndMakeVisible(_lblCompileInfo.get());

			_camera.ProcessKeyboard(Camera_Movement::BACKWARD, 5.0f);
			setSize(800, 600);
		}
		~Tutorial15()
		{
			shutdownOpenGL();
		}

		//==============================================================================
		void initialise() override
		{
			File f = File::getCurrentWorkingDirectory();
			macPath(f);


			auto vertexFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexFilename);
			auto fragmentFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentFileName);
			_shaderProgram.reset(new ShaderProgram(openGLContext, vertexFile.getFullPathName(), fragmentFile.getFullPathName()));

			auto objFile = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("nanosuit").getChildFile("nanosuit.obj");
			jassert(objFile.existsAsFile());
			 _model.load(objFile.getFullPathName().toStdString());
			 _model.init();
			 auto m = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -8.f, 0.f));
			 _model.setModel(m);

			 auto vertexFileLamp = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(lampVertexFilename);
			 auto fragmentFileLamp = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(lampFragmentFileName);
			 _shaderProgramLamp.reset(new ShaderProgram(openGLContext, vertexFileLamp.getFullPathName(), fragmentFileLamp.getFullPathName()));
			 _lamp.init();



		}
		void shutdown() override
		{
			_shaderProgram->stopThread(1000);
			_shaderProgram.reset();
			TextureCache::clear();
		}
		void render() override
		{
			jassert(OpenGLHelpers::isContextActive());

			// we compile the model shader.
			auto res = _shaderProgram->updateShader();
			if (res >= 0)
			{
				if (res == 1) 
					_model.setUniformEnv(_shaderProgram->_shader);

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderProgram->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}


			// we compile the lamp shader.
			res = _shaderProgramLamp->updateShader();
			if (res >= 0)
			{
				if (res == 1)
					_lamp.setUniformEnv(_shaderProgramLamp->_shader);

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderProgramLamp->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}

			// change the lamp pos
			static float inc = 0.0;
			inc += 0.001;
			lightPos.x = 2 * sin(inc);
			lightPos.z = abs( 8 * cos(inc) );
			// change the light color
			glm::vec3 lightColor;
			lightColor.x = sin(inc * 2.0f);
			lightColor.y = sin(inc* 0.7f);
			lightColor.z = sin(inc * 1.3f);


			
			glEnable(GL_DEPTH_TEST);
			OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

			// we draw the model 
			glDisable(GL_BLEND);
			if (_shaderProgram->_shader)
			{

				_shaderProgram->_shader->use();
				_model.Draw(_camera.getCameraPos(), lightPos, lightColor);
			}
			glEnable(GL_BLEND);


			//we draw the lamp
			if (_shaderProgramLamp->_shader)
			{
				auto m = glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(.4f));
				_lamp.setModel(m);
				_shaderProgramLamp->_shader->use();
				_lamp.draw();
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

			_model.setScreenWidthAndHeight(getBounds().getWidth(), getBounds().getHeight());
		}

		void buttonClicked(Button* buttonThatWasClicked) override
		{
			//[UserbuttonClicked_Post]
			//[/UserbuttonClicked_Post]
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
			//_sprite.setAngleValue(_camera.getZoom());
		}

	private:
		//==============================================================================
		// Your private member variables go here...
		Camera _camera;
		std::unique_ptr<ShaderProgram> _shaderProgram;
		std::unique_ptr<Label> _lblCompileInfo{ nullptr };
 
		glm::vec3 lightPos{ 1.2f, 0.0f, 2.0f };


		Model _model;

		std::unique_ptr<ShaderProgram> _shaderProgramLamp{ nullptr };
		SpriteLamp _lamp;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial15)
	};
}
