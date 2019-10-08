#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SpriteBase.h"
#include "ShaderProgram.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ValueTreeView.h>

namespace T14 {
	const String tutorialLink = "learnopengl.com/Lighting/Light-casters";

	const String lightVertexFilename = "T14ColorsVertex.h";
	const String lightFragmentFileName = "T14ColorsFragment.h";

	const String lampVertexFilename = "T14LampVertex.h";
	const String lampFragmentFileName = "T14LampFragment.h";


	struct UniformsCube : public UniformsBase
	{
	public:
		UniformsCube(OpenGLContext& openGLContext, OpenGLShaderProgram& shader) : UniformsBase(openGLContext, shader)
		{
			viewPos = createUniform(openGLContext, shader, "viewPos");

			material_diffuse = createUniform(openGLContext, shader, "material_diffuse");
			material_specular = createUniform(openGLContext, shader, "material_specular");
			material_shininess = createUniform(openGLContext, shader, "material_shininess");


			light_position = createUniform(openGLContext, shader, "light_position");
			light_direction = createUniform(openGLContext, shader, "light_direction");
			light_cutOff = createUniform(openGLContext, shader, "light_cutOff");
			light_outerCutOff = createUniform(openGLContext, shader, "light_outerCutOff");


			light_ambient = createUniform(openGLContext, shader, "light_ambient");
			light_diffuse = createUniform(openGLContext, shader, "light_diffuse");
			light_specular = createUniform(openGLContext, shader, "light_specular");

			light_constant = createUniform(openGLContext, shader, "light_constant");
			light_linear = createUniform(openGLContext, shader, "light_linear");
			light_quadratic = createUniform(openGLContext, shader, "light_quadratic");
		}

		ScopedPointer<OpenGLShaderProgram::Uniform> 
			viewPos{ nullptr },

			material_diffuse{ nullptr }, material_specular{ nullptr }, material_shininess{ nullptr },

			light_position{ nullptr }, light_direction{ nullptr }, light_cutOff{ nullptr }, light_outerCutOff{ nullptr },
			
			light_ambient{ nullptr }, light_diffuse{ nullptr }, light_specular{ nullptr },

			light_constant{ nullptr }, light_linear{ nullptr }, light_quadratic{ nullptr };
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


	class SpriteCube : public SpriteBase
	{
	public:
		SpriteCube(OpenGLContext& openGLContext, Camera& camera, glm::mat4 model = glm::mat4(1.0f)) : SpriteBase(openGLContext, camera, model)
		{
		}

		virtual void initPre() override
		{
			
				_pTextureDiffuseMap = TextureCache::getTexture(diffuseMapPath);
				_pTextureSpecularMap = TextureCache::getTexture(specularMapPath);

				if (!_pTextureDiffuseMap || !_pTextureSpecularMap)
				{
					jassertfalse;
					return;
				}
		}
		
		virtual void initBuffer() override
		{
			

			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);


			_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
			_openGLContext.extensions.glEnableVertexAttribArray(1);

			_openGLContext.extensions.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
			_openGLContext.extensions.glEnableVertexAttribArray(2);
		
		}

		virtual UniformsBase * getUniformBase()
		{
			return (UniformsBase*)_uniformCube.get();
		}

		void setUniformEnv(OpenGLContext& ogc, OpenGLShaderProgram *shader)
		{
			_uniformCube.reset(new UniformsCube(ogc, *shader));
		}

		

		virtual void drawPost()
		{
			// positions all containers
			static glm::vec3 cubePositions[] = {
										glm::vec3(0.0f,  0.0f,  0.0f),
										glm::vec3(2.0f,  5.0f, -15.0f),
										glm::vec3(-1.5f, -2.2f, -2.5f),
										glm::vec3(-3.8f, -2.0f, -12.3f),
										glm::vec3(2.4f, -0.4f, -3.5f),
										glm::vec3(-1.7f,  3.0f, -7.5f),
										glm::vec3(1.3f, -2.0f, -2.5f),
										glm::vec3(1.5f,  2.0f, -2.5f),
										glm::vec3(1.5f,  0.2f, -1.5f),
										glm::vec3(-1.3f,  1.0f, -1.5f)
			};
 
				_openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
				if (_pTextureDiffuseMap)
					_pTextureDiffuseMap->bind();
				else return;

				_openGLContext.extensions.glActiveTexture(GL_TEXTURE1);
				if (_pTextureSpecularMap)
					_pTextureSpecularMap->bind();
				else return;

				if (_uniformCube->material_diffuse)
					_uniformCube->material_diffuse->set(0);
				if (_uniformCube->material_specular)
					_uniformCube->material_specular->set(1);
		 


			_openGLContext.extensions.glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

 
			static float sa = 0.005;
			for (unsigned int i = 0; i < 10; i++)
			{
				// calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				angle = angle + sa;
				sa += 0.01;
				//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				if (_uniformCube->model)
				{
					_uniformCube->model->setMatrix4(glm::value_ptr(model), 1, GL_FALSE);
				}
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			return;
		}
 
	public:

		OpenGLTexture* _pTextureDiffuseMap{ nullptr }, *_pTextureSpecularMap{ nullptr };

		String diffuseMapPath;
		String specularMapPath;

		std::unique_ptr<UniformsCube> _uniformCube;
	};




