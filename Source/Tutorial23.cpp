/*
  ==============================================================================

    Tutorial23.cpp
    Created: 17 Oct 2019 9:29:20am
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



namespace T23 {
	const String tutorialLink = "learnopengl.com/Advanced-OpenGL/Instancing";
	const String vertexQuadsFilename = "T23InstancingVertex.h";
	const String fragmentQuadsFileName = "T23InstancingFragment.h";

 
	class SpriteQuads : public SpriteBase
	{
	public:
		SpriteQuads(OpenGLContext& openglContext, Camera& camera) : SpriteBase(openglContext, camera)
			, _instanceVBO(openglContext)
		{

		}

		virtual void setupTexture() override
		{
		}

		virtual void initPre() override
		{
			// store instance data in an array buffer
			// --------------------------------------
			
			int index = 0;
			float offset = 0.1f;
			for (int y = -10; y < 10; y += 2)
			{
				for (int x = -10; x < 10; x += 2)
				{
					glm::vec2 translation;
					translation.x = (float)x / 10.0f + offset;
					translation.y = (float)y / 10.0f + offset;
					translations[index++] = translation;
				}
			}
			_instanceVBO.init();
			_instanceVBO.BufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
		}


		virtual void initBuffer() override
		{
			// set up vertex data (and buffer(s)) and configure vertex attributes
			// ------------------------------------------------------------------
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
				// positions     // colors
				{-0.05f,  0.05f,  1.0f, 0.0f, 0.0f},
			   {0.05f, -0.05f,  0.0f, 1.0f, 0.0f},
			  {-0.05f, -0.05f,  0.0f, 0.0f, 1.0f},

			  {-0.05f,  0.05f,  1.0f, 0.0f, 0.0f},
			  { 0.05f, -0.05f,  0.0f, 1.0f, 0.0f},
			  { 0.05f,  0.05f,  0.0f, 1.0f, 1.0f}
			};

			_openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			_openGLContext.extensions.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			_openGLContext.extensions.glEnableVertexAttribArray(0);

			// color attribute
			_openGLContext.extensions.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
			_openGLContext.extensions.glEnableVertexAttribArray(1);

			_openGLContext.extensions.glEnableVertexAttribArray(2);
			//_openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO.VBO); // this attribute comes from a different vertex buffer
			_instanceVBO.bind();
			_openGLContext.extensions.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			_instanceVBO.unbind();

			typedef void(*glVertexAttribDivisor_type)(GLuint index, GLuint divisor);
			static glVertexAttribDivisor_type pglVertexAttribDivisor = nullptr;
			if (!pglVertexAttribDivisor)
				pglVertexAttribDivisor = (glVertexAttribDivisor_type)OpenGLHelpers::getExtensionFunction("glVertexAttribDivisor");
			if (pglVertexAttribDivisor)
				pglVertexAttribDivisor(2, 1);
			else
				jassertfalse;

		}

		virtual void initPost() override
		{
			return;
			
			_instanceVBO.bind();

			_instanceVBO.unbind();
		}

		virtual void bindTexture() override{}

		virtual UniformsBase * getUniformBase() override
		{
			jassert(_defaultUniform);  // you need call setUniformEnv
			return  _defaultUniform.get();
		}

		virtual void drawPost() override
		{
			typedef void(*glDrawArraysInstanced_type)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
			static glDrawArraysInstanced_type pglDrawArraysInstanced = nullptr;
			if(!pglDrawArraysInstanced)
				pglDrawArraysInstanced = (glDrawArraysInstanced_type)OpenGLHelpers::getExtensionFunction("glDrawArraysInstanced");
			if (pglDrawArraysInstanced)
				pglDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
			else
				jassertfalse;
			//glDrawArrays(GL_POINTS, 0, 4);
		}

		glm::vec2 translations[100];;
		OpenglBuffer _instanceVBO;
	};


	class Tutorial23 : public OpenGLAppComponent,
		public Button::Listener,
		public Slider::Listener
	{
	public:
		//==============================================================================
		Tutorial23() : _spriteQuads(openGLContext, _camera)

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
		~Tutorial23()
		{
			shutdownOpenGL();
		}

		//==============================================================================
		void initialise() override
		{
			File f = File::getCurrentWorkingDirectory();
			macPath(f);


			//load the model shader and lamp shader.
			auto vertexSceneFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(vertexQuadsFilename);
			auto fragmentSceneFile = f.getParentDirectory().getParentDirectory().getChildFile("Source").getChildFile("Shader").getChildFile(fragmentQuadsFileName);
			_shaderProgramQuads.reset(new ShaderProgram(openGLContext, vertexSceneFile.getFullPathName(), fragmentSceneFile.getFullPathName()));

			
			_spriteQuads.init();
			//auto m = glm::translate(glm::mat4(1.0f), glm::vec3(5.f, -8.f, 0.f));
			//_spriteQuads.setModel(m);
	
		}
		void shutdown() override
		{
			_shaderProgramQuads->stopThread(1000);
			_shaderProgramQuads.reset();

		
			TextureCache::clear();
		}
		void render() override
		{
			jassert(OpenGLHelpers::isContextActive());

			// we compile the model shader.
			auto res = _shaderProgramQuads->updateShader();
			if (res >= 0)
			{
				if (res == 1)
				{
					_spriteQuads.setUniformEnv(_shaderProgramQuads->_shader);
				}

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
					_lblCompileInfo->setText(_shaderProgramQuads->getCompileResult() + " \n" + tutorialLink, NotificationType::dontSendNotification);

			}


			//glEnable(GL_DEPTH_TEST);
 
			if (_shaderProgramQuads->_shader)
			{
				OpenGLHelpers::clear(juce::Colour(0.2f, 0.3f, 0.3f, 1.0f));
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				_shaderProgramQuads->_shader->use();

				_spriteQuads.draw();
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
		std::unique_ptr<ShaderProgram> _shaderProgramQuads;
		SpriteQuads _spriteQuads;

		std::unique_ptr<Label> _lblCompileInfo{ nullptr };


		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tutorial23)
	};
}


 
static ComponentList<T23::Tutorial23> t23((const String)("23: instancing_quads"), 22);
 