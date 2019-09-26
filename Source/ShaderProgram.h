/*
  ==============================================================================

    ShaderProgram.h
    Created: 26 Sep 2019 11:51:46am
    Author:  user

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"




#pragma pack(1)


struct Position {
	float x;
	float y;
};

struct ColorRGBA8 {
	ColorRGBA8() : r(0), g(0), b(0), a(0) { }
	ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
		r(R), g(G), b(B), a(A) { }
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct UV {
	float u;
	float v;
};

//The vertex definition
struct Vertex {

	Position position;

	//4 bytes for r g b a color.
	ColorRGBA8 color;

	//UV texture coordinates.
	UV uv;

	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}
};
#pragma pack()

struct Attributes
{
	Attributes(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
	{
		position = createAttribute(openGLContext, shader, "position");
		/*normal = createAttribute(openGLContext, shader, "normal");
		sourceColour = createAttribute(openGLContext, shader, "sourceColour");*/
		textureCoordIn = createAttribute(openGLContext, shader, "textureCoordIn");
	}

	void enable(OpenGLContext& openGLContext)
	{
		if (position != nullptr)
		{
			openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);
			openGLContext.extensions.glVertexAttribPointer(position->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		}

		if (textureCoordIn != nullptr)
		{
			openGLContext.extensions.glVertexAttribPointer(textureCoordIn->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 2));
			openGLContext.extensions.glEnableVertexAttribArray(textureCoordIn->attributeID);
		}
	}

	void disable(OpenGLContext& openGLContext)
	{
		if (position != nullptr)       openGLContext.extensions.glDisableVertexAttribArray(position->attributeID);
		/*	if (normal != nullptr)         openGLContext.extensions.glDisableVertexAttribArray(normal->attributeID);
		if (sourceColour != nullptr)   openGLContext.extensions.glDisableVertexAttribArray(sourceColour->attributeID);*/
		if (textureCoordIn != nullptr)  openGLContext.extensions.glDisableVertexAttribArray(textureCoordIn->attributeID);
	}

	bool getStatus()
	{
		return true;
		if (position == nullptr)
			return false;
		else
			return true;
	}
	ScopedPointer<OpenGLShaderProgram::Attribute> position, textureCoordIn;

private:
	static OpenGLShaderProgram::Attribute* createAttribute(OpenGLContext& openGLContext,
		OpenGLShaderProgram& shader,
		const char* attributeName)
	{
		if (openGLContext.extensions.glGetAttribLocation(shader.getProgramID(), attributeName) < 0)
			return nullptr;

		return new juce::OpenGLShaderProgram::Attribute(shader, attributeName);
	}
};



struct Uniforms
{
	Uniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
	{
		projectionMatrix = createUniform(openGLContext, shader, "projectionMatrix");
		viewMatrix = createUniform(openGLContext, shader, "viewMatrix");
		ProjectionView = createUniform(openGLContext, shader, "ProjectionView");
		texture = createUniform(openGLContext, shader, "Texture_1");
		texture2 = createUniform(openGLContext, shader, "Texture_2");
		texture3 = createUniform(openGLContext, shader, "Texture_3");
		texture4 = createUniform(openGLContext, shader, "Texture_4");

		lightPosition = createUniform(openGLContext, shader, "lightPosition");
		iGlobalTime = createUniform(openGLContext, shader, "iGlobalTime");

		x = createUniform(openGLContext, shader, "_x");
		y = createUniform(openGLContext, shader, "_y");
		w = createUniform(openGLContext, shader, "_w");
		h = createUniform(openGLContext, shader, "_h");

		iMouse = createUniform(openGLContext, shader, "iMouse");
		iResolution = createUniform(openGLContext, shader, "iResolution");


		winW = createUniform(openGLContext, shader, "winW");
		winH = createUniform(openGLContext, shader, "winH");

		arrFloat = createUniform(openGLContext, shader, "arrFloat");

	}
	bool getStatus()
	{
		return true;

	}
	ScopedPointer<OpenGLShaderProgram::Uniform>projectionMatrix, viewMatrix, ProjectionView, lightPosition, iGlobalTime, arrFloat, texture, texture2, texture3, texture4;
	ScopedPointer<OpenGLShaderProgram::Uniform> x, y, w, h, winW, winH, iMouse, iResolution;

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



struct DynamicTexture
{
	DynamicTexture() {  }

	Image image;