	class SpriteLamp : public SpriteBase
	{
	public:
		SpriteLamp(OpenGLContext& openGLContext, Camera& camera, glm::mat4 model = glm::mat4(1.0f)) :
			SpriteBase(openGLContext, camera, model)
		{
		}


		virtual void initBuffer() override
		{
			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);


			_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
			_openGLContext.extensions.glEnableVertexAttribArray(1);

			_openGLContext.extensions.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
			_openGLContext.extensions.glEnableVertexAttribArray(2);

		}


		virtual UniformsBase * getUniformBase()
		{
			return (UniformsBase*)_uniformLamp.get();
		}

		void setUniformEnv(OpenGLContext& ogc, OpenGLShaderProgram *shader)
		{
			_uniformLamp.reset(new UniformsBase(ogc, *shader));
		}

		virtual void drawPost() override
		{

			glDrawArrays(GL_TRIANGLES, 0, 36);


		}


		std::unique_ptr<UniformsBase> _uniformLamp;
	};

	class Tutorial14 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener,
		public juce::ValueTree::Listener
	{
	public:

		//==============================================================================
		Tutorial14() :
			_spriteLight(openGLContext, _camera, glm::mat4(1.0f)),
			_spriteLamp(openGLContext, _camera),
			_tree("Settings")
		{
			setWantsKeyboardFocus(true);


			_spriteLight._model = glm::rotate(glm::mat4(1.0f), 0.4f, glm::vec3(0.0f, 0.3f, 0.0f));


			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
			openGLContext.setComponentPaintingEnabled(true);

			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
			addAndMakeVisible(_lblCompileInfo.get());

			_camera.ProcessKeyboard(Camera_Movement::RIGHT, .45f);
			_camera.ProcessKeyboard(Camera_Movement::BACKWARD, .3f);



			valueTreeDebugger.reset(new ValueTreeDebugger(_tree));
			ValueTree click("Point_Light");
			_tree.addChild(click, -1, nullptr);
			click.setProperty("Constant", _Constant, nullptr);
			click.setProperty("Linear", _Linear, nullptr);
			click.setProperty("Quadratic", _Quadratic, nullptr);

			click.setProperty("CutOff", _CutOff, nullptr);
			click.setProperty("OuterCutOff", _OuterCutOff, nullptr);
			
			_tree.addListener(this);



			setSize(800, 600);
		}
		~Tutorial14()
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


			_spriteLight.diffuseMapPath = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("container2.png").getFullPathName();
			_spriteLight.specularMapPath = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("container2_specular.png").getFullPathName();

			
			
			_spriteLight.init();
			_spriteLamp.init();

		}
		void shutdown() override
		{
			_shaderProgramLight->stopThread(1000);
			_shaderProgramLight.reset();

			TextureCache::clear();
		}

		int updateShader(ShaderProgram* shaderProg)
		{
			auto res = shaderProg->updateShader();
			return res;
		}
		void render() override
		{
			if (!OpenGLHelpers::isContextActive())
				return;


			bool NeedUpdate = false;
			String compilerInof = "";
			int res = -1;
			res = updateShader(_shaderProgramLight.get());
	
			if (res == 1)
			{
				_spriteLight.setUniformEnv(openGLContext, _shaderProgramLight->_shader);
			}
			else if (res == 0)
			{
				NeedUpdate = true;
				compilerInof = _shaderProgramLight->getCompileResult() + "\n";
			}


			res = updateShader(_shaderProgramLamp.get());

			if (res == 1)
			{
				_spriteLamp.setUniformEnv(openGLContext, _shaderProgramLamp->_shader);
			}
			else if (res == 0)
			{
				NeedUpdate = true;
				compilerInof += _shaderProgramLamp->getCompileResult();
			}

			if (NeedUpdate)
			{
				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
				{
					_lblCompileInfo->setText(compilerInof + " \n" + tutorialLink, NotificationType::dontSendNotification);
				}
			}



			OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);


