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

			setWantsKeyboardFocus(true);

			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
			addAndMakeVisible(_lblCompileInfo.get());

			_camera.ProcessKeyboard(Camera_Movement::BACKWARD, 10.0);
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
					_model.setUniformEnv(_shaderProgram->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
				{
					_lblCompileInfo->setText(_shaderProgram->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);
				}
			}
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			


			if (_shaderProgram->_shader)
			{
				_shaderProgram->_shader->use();
				_model.Draw();
			}
			glEnable(GL_BLEND);
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
 
		Model _model;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial15)
	};
}
