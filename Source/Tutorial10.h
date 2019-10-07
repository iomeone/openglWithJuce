#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ShaderProgram.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace T10 {
	const String tutorialLink = "learnopengl.com/Lighting/Lighting-maps";

	const String lightVertexFilename = "T10ColorsVertex.h";
	const String lightFragmentFileName = "T10ColorsFragment.h";

	const String lampVertexFilename = "T10LampVertex.h";
	const String lampFragmentFileName = "T10LampFragment.h";


	struct Uniforms
	{
		Uniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
		{
			model = createUniform(openGLContext, shader, "model");
			view = createUniform(openGLContext, shader, "view");
			projection = createUniform(openGLContext, shader, "projection");

			light_position = createUniform(openGLContext, shader, "light_position");
			viewPos = createUniform(openGLContext, shader, "viewPos");

			material_diffuse = createUniform(openGLContext, shader, "material_diffuse");
			material_specular = createUniform(openGLContext, shader, "material_specular");
			material_shininess = createUniform(openGLContext, shader, "material_shininess");

			light_ambient = createUniform(openGLContext, shader, "light_ambient");
			light_diffuse = createUniform(openGLContext, shader, "light_diffuse");
			light_specular = createUniform(openGLContext, shader, "light_specular");
		}

		ScopedPointer<OpenGLShaderProgram::Uniform>  model{ nullptr }, view{ nullptr }, projection{ nullptr }, 
			light_position{ nullptr }, viewPos{ nullptr },
            material_diffuse{ nullptr }, material_specular{ nullptr }, material_shininess{ nullptr },
			light_ambient{ nullptr }, light_diffuse{ nullptr }, light_specular{ nullptr };

        
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

		void init(bool useTexture = false , String diffuseMapPath = "", String specularMapPath = "" )
		{
            _useTexture = useTexture;
            if(_useTexture)
            {
                _pTextureDiffuseMap = TextureCache::getTexture(diffuseMapPath);
                _pTextureSpecularMap = TextureCache::getTexture(specularMapPath);
                
                if (!_pTextureDiffuseMap || !_pTextureSpecularMap)
                {
                        return;
                }
            }

            
            
			_openGLContext.extensions.glGenVertexArrays(1, &VAO);
			_openGLContext.extensions.glGenBuffers(1, &VBO);

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

			_openGLContext.extensions.glBindVertexArray(VAO);

			_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);


			_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
			_openGLContext.extensions.glEnableVertexAttribArray(1);

            _openGLContext.extensions.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
            _openGLContext.extensions.glEnableVertexAttribArray(2);
            

			_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
			_openGLContext.extensions.glBindVertexArray(0);
		}

		void setUniformEnv(OpenGLContext& ogc, OpenGLShaderProgram *shader)
		{
			_uniforms.reset(new Uniforms(ogc, *shader));
		}

		void draw()
		{
            if(_useTexture)
            {
                _openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
                if (_pTextureDiffuseMap)
                    _pTextureDiffuseMap->bind();
                else return;
                
                _openGLContext.extensions.glActiveTexture(GL_TEXTURE1);
                if (_pTextureSpecularMap)
                    _pTextureSpecularMap->bind();
                else return;
                
                if (_uniforms->material_diffuse)
                    _uniforms->material_diffuse->set(0);
                if (_uniforms->material_specular)
                    _uniforms->material_specular->set(1);
            }


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

        OpenGLTexture* _pTextureDiffuseMap{ nullptr }, *_pTextureSpecularMap{ nullptr };
        
        bool _useTexture {false};
	};



	class Tutorial10 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{
	public:
		
		//==============================================================================
		Tutorial10() :
			_spriteLight(openGLContext, _camera, glm::mat4(1.0f)),
			_spriteLamp(openGLContext, _camera)
		{
			setWantsKeyboardFocus(true);


			_spriteLight._model = glm::rotate(glm::mat4(1.0f), 0.4f, glm::vec3(0.0f, 0.3f, 0.0f));
			//_spriteLight._model = glm::translate(_spriteLight._model, glm::vec3(3.0, 0.0, 0.0));

			auto m = glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(.2f));
			_spriteLamp.setModel(m);

			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
			openGLContext.setComponentPaintingEnabled(true);

			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
			addAndMakeVisible(_lblCompileInfo.get());

            _camera.ProcessKeyboard(Camera_Movement::LEFT, .3f);
//            _camera.ProcessKeyboard(Camera_Movement::BACKWARD, 2.5f);
			setSize(800, 600);
		}
		~Tutorial10()
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


            auto textureDiffuse = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("container2.png").getFullPathName();
            auto textureSpecular = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("container2_specular.png").getFullPathName();
            
            
			_spriteLight.init(true, textureDiffuse, textureSpecular);
			_spriteLamp.init();

		}
		void shutdown() override
		{
			_shaderProgramLight->stopThread(1000);
			_shaderProgramLight.reset();
            
            TextureCache::clear();
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


			static float inc = 0.0;
			inc += 0.0005;
	/*		lightPos.x = 2 * sin(inc);
			lightPos.z =  8 * cos(inc);*/


//            glm::vec3 lightColor;
//            lightColor.x = sin(inc * 2.0f);
//            lightColor.y = sin(inc* 0.7f);
//            lightColor.z = sin(inc * 1.3f);
//
//            glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
//            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
//
//


			/*_spriteLight._model = glm::rotate(glm::mat4(1.0f), float(inc/10), glm::vec3(0.0f, 0.3f, 0.0f));*/

			if (_shaderProgramLight->_shader)
			{
				_shaderProgramLight->_shader->use();

				if (_spriteLight._uniforms)
				{
					if (_spriteLight._uniforms->light_position)
					{
						_spriteLight._uniforms->light_position->set(lightPos.x, lightPos.y, lightPos.z);
					}
					if (_spriteLight._uniforms->viewPos)
					{
						auto cp = _camera.getCameraPos();
						_spriteLight._uniforms->viewPos->set(cp.x, cp.y, cp.z);
					}

					if (_spriteLight._uniforms->material_shininess)
					{
						_spriteLight._uniforms->material_shininess->set(64.0f);
					}
				

					if (_spriteLight._uniforms->light_ambient)
					{
						_spriteLight._uniforms->light_ambient->set(0.2f, 0.2f, 0.2f);
					}
					if (_spriteLight._uniforms->light_diffuse)
					{
						_spriteLight._uniforms->light_diffuse->set(0.5f, 0.5f, 0.5f);
					}
					if (_spriteLight._uniforms->light_specular)
					{
						_spriteLight._uniforms->light_specular->set(1.0f, 1.0f, 1.0f);
					}

				}

				_spriteLight.draw();
			}


			if (_shaderProgramLamp->_shader)
			{

				auto m = glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(.4f));
				_spriteLamp.setModel(m);

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


		glm::vec3 lightPos{ 2.f, 0.0f, 2.0f };

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial10)
	};
}
