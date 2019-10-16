/*
  ==============================================================================

    Tutorial22.cpp
    Created: 16 Oct 2019 3:43:30pm
    Author:  user

  ==============================================================================
*/


#include "../JuceLibraryCode/JuceHeader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CompList.h"

#include "Camera.h"

#include "ShaderProgram.h"
#include "SpriteBase.h"

#include "TextureCache.h"


#include "Model.h"



namespace T22 {
	const String tutorialLink = "learnopengl.com/Advanced-OpenGL/Geometry-Shader";
	const String vertexCubeFilename = "T22Vertex.h";
	const String fragmentCubeFileName = "T22Fragment.h";

	const String vertexNormalFilename = "T22NormalVertex.h";
	const String fragmentNormalFileName = "T22NormalFragment.h";
	const String  geometryNormalFilename = "T22NormalGeometry.h";

	class Tutorial22 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{
	public:
		//==============================================================================
		Tutorial22() : _model(openGLContext, _camera)

		{
			openGLContext.setPixelFormat(OpenGLPixelFormat(8, 8, 16, 8));
			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
			openGLContext.setComponentPaintingEnabled(true);

			setWantsKeyboardFocus(true);

			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
			addAndMakeVisible(_lblCompileInfo.get());

			_camera.ProcessKeyboard(Camera_Movement::BACKWARD, 5.0f);
			setSize(800, 600);
		}
		~Tutorial22()
		{
			shutdownOpenGL();
		}

		//==============================================================================
		void initialise() override
		{
			File f = File::getCurrentWorkingDirectory();
			macPath(f);


			//load the model shader and lamp shader.
			auto vertexSceneFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexCubeFilename);
			auto fragmentSceneFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentCubeFileName);
			_shaderProgramScene.reset(new ShaderProgram(openGLContext, vertexSceneFile.getFullPathName(), fragmentSceneFile.getFullPathName()));

			

			auto vertexNormalFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexNormalFilename);
			auto fragmentNormalFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentNormalFileName);
			auto geometryNormalFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(geometryNormalFilename);

			_shaderProgramNormal.reset(new ShaderProgram(openGLContext, vertexNormalFile.getFullPathName(), fragmentNormalFile.getFullPathName(), geometryNormalFile.getFullPathName()));

			

			// load the nanosuit model
			auto objFile = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("nanosuit").getChildFile("nanosuit.obj");
			jassert(objFile.existsAsFile());
			_model.load(objFile.getFullPathName().toStdString());
			_model.init();
			auto m = glm::translate(glm::mat4(1.0f), glm::vec3(5.f, -8.f, 0.f));
			_model.setModel(m);
	 

		}
		void shutdown() override
		{
			_shaderProgramScene->stopThread(1000);
			_shaderProgramScene.reset();

			_shaderProgramNormal->stopThread(1000);
			_shaderProgramNormal.reset();
		
			TextureCache::clear();
		}
		void render() override
		{
			jassert(OpenGLHelpers::isContextActive());

			// we compile the model shader.
			auto res = _shaderProgramScene->updateShader();
			if (res >= 0)
			{
				if (res == 1)
				{
					//_model.setUniformEnv(_shaderProgramScene->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderProgramScene->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}


			res = _shaderProgramNormal->updateShader();
			if (res >= 0)
			{
				if (res == 1)
				{
					//_model.setUniformEnv(_shaderProgramScene->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderProgramNormal->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			if (_shaderProgramScene->_shader)
			{
				OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				_shaderProgramScene->_shader->use();
				_model.setUniformEnv(_shaderProgramScene->_shader);
				_model.Draw(_camera.getCameraPos(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
				jassertfalse;
		


			if (_shaderProgramNormal->_shader)
			{
				_shaderProgramNormal->_shader->use();
				_model.setUniformEnv(_shaderProgramNormal->_shader);
				_model.Draw(_camera.getCameraPos(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
				jassertfalse;

			glEnable(GL_BLEND);
		}

		//==============================================================================
		void paint(Graphics& g) override {}
		void resized() override
		{
			auto r = getLocalBounds();
			if (_lblCompileInfo)
			{
				_lblCompileInfo->setBounds(r.removeFromTop(proportionOfHeight(0.05000f)));
			}
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
		}

	private:
		//==============================================================================
		// Your private member variables go here...
		Camera _camera;
		std::unique_ptr<ShaderProgram> _shaderProgramScene;
		std::unique_ptr<ShaderProgram> _shaderProgramNormal;
		

		Model _model;

		std::unique_ptr<Label> _lblCompileInfo{ nullptr };


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial22)
	};
}



#ifdef  _WIN64
static ComponentList<T22::Tutorial22> t22((const String)("22: Visualizing normal vectors"), 21);
#else
static ComponentList<T22::Component> t22((const String)("22: Visualizing normal vectors"), 21);
#endif 