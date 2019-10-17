/*
  ==============================================================================

    Tutorial24.cpp
    Created: 17 Oct 2019 3:50:01pm
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

unsigned int amount = 100000;

namespace T24 {
	const String tutorialLink = "learnopengl.com/Advanced-OpenGL/Instancing";
	const String vertexAstoridilename = "T24AstoridVertex.h";
	const String fragmentAstoridFileName = "T24AstoridFragment.h";

	const String vertexPlanetFilename = "T24PlanetVertex.h";
	const String fragmentPlanetFileName = "T24PlanetFragment.h";


	class Tutorial24 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{

	public:
		struct OffsetOfAstoriod
		{
			float angle;
			float displamentx;
			float displamenty;
			float displamentz;
			float scale;
		};

		OffsetOfAstoriod*  _OffsetOfAstoriodS{ nullptr };
	public:
		//==============================================================================
		Tutorial24() : _asteroidModel(openGLContext, _camera),
					   _planetModel(openGLContext, _camera),
			_instanceVBO(openGLContext)

		{
			openGLContext.setPixelFormat(OpenGLPixelFormat(8, 8, 16, 8));
			openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
			openGLContext.setComponentPaintingEnabled(true);

			setWantsKeyboardFocus(true);

			_lblCompileInfo.reset(new Label("compileInfo", "Shader source build info."));
			addAndMakeVisible(_lblCompileInfo.get());

			_camera.ProcessKeyboard(Camera_Movement::BACKWARD, 105.0f);
			setSize(800, 600);
		}
		~Tutorial24()
		{
			shutdownOpenGL();
		}

		//==============================================================================
		void initialise() override
		{
			File f = File::getCurrentWorkingDirectory();
			macPath(f);


			//load the model shader and lamp shader.
			auto vertexAstoridFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexAstoridilename);
			auto fragmentAstoridFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentAstoridFileName);
			_shaderProgramAstorid.reset(new ShaderProgram(openGLContext, vertexAstoridFile.getFullPathName(), fragmentAstoridFile.getFullPathName()));


			auto vertexPlanetFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexPlanetFilename);
			auto fragmentPlanetFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentPlanetFileName);
			_shaderProgramPlanet.reset(new ShaderProgram(openGLContext, vertexPlanetFile.getFullPathName(), fragmentPlanetFile.getFullPathName()));


			// load the rock model rock/rock.obj
			auto objFile = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("rock").getChildFile("rock.obj");
			jassert(objFile.existsAsFile());
			_asteroidModel.load(objFile.getFullPathName().toStdString());
			_asteroidModel.init();
		








			 
			modelMatrices = new glm::mat4[amount];
	 
			_OffsetOfAstoriodS = new OffsetOfAstoriod[amount];
		
			
			float offset = 25.0f;
			for (unsigned int i = 0; i < amount; i++)
			{
				
				// 1. translation: displace along circle with 'radius' in range [-offset, offset]
				float angle = (float)i / (float)amount * 360.0f;
				float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				_OffsetOfAstoriodS[i].displamentx = displacement;
				//float x = sin(angle) * radius + displacement;
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset - 100.f;
				_OffsetOfAstoriodS[i].displamenty = displacement;
				//float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				_OffsetOfAstoriodS[i].displamentz = displacement;
				//float z = cos(angle) * radius + displacement ;
				//model = glm::translate(model, glm::vec3(x, y, z));

				// 2. scale: Scale between 0.05 and 0.25f
				float scale = (rand() % 20) / 100.0f + 0.05;
				//model = glm::scale(model, glm::vec3(scale));

				_OffsetOfAstoriodS[i].scale = scale;
				//_OffsetOfAstoriodS[i].displamentx = displacement;
				// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
				//float rotAngle = (rand() % 360);
				//model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

				//// 4. now add to list of matrices
				//modelMatrices[i] = model;
				//modelMatrices_notRotated[i] = model;
			}

			// configure instanced array
			// -------------------------
			_instanceVBO.init();
			_instanceVBO.BufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
			

			for (unsigned int i = 0; i < _asteroidModel.meshes.size(); i++)
			{
				unsigned int VAO = _asteroidModel.meshes[i].VAO;
				
				openGLContext.extensions.glBindVertexArray(VAO);
				_instanceVBO.bind(); // could write out of the loop ?
				
				// set attribute pointers for matrix (4 times vec4)
				openGLContext.extensions.glEnableVertexAttribArray(5);
				openGLContext.extensions.glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
				openGLContext.extensions.glEnableVertexAttribArray(6);
				openGLContext.extensions.glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
				openGLContext.extensions.glEnableVertexAttribArray(7);
				openGLContext.extensions.glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
				openGLContext.extensions.glEnableVertexAttribArray(8);
				openGLContext.extensions.glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

				typedef void(*glVertexAttribDivisor_type)(GLuint index, GLuint divisor);
				static glVertexAttribDivisor_type pglVertexAttribDivisor = nullptr;
				if (!pglVertexAttribDivisor)
					pglVertexAttribDivisor = (glVertexAttribDivisor_type)OpenGLHelpers::getExtensionFunction("glVertexAttribDivisor");
				if (!pglVertexAttribDivisor)
					jassertfalse;

				pglVertexAttribDivisor(5, 1);
				pglVertexAttribDivisor(6, 1);
				pglVertexAttribDivisor(7, 1);
				pglVertexAttribDivisor(8, 1);

				_instanceVBO.unbind(); // need not?
				openGLContext.extensions.glBindVertexArray(0);
			}
	
 

			// load the planet model planet/planet.obj
			objFile = f.getParentDirectory().getParentDirectory().getChildFile("Resource").getChildFile("planet").getChildFile("planet.obj");
			jassert(objFile.existsAsFile());
			_planetModel.load(objFile.getFullPathName().toStdString());
			_planetModel.init();
			_planetModel.setFarPlane(1000.0f);
			auto m = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -38.f, 0.f));
			_planetModel.setModel(m);

		}

		void shutdown() override
		{
			_shaderProgramAstorid->stopThread(1000);
			_shaderProgramAstorid.reset();

			_shaderProgramPlanet->stopThread(1000);
			_shaderProgramPlanet.reset();

			TextureCache::clear();
		}
		void updatePos()
		{
	

			//float radius = 150.0;;
			//float offset = 25.0f;
			//for (unsigned int i = 0; i < amount; i++)
			//{
			//	glm::mat4 model = glm::mat4(1.0f);
			//	// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			//	float angle = (float)i / (float)amount * 360.0f;
			//	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			//	float x = sin(angle) * radius + displacement;
			//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset - 100.f;
			//	float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
			//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			//	float z = cos(angle) * radius + displacement;
			//	model = glm::translate(model, glm::vec3(x, y, z));

			//	// 2. scale: Scale between 0.05 and 0.25f
			//	float scale = (rand() % 20) / 100.0f + 0.05;
			//	model = glm::scale(model, glm::vec3(scale));

			//	// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			//	float rotAngle = (rand() % 360);
			//	model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			//	// 4. now add to list of matrices
			//	modelMatrices[i] = model;
			//}
			static float radius = 150.0;;
			static float inc = 0.1;
			inc += 0.001;
			for (unsigned int i = 0; i < amount; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				float angle = (float)i / (float)amount * 360.0f;

				float x = sin(angle+inc) * radius + _OffsetOfAstoriodS[i].displamentx;
				float y = _OffsetOfAstoriodS[i].displamenty * 0.4f;
				float z = cos(angle+inc) * radius + _OffsetOfAstoriodS[i].displamentz;
				


				model = glm::translate(model, glm::vec3(x, y, z));
				model = glm::scale(model, glm::vec3(_OffsetOfAstoriodS[i].scale));

				model = glm::rotate(model, inc*100.f, glm::vec3(0.4f, 0.6f, 0.8f));

	

				modelMatrices[i] = model;
			}


			_instanceVBO.BufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
		}
		void render() override
		{
			jassert(OpenGLHelpers::isContextActive());

			// we compile the model shader.
			auto res = _shaderProgramAstorid->updateShader();
			if (res >= 0)
			{
				if (res == 1)
				{
					_asteroidModel.setUniformEnv(_shaderProgramAstorid->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderProgramAstorid->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}


			res = _shaderProgramPlanet->updateShader();
			if (res >= 0)
			{
				if (res == 1)
				{
					_planetModel.setUniformEnv(_shaderProgramPlanet->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderProgramPlanet->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (_shaderProgramAstorid->_shader)
			{
				_shaderProgramAstorid->_shader->use();
				//_asteroidModel.Draw(_camera.getCameraPos(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

				for (unsigned int i = 0; i < _asteroidModel.meshes.size(); i++)
				{

					if (_asteroidModel.meshes[i].getUniformBase()->view)
					{
						glm::mat4 view = _camera.getViewMat();
						_asteroidModel.meshes[i].getUniformBase()->view->setMatrix4(glm::value_ptr(view), 1, GL_FALSE);
					}

					if (_asteroidModel.meshes[i].getUniformBase()->projection)
					{
						glm::mat4 projection;
						projection = glm::perspective(_camera.getZoom(), float(getBounds().getWidth()) / float(getBounds().getHeight()), 0.1f, 1000.0f);
						_asteroidModel.meshes[i].getUniformBase()->projection->setMatrix4(glm::value_ptr(projection), 1, GL_FALSE);
					}
 
					updatePos();
					_asteroidModel.meshes[i].bindTexture();

					openGLContext.extensions.glBindVertexArray(_asteroidModel.meshes[i].VAO);

					typedef void(*glDrawElementsInstanced_type)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount);
					static glDrawElementsInstanced_type pglDrawElementsInstanced = nullptr;
					if (!pglDrawElementsInstanced)
						pglDrawElementsInstanced = (glDrawElementsInstanced_type)OpenGLHelpers::getExtensionFunction("glDrawElementsInstanced");
					if (!pglDrawElementsInstanced)
						jassertfalse;

					pglDrawElementsInstanced(GL_TRIANGLES, _asteroidModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);

					openGLContext.extensions.glBindVertexArray(0);
				}

			}
			else
				jassertfalse;



			if (_shaderProgramPlanet->_shader)
			{
				_shaderProgramPlanet->_shader->use();
				_planetModel.Draw(_camera.getCameraPos(), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else
				jassertfalse;

			//glEnable(GL_BLEND);
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
		std::unique_ptr<ShaderProgram> _shaderProgramAstorid;
		std::unique_ptr<ShaderProgram> _shaderProgramPlanet;


		Model _asteroidModel;
		Model _planetModel;

		OpenglBuffer _instanceVBO;
		glm::mat4* modelMatrices{ nullptr };
	 



		std::unique_ptr<Label> _lblCompileInfo{ nullptr };


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial24)
	};
}



#ifdef  _WIN64
static ComponentList<T24::Tutorial24> t24((const String)("24: Asteroid field"), 23);
#else
static ComponentList<T24::Component> t24((const String)("24: Asteroid field"), 23);
#endif 