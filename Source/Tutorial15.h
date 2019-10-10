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
// todo: build mac version of assimp

#endif






namespace T15 {
	const String tutorialLink = "learnopengl.com/Model-Loading/Model";
	const String vertexFilename = "vertexT15.h";
	const String fragmentFileName = "fragmentT15.h";


	class Tutorial15 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{
	public:
		//==============================================================================
		Tutorial15() : _model(openGLContext, _camera)/* : _sprite(openGLContext, _camera, _useCircle)*/
		{
			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
			openGLContext.setComponentPaintingEnabled(true);

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


			//_sprite.init(textureBoxFile, textureAwsomeFace);
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
					//todo: should set the mesh env
					//_sprite.setUniformEnv(openGLContext, _shaderProgram->_shader);
					_model.setUniformEnv(_shaderProgram->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
				{
					_lblCompileInfo->setText(_shaderProgram->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);
				}
			}

			OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);


			if (_shaderProgram->_shader)
			{
				_shaderProgram->_shader->use();
				// todo: draw the mesh
				//_sprite.draw();
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

			_useCircleDemo->setBounds(areaToggle.removeFromTop(proportionOfHeight(0.05)));
			_sliderAngle->setBounds(areaToggle.removeFromTop(proportionOfHeight(0.05)));
			_sliderZ->setBounds(areaToggle.removeFromTop(proportionOfHeight(0.05)));


			//todo: setthe mesh screnn w and h?
			//_sprite.setScreenWidthAndHeight(getBounds().getWidth(), getBounds().getHeight());
		}

		void buttonClicked(Button* buttonThatWasClicked) override
		{

			if (buttonThatWasClicked == _useCircleDemo.get())
			{
				_useCircle = _useCircleDemo->getToggleState();
				if (!_useCircle)
					_camera.setCameraPos({ 0.0f, 0.0f, 3.0f });

			}

			//[UserbuttonClicked_Post]
			//[/UserbuttonClicked_Post]
		}

		void sliderValueChanged(Slider* sliderThatWasMoved) override
		{
			if (sliderThatWasMoved == _sliderAngle.get())
			{
				//_sprite.setAngleValue(_sliderAngle->getValue());
			}
			else if (sliderThatWasMoved == _sliderZ.get())
			{
				//_sprite.setZValue(_sliderZ->getValue());
			}
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
		//SpriteTutorial6 _sprite;
		std::unique_ptr<ShaderProgram> _shaderProgram;
		std::unique_ptr<Label> _lblCompileInfo{ nullptr };
		bool init{ false };

		bool _useCircle{ false };
		bool _useAttribute{ false };

		std::unique_ptr<ToggleButton> _useCircleDemo;

		std::unique_ptr<Slider> _sliderAngle;
		std::unique_ptr<Slider> _sliderZ;

		Label _lblAngle;

		Label _lblZ;
		Model _model;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial15)
	};
}












//
//namespace T15 {
//	class Sprite
//	{
//	public:
//		Sprite(OpenGLContext& openGLContext) :VBO(0), VAO(0), EBO(0),
//			_openGLContext(openGLContext)
//		{
//		}
//
//
//		~Sprite()
//		{
//			_openGLContext.extensions.glDeleteVertexArrays(1, &VAO);
//			_openGLContext.extensions.glDeleteBuffers(1, &VBO);
//			_openGLContext.extensions.glDeleteBuffers(1, &EBO);
//		}
//
//
//
//		void init()
//		{
//
//			_openGLContext.extensions.glGenVertexArrays(1, &VAO);
//			_openGLContext.extensions.glGenBuffers(1, &VBO);
//			_openGLContext.extensions.glGenBuffers(1, &EBO);
//
//			float vertices[] = {
//			 0.5f,  0.5f, 0.0f,  // top right
//			 0.5f, -0.5f, 0.0f,  // bottom right
//			-0.5f, -0.5f, 0.0f,  // bottom left
//			-0.5f,  0.5f, 0.0f   // top left 
//			};
//			unsigned int indices[] = {  // note that we start from 0!
//				0, 1, 3,  // first Triangle
//				1, 2, 3   // second Triangle
//			};
//
//			_openGLContext.extensions.glBindVertexArray(VAO);
//
//			_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
//			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//
//			_openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//			_openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//
//			_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//			_openGLContext.extensions.glEnableVertexAttribArray(0);
//
//			_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//			_openGLContext.extensions.glBindVertexArray(0);
//		}
//
//
//		void draw()
//		{
//
//			_openGLContext.extensions.glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//
//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//			//_openGLContext.extensions.glBindVertexArray(0); // no need to unbind it every time 
//			return;
//		}
//
//	public:
//
//		GLuint VBO, VAO, EBO;
//
//		OpenGLContext& _openGLContext;
//
//
//	};
//
//
//
//
//	class Tutorial15 : public OpenGLAppComponent
//	{
//	public:
//		//==============================================================================
//		Tutorial15() : _sprite(openGLContext)
//		{
//			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
//			openGLContext.setComponentPaintingEnabled(true);
//
//			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
//			addAndMakeVisible(_lblCompileInfo.get());
//
//			
//
//
//			setSize(800, 600);
//		}
//		~Tutorial15()
//		{
//
//			shutdownOpenGL();
//		}
//
//
//		//==============================================================================
//		void initialise() override
//		{
//			File f = File::getCurrentWorkingDirectory();
//			macPath(f);
//			auto vertexFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile("vertexT1.h");
//			auto fragmentFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile("fragmentT1.h");
//			_shaderProgram.reset(new ShaderProgram(openGLContext, vertexFile.getFullPathName(), fragmentFile.getFullPathName()));
//
//			_sprite.init();
//		}
//		void shutdown() override
//		{
//			_shaderProgram->stopThread(1000);
//			_shaderProgram.reset();
//		}
//		void render() override
//		{
//			if (!OpenGLHelpers::isContextActive())
//				return;
//
//			if (_shaderProgram->updateShader() >= 0)
//			{
//				const MessageManagerLock mmLock;
//				if (mmLock.lockWasGained())
//				{
//					_lblCompileInfo->setText(_shaderProgram->getCompileResult() + " \nlearnopengl.com/Getting-started/Hello-Triangle", NotificationType::dontSendNotification);
//				}
//			}
//
//			OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
//			glClear(GL_COLOR_BUFFER_BIT);
//
//			if (_shaderProgram->_shader)
//			{
//				_shaderProgram->_shader->use();
//				_sprite.draw();
//			}
//		}
//
//		//==============================================================================
//		void paint(Graphics& g) override
//		{
//
//		}
//		void resized() override
//		{
//			auto r = getLocalBounds();
//			if (_lblCompileInfo)
//			{
//				_lblCompileInfo->setBounds(r.removeFromTop(proportionOfHeight(0.05000f)));
//			}
//		}
//
//	private:
//		//==============================================================================
//		// Your private member variables go here...
//		Camera _camera;
//		Sprite _sprite;
//		std::unique_ptr<ShaderProgram> _shaderProgram;
//		std::unique_ptr<Label> _lblCompileInfo{ nullptr };
//		bool init{ false };
//
//		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial15)
//	};
//
//}
//
// 
//
//
// 