	bool applyTo(OpenGLTexture& texture, juce::String p)
	{
		const int size = 16 * 16;

		//if (!image.isValid())
		{
			File f = p;
			if (!f.existsAsFile())
			{
				jassertfalse;
				//msg("file " + p + " is not exist!");
			}
			else
				image = juce::ImageFileFormat::loadFrom(f);
		}
		texture.loadImage(image);
		return true;
	}
};

class ShaderProgram :public Thread
{
public:
	ShaderProgram(String vertexFilePath, String fragmentFilePath, Label * lblToShowCompileResult = nullptr)
		: Thread("Shader Thread")
	{
		_fVertex = vertexFilePath;
		_fFragment = fragmentFilePath;
		while (true)
		{
			bool bfind = true;
			if (_fVertex.existsAsFile())
			{

				lastModiyVertex = _fVertex.getLastModificationTime();
				juce::StringArray sa;
				_fVertex.readLines(sa);

				_strVertex = sa.joinIntoString("\n");

				_oldVertex = _strVertex;

			}
			else
			{
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "error", "Please create vertex shader source file at: " +  vertexFilePath);
				bfind = false;
			}


			if (_fFragment.existsAsFile())
			{

				lastModiyFragment = _fFragment.getLastModificationTime();
				juce::StringArray sa;
				_fFragment.readLines(sa);
				_strFragment = sa.joinIntoString("\n");
				_oldFragment = _strFragment;

			}
			else
			{
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "error", "Please create fragment shader source file at: " + vertexFilePath);
				bfind = false;
			}
				
			if (bfind)
				break;
		}

		if (lblToShowCompileResult)
			_lblshader.reset(lblToShowCompileResult);
		startThread();
	}

	~ShaderProgram() override
	{
		// allow the thread 2 seconds to stop cleanly - should be plenty of time.
		stopThread(2000);
	}

	void run() override
	{
	 
		while (!threadShouldExit())
		{
			wait(500);

		 
			{
				juce::Time tVertex = _fVertex.getLastModificationTime();

				if (lastModiyVertex != tVertex)
				{
					lastModiyVertex = tVertex;
					juce::StringArray sa;
					_fVertex.readLines(sa);

					_strVertex = sa.joinIntoString("\n");
					_oldVertex = _strVertex;
				}
			}


			{
				juce::Time tFragment = _fFragment.getLastModificationTime();

				if (lastModiyFragment != tFragment)
				{
					lastModiyFragment = tFragment;
					juce::StringArray sa;
					_fFragment.readLines(sa);

					_strFragment = sa.joinIntoString("\n");
					_oldFragment = _strFragment;
				}
			}

			if (!_strFragment.isEmpty() || !_strVertex.isEmpty())
			{
				if (_strFragment.isEmpty())
				{
					_strFragment = _oldFragment;
				}
				if (_strVertex.isEmpty())
				{
					_strVertex = _oldVertex;
				}
				updateShader();
			}
		}
	}


	void updateShader()
	{

		String _compileResult;
		ScopedPointer<OpenGLShaderProgram> newShader(new OpenGLShaderProgram(_openGLContext));

		if (newShader->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(_strVertex))
			&& newShader->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(_strFragment))
			&& newShader->link())
		{
			_shader = nullptr;
			_shader = newShader;

			_attributes = nullptr;
			_attributes = new Attributes(_openGLContext, *_shader);

			_uniforms = nullptr;
			_uniforms = new Uniforms(_openGLContext, *_shader);

			if (_lblshader)
			{
				_compileResult = "GLSL: v" + String(juce::OpenGLShaderProgram::getLanguageVersion(), 2);

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
				{
					_lblshader->setText(_compileResult, NotificationType::dontSendNotification);
					//msg(_compileResult);
					//l->setText(_compileResult, dontSendNotification);
				}
			}
		}
		else
		{
			String s = newShader->getLastError();
			if (0)
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "error", s);

			if (_lblshader)
			{
				_compileResult = s;

				const MessageManagerLock mmLock;

				if (mmLock.lockWasGained())
					_lblshader->setText(_compileResult, NotificationType::dontSendNotification);
			}
		}
		_strVertex = String();
		_strFragment = String();
	}

private:
	
	juce::File _fVertex;
	juce::File _fFragment;

	String _strVertex, _oldVertex;
	String _strFragment, _oldFragment;

	juce::Time lastModiyFragment;
	juce::Time lastModiyVertex;

	ScopedPointer<Attributes> _attributes;
	ScopedPointer<Uniforms> _uniforms;

	ScopedPointer<OpenGLShaderProgram> _shader;
	std::unique_ptr<Label> _lblshader{ nullptr };

	OpenGLContext & _openGLContext;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShaderProgram)
};