			/*static float inc = 0.0;
			inc += 0.0005;
			 lightPos.x = 2 * sin(inc);
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

				if (_spriteLight._uniformCube)
				{
					if (_spriteLight._uniformCube->light_position)
					{
						_spriteLight._uniformCube->light_position->set(lightPos.x, lightPos.y, lightPos.z);
					}

					if (_spriteLight._uniformCube->light_direction)
					{
						_spriteLight._uniformCube->light_direction->set(_camera.getCameraFront().x, _camera.getCameraFront().y, _camera.getCameraFront().z);
					}

					if (_spriteLight._uniformCube->light_cutOff)
					{
						_spriteLight._uniformCube->light_cutOff->set(glm::cos(glm::radians(_CutOff)));
					}
					
					if (_spriteLight._uniformCube->light_outerCutOff)
					{
						_spriteLight._uniformCube->light_outerCutOff->set(glm::cos(glm::radians(_OuterCutOff)));
					}


					if (_spriteLight._uniformCube->viewPos)
					{
						auto cp = _camera.getCameraPos();
						_spriteLight._uniformCube->viewPos->set(cp.x, cp.y, cp.z);
					}

					if (_spriteLight._uniformCube->material_shininess)
					{
						_spriteLight._uniformCube->material_shininess->set(64.0f);
					}


					if (_spriteLight._uniformCube->light_ambient)
					{
						_spriteLight._uniformCube->light_ambient->set(0.2f, 0.2f, 0.2f);
					}
					if (_spriteLight._uniformCube->light_diffuse)
					{
						_spriteLight._uniformCube->light_diffuse->set(0.5f, 0.5f, 0.5f);
					}
					if (_spriteLight._uniformCube->light_specular)
					{
						_spriteLight._uniformCube->light_specular->set(1.0f, 1.0f, 1.0f);
					}


					if (_spriteLight._uniformCube->light_constant)
					{
						_spriteLight._uniformCube->light_constant->set(_Constant);
					}

					if (_spriteLight._uniformCube->light_linear)
					{
						_spriteLight._uniformCube->light_linear->set(_Linear);
					}

					if (_spriteLight._uniformCube->light_quadratic)
					{
						_spriteLight._uniformCube->light_quadratic->set(_Quadratic);
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


		//value tree section 

		void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &_property) override
		{

	
			if (_property.toString() == "Constant")
			{
				_Constant = treeWhosePropertyHasChanged.getProperty("Constant").toString().getFloatValue();
			}
			else if (_property.toString() == "Linear")
			{
				_Linear = treeWhosePropertyHasChanged.getProperty("Linear").toString().getFloatValue();

			}
			else if (_property.toString() == "Quadratic")
			{
				_Quadratic = treeWhosePropertyHasChanged.getProperty("Quadratic").toString().getFloatValue();
			}
			else if (_property.toString() == "CutOff")
			{
				_CutOff = treeWhosePropertyHasChanged.getProperty("CutOff").toString().getFloatValue();
			}
			else if (_property.toString() == "OuterCutOff")
			{
				_OuterCutOff = treeWhosePropertyHasChanged.getProperty("OuterCutOff").toString().getFloatValue();
			}

		
		}
		void valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override {}
		void valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {}
		void valueTreeChildOrderChanged(juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
		void valueTreeParentChanged(juce::ValueTree &treeWhoseParentHasChanged) override {}
		void valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged) override {}


	private:
		//==============================================================================
	 // Your private member variables go here...
		Camera _camera;
		SpriteCube _spriteLight;
		SpriteLamp _spriteLamp;

		std::unique_ptr<ShaderProgram> _shaderProgramLight;
		std::unique_ptr<ShaderProgram> _shaderProgramLamp;
		std::unique_ptr<Label> _lblCompileInfo{ nullptr };
		bool init{ false };


		glm::vec3 lightPos{ 2.f, 0.0f, 2.0f };

	 
		float _Constant{ 1.0f };
		float _Linear{ 0.09f };
		float _Quadratic{ 0.032f };
		
		float _CutOff{ 12.5f };
		float _OuterCutOff{17.5f };

		ValueTree _tree;
		std::unique_ptr<ValueTreeDebugger> valueTreeDebugger;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial14)
	};
}