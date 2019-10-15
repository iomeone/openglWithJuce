/*
  ==============================================================================

    Tutorial19.cpp
    Created: 15 Oct 2019 9:50:16am
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




namespace T19 {
	const String tutorialLink = "learnopengl.com/Advanced-OpenGL/Cubemaps";
	const String vertexCubeFilename = "T19CubeVertex.h";
	const String fragmentCubeFileName = "T19CubeFragment.h";

	const String vertexSkyboxFilename = "T19SkyboxVertex.h";
	const String fragmentSkyboxFilename = "T19SkyboxFragment.h";





	struct UniformsCube : public UniformsBase
	{
	public:
		UniformsCube(OpenGLContext& openGLContext, OpenGLShaderProgram& shader) : UniformsBase(openGLContext, shader)
		{
			texture1 = createUniform(openGLContext, shader, "texture1");
		}

		ScopedPointer<OpenGLShaderProgram::Uniform> texture1{ nullptr };

	};


	struct UniformSkybox : public UniformsBase
	{
	public:
		UniformSkybox(OpenGLContext& openGLContext, OpenGLShaderProgram& shader) : UniformsBase(openGLContext, shader)
		{
			skybox = createUniform(openGLContext, shader, "skybox");
		}

		ScopedPointer<OpenGLShaderProgram::Uniform> skybox{ nullptr };

	};

	class SpriteCube : public SpriteBase
	{
	public:
		SpriteCube(OpenGLContext& openglContext, Camera& camera) : SpriteBase(openglContext, camera)
		{

		}

		virtual void setupTexture() override
		{
			_pTextureCube = TextureCache::getTexture(_texutureCubePath);
			if (!_pTextureCube)
				jassertfalse;
		}


		virtual void initBuffer() override
		{

#pragma pack(1)
			struct Vertex {
				struct Position {
					float x;
					float y;
					float z;
				};

				struct UV {
					float u;
					float v;
				};

				Position position;

				UV uv;

				Vertex(float x, float y, float z, float u, float v)
				{
					position.x = x;
					position.y = y;
					position.z = z;

					uv.u = u;
					uv.v = v;
				}
			};
#pragma pack()

			Vertex vertices[] = {
				// positions         // texture coords
				{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f},
				{0.5f, -0.5f, -0.5f,  1.0f,  0.0f},
				{0.5f,  0.5f, -0.5f,  1.0f,  1.0f},
				{0.5f,  0.5f, -0.5f,  1.0f,  1.0f},
				{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f},
				{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f},

				{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f},
				{0.5f, -0.5f,  0.5f,  1.0f,  0.0f},
				{0.5f,  0.5f,  0.5f,  1.0f,  1.0f},
				{0.5f,  0.5f,  0.5f,  1.0f,  1.0f},
				{-0.5f,  0.5f,  0.5f,  0.0f,  1.0f},
				{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f},

				{-0.5f,  0.5f,  0.5f,  1.0f,  0.0f},
				{-0.5f,  0.5f, -0.5f,  1.0f,  1.0f},
				{-0.5f, -0.5f, -0.5f,  0.0f,  1.0f},
				{-0.5f, -0.5f, -0.5f,  0.0f,  1.0f},
				{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f},
				{-0.5f,  0.5f,  0.5f,  1.0f,  0.0f},

				{0.5f,  0.5f,  0.5f,  1.0f,  0.0f},
				{0.5f,  0.5f, -0.5f,  1.0f,  1.0f},
				{0.5f, -0.5f, -0.5f,  0.0f,  1.0f},
				{0.5f, -0.5f, -0.5f,  0.0f,  1.0f},
				{0.5f, -0.5f,  0.5f,  0.0f,  0.0f},
				{0.5f,  0.5f,  0.5f,  1.0f,  0.0f},

				{-0.5f, -0.5f, -0.5f,  0.0f,  1.0f},
				{0.5f, -0.5f, -0.5f,  1.0f,  1.0f},
				{0.5f, -0.5f,  0.5f,  1.0f,  0.0f},
				{0.5f, -0.5f,  0.5f,  1.0f,  0.0f},
				{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f},
				{-0.5f, -0.5f, -0.5f,  0.0f,  1.0f},

				{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f},
				{0.5f,  0.5f, -0.5f,  1.0f,  1.0f},
				{0.5f,  0.5f,  0.5f,  1.0f,  0.0f},
				{0.5f,  0.5f,  0.5f,  1.0f,  0.0f},
				{-0.5f,  0.5f,  0.5f,  0.0f,  0.0f},
				{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f}

			};

			_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);

			// uv attribute
			_openGLContext.extensions.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
			_openGLContext.extensions.glEnableVertexAttribArray(1);
		}

		virtual void bindTexture() override
		{
			_openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
			if (_pTextureCube)
				_pTextureCube->bind();
			else
				jassertfalse;

			jassert(_uniformCube);

			if (_uniformCube->texture1)
				_uniformCube->texture1->set(0);

		}
		virtual void drawPost() override
		{
			glDrawArrays(GL_TRIANGLES, 0, 36);
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

		OpenGLTextureEx* _pTextureCube{ nullptr };
		String _texutureCubePath;
		std::unique_ptr<UniformsCube> _uniformCube{ nullptr };
	};

	class SpriteSkybox : public SpriteBase
	{
	public:
		SpriteSkybox(OpenGLContext& openglContext, Camera& camera) : SpriteBase(openglContext, camera)
		{

		}
		virtual void setupTexture() override
		{
			_pTextureSkybox = TextureCache::getCubeTexture(_textureSkyboxPath, _textureSkyboxId);
			if (!_pTextureSkybox)
				jassertfalse;
		}
		virtual void initBuffer() override
		{

#pragma pack(1)
			struct VertexSkyBox {
				struct Position {
					float x;
					float y;
					float z;
				};

				Position position;

				VertexSkyBox(float x, float y, float z)
				{
					position.x = x;
					position.y = y;
					position.z = z;
				}
			};
#pragma pack()
			VertexSkyBox vertices[] = {
									// positions          
							{-1.0f,  1.0f, -1.0f},
							{-1.0f, -1.0f, -1.0f },
							 {1.0f, -1.0f, -1.0f},
							 {1.0f, -1.0f, -1.0f},
							 {1.0f,  1.0f, -1.0f},
							{-1.0f,  1.0f, -1.0f},

							{-1.0f, -1.0f,  1.0f},
							{-1.0f, -1.0f, -1.0f},
							{-1.0f,  1.0f, -1.0f},
							{-1.0f,  1.0f, -1.0f},
							{-1.0f,  1.0f,  1.0f},
							{-1.0f, -1.0f,  1.0f},

							 {1.0f, -1.0f, -1.0f},
							 {1.0f, -1.0f,  1.0f},
							 {1.0f,  1.0f,  1.0f},
							 {1.0f,  1.0f,  1.0f},
							 {1.0f,  1.0f, -1.0f},
							 {1.0f, -1.0f, -1.0f},

							{-1.0f, -1.0f,  1.0f},
							{-1.0f,  1.0f,  1.0f},
							{ 1.0f,  1.0f,  1.0f},
							{ 1.0f,  1.0f,  1.0f},
							{ 1.0f, -1.0f,  1.0f},
							{-1.0f, -1.0f,  1.0f},

							{-1.0f,  1.0f, -1.0f},
							{ 1.0f,  1.0f, -1.0f},
							{ 1.0f,  1.0f,  1.0f},
							{ 1.0f,  1.0f,  1.0f},
							{-1.0f,  1.0f,  1.0f},
							{-1.0f,  1.0f, -1.0f},

							{-1.0f, -1.0f, -1.0f},
							{-1.0f, -1.0f,  1.0f},
							{ 1.0f, -1.0f, -1.0f},
							{ 1.0f, -1.0f, -1.0f},
							{-1.0f, -1.0f,  1.0f},
							{ 1.0f, -1.0f,  1.0f}
			};

			_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSkyBox), (GLvoid*)offsetof(VertexSkyBox, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);

		}

		virtual void bindTexture() override
		{
			_openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
			if (_pTextureSkybox)
				_pTextureSkybox->bindCube();
			else
				jassertfalse;

			jassert(_uniformSkybox);

			if (_uniformSkybox->skybox)
				_uniformSkybox->skybox->set(0);

		}

		void setUniformEnv(OpenGLShaderProgram *shader)
		{
			_uniformSkybox.reset(new UniformSkybox(_openGLContext, *shader));
		}


		virtual void drawPost() override
		{
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		virtual UniformsBase * getUniformBase() override
		{
			jassert(_uniformSkybox);  // you need call setUniformEnv
			return  _uniformSkybox.get();
		}
	public:
		OpenGLTextureEx*  _pTextureSkybox{ nullptr };  // we should only can delete this in  opengl shutdown function when opengl context is active.  so we delete it by TextureCache::clear(); method
		 
		std::vector<String> _textureSkyboxPath;
		String _textureSkyboxId{ "Skybox" };

		std::unique_ptr<UniformSkybox> _uniformSkybox{ nullptr };

	};


	class Tutorial19 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{
	public:
		//==============================================================================
		Tutorial19() : _spriteCube(openGLContext, _camera)/* : _sprite(openGLContext, _camera, _useCircle)*/
			, _spriteSkybox(openGLContext, _camera)

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
		~Tutorial19()
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


			auto vertexScreenFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexSkyboxFilename);
			auto fragmentScreenFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentSkyboxFilename);
			_shaderScreen.reset(new ShaderProgram(openGLContext, vertexScreenFile.getFullPathName(), fragmentScreenFile.getFullPathName()));


			_spriteCube._texutureCubePath = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("marble.jpg").getFullPathName();
		
			_spriteSkybox._textureSkyboxPath.push_back(f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("skybox").getChildFile("right.jpg").getFullPathName());
			_spriteSkybox._textureSkyboxPath.push_back(f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("skybox").getChildFile("left.jpg").getFullPathName());
			_spriteSkybox._textureSkyboxPath.push_back(f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("skybox").getChildFile("top.jpg").getFullPathName());
			_spriteSkybox._textureSkyboxPath.push_back(f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("skybox").getChildFile("bottom.jpg").getFullPathName());
			_spriteSkybox._textureSkyboxPath.push_back(f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("skybox").getChildFile("front.jpg").getFullPathName());
			_spriteSkybox._textureSkyboxPath.push_back(f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("skybox").getChildFile("back.jpg").getFullPathName());

			jassert(_spriteSkybox._textureSkyboxPath.size() == 6);

			_spriteCube.init();
			_spriteSkybox.init();

		}
		void shutdown() override
		{
 
			_shaderProgramScene->stopThread(1000);
			_shaderProgramScene.reset();

			_shaderScreen->stopThread(1000);
			_shaderScreen.reset();

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

			res = _shaderScreen->updateShader();
			if (res >= 0)
			{
				if (res == 1)
				{
					_spriteSkybox.setUniformEnv(_shaderScreen->_shader);   // cube and plan use the same shader
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderScreen->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);
			}



			if (_shaderProgramScene->_shader && _shaderScreen->_shader)
			{
				glEnable(GL_DEPTH_TEST);
				OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				_shaderProgramScene->_shader->use();
				_spriteCube.draw();


				glDepthFunc(GL_LEQUAL);
				_shaderScreen->_shader->use();
				_spriteSkybox.draw();
				glDepthFunc(GL_LESS);
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
		std::unique_ptr<ShaderProgram> _shaderScreen;

		SpriteCube _spriteCube;
		SpriteSkybox _spriteSkybox;


		std::unique_ptr<Label> _lblCompileInfo{ nullptr };


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial19)
	};
}

static ComponentList<T19::Tutorial19> t19((const String)("19: Cubemaps"), 18);
