/*
  ==============================================================================

    Tutorial20.cpp
    Created: 16 Oct 2019 11:17:30am
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




namespace T20 {
	const String tutorialLink = "learnopengl.com/Advanced-OpenGL/Cubemaps";
	const String vertexCubeFilename = "T20Vertex.h";

	const String geometryCubeFilename = "T20Geometry.h";

	const String fragmentCubeFileName = "T20Fragment.h";




	struct UniformsCube : public UniformsBase
	{
	public:
		UniformsCube(OpenGLContext& openGLContext, OpenGLShaderProgram& shader) : UniformsBase(openGLContext, shader)
		{
			texture1 = createUniform(openGLContext, shader, "texture1");
		}

		ScopedPointer<OpenGLShaderProgram::Uniform> texture1{ nullptr };

	};



	class SpriteCube : public SpriteBase
	{
	public:
		SpriteCube(OpenGLContext& openglContext, Camera& camera) : SpriteBase(openglContext, camera)
		{

		}

		virtual void setupTexture() override
		{
		}


		virtual void initBuffer() override
		{

#pragma pack(1)
			struct Vertex {
				struct Position {
					float x;
					float y;
	
				};

				struct Color
				{
					float r;
					float g;
					float b;
				};

				Position position;
				Color color;

				Vertex(float x, float y, float r, float g, float b)
				{
					position.x = x;
					position.y = y;
					color.r = r;
					color.g = g;
					color.b = b;

				}
			};
#pragma pack()
			Vertex vertices[] = {
				// positions        
				 {-0.5f,  0.5f, 1.0f, 0.0f, 0.0f}, // top-left
				 {0.5f,  0.5f, 0.0f, 1.0f, 0.0f}, // top-right
				 {0.5f, -0.5f, 0.0f, 0.0f, 1.0f}, // bottom-right
				 {-0.5f, -0.5f, 1.0f, 1.0f, 0.0f}  // bottom-left
			};

			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);

			// uv attribute
			_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
			_openGLContext.extensions.glEnableVertexAttribArray(1);
		}

		virtual void bindTexture() override
		{
		}
		virtual void drawPost() override
		{
			glDrawArrays(GL_POINTS, 0, 4);
		}

		virtual UniformsBase * getUniformBase() override
		{
			jassert(_uniformCube);  // you need call setUniformEnv
			return  _uniformCube.get();
		}

		void setUniformEnv(OpenGLShaderProgram *shader)
		{
			_uniformCube.reset(new UniformsCube(_openGLContext, *shader));
		}


	public:

		std::unique_ptr<UniformsCube> _uniformCube{ nullptr };
	};



	class Tutorial20 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{
	public:
		//==============================================================================
		Tutorial20() : _spriteCube(openGLContext, _camera)

		{
			openGLContext.setPixelFormat(OpenGLPixelFormat(8, 8, 16, 8));
			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
			openGLContext.setComponentPaintingEnabled(true);

			setWantsKeyboardFocus(true);

			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
			addAndMakeVisible(_lblCompileInfo.get());

			//_camera.ProcessKeyboard(Camera_Movement::BACKWARD, 5.0f);
			setSize(800, 600);
		}
		~Tutorial20()
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
			auto geometryFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(geometryCubeFilename);


			_shaderProgramScene.reset(new ShaderProgram(openGLContext, vertexSceneFile.getFullPathName(), fragmentSceneFile.getFullPathName(), geometryFile.getFullPathName()));

			_spriteCube.init();
	 

		}
		void shutdown() override
		{
			_shaderProgramScene->stopThread(1000);
			_shaderProgramScene.reset();

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
					_spriteCube.setUniformEnv(_shaderProgramScene->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderProgramScene->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}

			if (_shaderProgramScene->_shader)
			{
				glEnable(GL_DEPTH_TEST);
				OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				_shaderProgramScene->_shader->use();
				_spriteCube.draw();
			}
			else
				jassertfalse;
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
		SpriteCube _spriteCube;
		std::unique_ptr<Label> _lblCompileInfo{ nullptr };


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial20)
	};
}

static ComponentList<T20::Tutorial20> t20((const String)("20: Geometry-Shader-Points"), 19);
