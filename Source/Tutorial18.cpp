/*
  ==============================================================================

    Tutorial18.cpp
    Created: 13 Oct 2019 5:04:27pm
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




namespace T18 {
    const String tutorialLink = "learnopengl.com/Advanced-OpenGL/Framebuffers";
    const String vertexSceneFilename = "T18PlaneAndBoxVertex.h";
    const String fragmentSceneFileName = "T18PlaneAndBoxFragment.h";
    
    const String vertexScreenFilename = "T18ScreenVertex.h";
    const String fragmentScreenFilename = "T18ScreenFragment.h";



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


    struct UniformsCubeAndPlan : public UniformsBase
    {
    public:
        UniformsCubeAndPlan(OpenGLContext& openGLContext, OpenGLShaderProgram& shader) : UniformsBase(openGLContext, shader)
        {
            texture1 = createUniform(openGLContext, shader, "texture1");
        }

        ScopedPointer<OpenGLShaderProgram::Uniform> texture1{ nullptr };

    };


	struct UniformsScreen : public UniformsBase
	{
	public:
		UniformsScreen(OpenGLContext& openGLContext, OpenGLShaderProgram& shader) : UniformsBase(openGLContext, shader)
		{
			screenTexture = createUniform(openGLContext, shader, "screenTexture");
		}

		ScopedPointer<OpenGLShaderProgram::Uniform> screenTexture{ nullptr };

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

			jassert(_uniformBase);
			UniformsCubeAndPlan * ucp = dynamic_cast<UniformsCubeAndPlan*>(_uniformBase);
			if (ucp)
			{
				if (ucp->texture1)
					ucp->texture1->set(0);
			}


			UniformsScreen * us = dynamic_cast<UniformsScreen*>(_uniformBase);
			if (us)
			{
				if (us->screenTexture)
					us->screenTexture->set(0);
			}


        }
        virtual void drawPost() override
        {
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

		void  setUniformBase(UniformsBase* ub)
		{
			_uniformBase = ub;
		}

        virtual UniformsBase * getUniformBase() override
        {
            jassert(_uniformBase);  // you need call setUniformEnv
            return  _uniformBase;
        }

        void setUniformScene(OpenGLShaderProgram *shader)
        {
            _uniformCube.reset(new UniformsCubeAndPlan(_openGLContext, *shader));
        }
		void setUniformScreen(OpenGLShaderProgram *shader)
		{
			_uniformScreen.reset(new UniformsScreen(_openGLContext, *shader));
		}

    public:

        OpenGLTextureEx* _pTextureCube{ nullptr };
        String _texutureCubePath;


		UniformsBase* _uniformBase{ nullptr };

        std::unique_ptr<UniformsCubeAndPlan> _uniformCube{ nullptr };

		std::unique_ptr<UniformsScreen> _uniformScreen{ nullptr };
    };

    class SpritePlane : public SpriteBase
    {
    public:
        SpritePlane(OpenGLContext& openglContext, Camera& camera) : SpriteBase(openglContext, camera)
        {

        }
        virtual void setupTexture() override
        {
            _pTextureFloor = TextureCache::getTexture(_textureFloorPath);
            if (!_pTextureFloor)
                jassertfalse;
        }
        virtual void initBuffer() override
        {

            Vertex vertices[] = {
                // positions         // texture coords
            {    5.0f, -0.5f,  5.0f,   2.0f, 0.0f     },
            {    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f },
            {    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f },

            {    5.0f, -0.5f,  5.0f,   2.0f, 0.0f     },
            {    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f },
            {    5.0f, -0.5f, -5.0f,   2.0f, 2.0f  }

            };

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
            if (_pTextureFloor)
                _pTextureFloor->bind();
            else
                jassertfalse;

			jassert(_uniformBase);
			UniformsCubeAndPlan * ucp = dynamic_cast<UniformsCubeAndPlan*>(_uniformBase);
			if (ucp)
			{
				if (ucp->texture1)
					ucp->texture1->set(0);
			}


			UniformsScreen * us = dynamic_cast<UniformsScreen*>(_uniformBase);
			if (us)
			{
				if (us->screenTexture)
					us->screenTexture->set(0);
			}

        }

		void  setUniformBase(UniformsBase* ub)
		{
			_uniformBase = ub;
		}


		void setUniformScene(OpenGLShaderProgram *shader)
		{
			_uniformCube.reset(new UniformsCubeAndPlan(_openGLContext, *shader));
		}
		void setUniformScreen(OpenGLShaderProgram *shader)
		{
			_uniformScreen.reset(new UniformsScreen(_openGLContext, *shader));
		}


        virtual void drawPost() override
        {
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        virtual UniformsBase * getUniformBase() override
        {
			jassert(_uniformBase);  // you need call setUniformEnv
			return  _uniformBase;
        }
    public:
        OpenGLTextureEx*  _pTextureFloor{ nullptr };  // we should only can delete this in  opengl shutdown function when opengl context is active.  so we delete it by TextureCache::clear(); method
        String _textureFloorPath;



		UniformsBase* _uniformBase{ nullptr };

		std::unique_ptr<UniformsCubeAndPlan> _uniformCube{ nullptr };

		std::unique_ptr<UniformsScreen> _uniformScreen{ nullptr };

    };

	class SpriteQuard : public SpriteBase
	{
	public:
		SpriteQuard(OpenGLContext& openglContext, Camera& camera) : SpriteBase(openglContext, camera)
		{

		}

		void setTexture(GLuint frameBufferTextureID)
		{
			_frameBufferTextureID = frameBufferTextureID;
		}

		virtual void setupTexture() override
		{
			jassert(_frameBufferTextureID != 0); // you need setTexture first!
		}
		virtual void initBuffer() override
		{
			Vertex vertices[] = {
				// positions         // texture coords
			{   -1.0f,  1.0f, 0.0f,  0.0f, 1.0f},
			{   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
			{    1.0f, -1.0f, 0.0f, 1.0f, 0.0f },

			{    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f},
			{    1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
			{    1.0f,  1.0f, 0.0f, 1.0f, 1.0f }

			};

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

			jassert(_frameBufferTextureID != 0);                      // you need call setupTexture

			glBindTexture(GL_TEXTURE_2D, _frameBufferTextureID);


			UniformsScreen * us = dynamic_cast<UniformsScreen*>(_uniformBase);
			if (us)
			{
				if (us->screenTexture)
					us->screenTexture->set(0);
			}

		}

		void setUniformEnv(OpenGLShaderProgram *shader)
		{
			_uniformScreen.reset(new UniformsScreen(_openGLContext, *shader));
		}

		virtual void drawPost() override
		{
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		virtual UniformsBase * getUniformBase() override
		{
			jassert(_uniformScreen);  // you need call setUniformEnv
			return  _uniformScreen.get();
		}
	public:
		GLuint _frameBufferTextureID	{ 0 };
		String _textureFloorPath;



		UniformsBase* _uniformBase{ nullptr };

		std::unique_ptr<UniformsScreen> _uniformScreen{ nullptr };

	};


    class Tutorial18 : public OpenGLAppComponent,
        public Button::Listener,
        public Slider::Listener
    {
    public:
        //==============================================================================
        Tutorial18() : _spriteCube(openGLContext, _camera)/* : _sprite(openGLContext, _camera, _useCircle)*/
            , _spritePlane(openGLContext, _camera)
			, _spriteQuard(openGLContext, _camera)
        
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
        ~Tutorial18()
        {
            shutdownOpenGL();
        }

        //==============================================================================
        void initialise() override
        {
            File f = File::getCurrentWorkingDirectory();
            macPath(f);
            
            _frameBuffer.initialise(openGLContext, 800, 600, true, true);
			_spriteQuard.setTexture(_frameBuffer.getTextureID());

            //load the model shader and lamp shader.
            auto vertexSceneFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexSceneFilename);
            auto fragmentSceneFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentSceneFileName);
            _shaderProgramScene.reset(new ShaderProgram(openGLContext, vertexSceneFile.getFullPathName(), fragmentSceneFile.getFullPathName()));


			auto vertexScreenFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexScreenFilename);
			auto fragmentScreenFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentScreenFilename);
			_shaderScreen.reset(new ShaderProgram(openGLContext, vertexScreenFile.getFullPathName(), fragmentScreenFile.getFullPathName()));
		

            _spriteCube._texutureCubePath = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("marble.jpg").getFullPathName();
            _spritePlane._textureFloorPath = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("metal.png").getFullPathName();


            _spriteCube.init();
            _spritePlane.init();
			_spriteQuard.init();
		
        }
        void shutdown() override
        {
			_frameBuffer.release();
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
                    _spriteCube.setUniformScene(_shaderProgramScene->_shader);
                    _spritePlane.setUniformScene(_shaderProgramScene->_shader);   // cube and plan use the same shader!
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
					//_spriteCube.setUniformScreen(_shaderProgramScene->_shader);     // nerver used.  (it's uncommon an sprite uses different shaders)
					//_spritePlane.setUniformScreen(_shaderProgramScene->_shader);   // nerver used.  (it's uncommon an sprite uses different shaders)
					_spriteQuard.setUniformEnv(_shaderScreen->_shader);
				}


				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderScreen->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}


			
            if (_shaderProgramScene->_shader && _shaderScreen->_shader )
            {
				_frameBuffer.makeCurrentRenderingTarget();
				glEnable(GL_DEPTH_TEST);
                OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                _shaderProgramScene->_shader->use();

				_spritePlane.setUniformBase(_spritePlane._uniformCube.get());
                _spritePlane.draw();
 
				_spriteCube.setUniformBase(_spritePlane._uniformCube.get());
                _spriteCube.draw();






				_frameBuffer.releaseAsRenderingTarget();
				glDisable(GL_DEPTH_TEST);
				// clear all relevant buffers
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
				glClear(GL_COLOR_BUFFER_BIT);


				_shaderScreen->_shader->use();
				_spriteQuard.draw();

				//_spritePlane.setUniformBase(_spritePlane._uniformScreen.get());
				//_spritePlane.draw();

				//_spriteCube.setUniformBase(_spritePlane._uniformScreen.get());
				//_spriteCube.draw();

				
            }
        }

        //==============================================================================
        void paint(Graphics& g) override{}
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
        SpritePlane _spritePlane;
		SpriteQuard _spriteQuard;
        
        OpenGLFrameBuffer _frameBuffer;

        std::unique_ptr<Label> _lblCompileInfo{ nullptr };


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial18)
    };
}

static ComponentList<T18::Tutorial18> t18((const String)("18: Framebuffers"), 17);
