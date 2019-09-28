/*
  ==============================================================================

    ShaderProgram.h
    Created: 26 Sep 2019 11:51:46am
    Author:  user

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class ShaderProgram :public Thread
{
public:
	ShaderProgram(OpenGLContext & ogc, String vertexFilePath, String fragmentFilePath)
		: Thread("Shader Thread"), _openGLContext(ogc)
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
			_isReadingFile = true;
		 
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
				
			}
			_isReadingFile = false;
		}
	}


	int updateShader()
	{
		bool result = -1;
		if (_isReadingFile.get())
		{
			return -1;
		}
		if (_strFragment.length() == 0 || _strVertex.length() == 0)
			return -1;
		
		ScopedPointer<OpenGLShaderProgram> newShader(new OpenGLShaderProgram(_openGLContext));
		jassert(_openGLContext.isActive());
		jassert(_openGLContext.isAttached());
		if (newShader->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(_strVertex))
			&& newShader->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(_strFragment))
			&& newShader->link())
		{
			_shader = nullptr;
			_shader = newShader;
			_compileResult = "GLSL: v" + String(juce::OpenGLShaderProgram::getLanguageVersion(), 2);
			result = 1;
		}
		else
		{
			result = 0;
			_compileResult = newShader->getLastError();

		}
		_strVertex = String();
		_strFragment = String();
		return result;
	}

	String getCompileResult()
	{
		return _compileResult;
	}

	ScopedPointer<OpenGLShaderProgram> _shader{ nullptr };

 

private:
	
	juce::File _fVertex;
	juce::File _fFragment;

	String _strVertex, _oldVertex;
	String _strFragment, _oldFragment;

	juce::Time lastModiyFragment;
	juce::Time lastModiyVertex;

 

	OpenGLContext & _openGLContext;

	juce::Atomic<bool> _isReadingFile{ false };

	String _compileResult;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShaderProgram)
};

inline void macPath(File & f)
{
	if ((juce::SystemStats::getOperatingSystemType()
		& juce::SystemStats::MacOSX) != 0)
	{
		f = f.getParentDirectory().getParentDirectory();
	}
}





//
//struct AttributesTutorial3
//{
//	AttributesTutorial3(OpenGLContext& openGLContext, OpenGLShaderProgram& shader) : _ogc(openGLContext)
//	{
//		aPos = createAttribute(openGLContext, shader, "aPos");
//		aColor = createAttribute(openGLContext, shader, "aColor");
//		aTexCoord = createAttribute(openGLContext, shader, "aTexCoord");
//	}
//
//	void enable()
//	{
//		if (aPos != nullptr)
//		{
//			_ogc.extensions.glEnableVertexAttribArray(aPos->attributeID);
//			_ogc.extensions.glVertexAttribPointer(aPos->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
//
//		}
//		else jassertfalse;
//
//		if (aColor != nullptr)
//		{
//			_ogc.extensions.glVertexAttribPointer(aColor->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
//			_ogc.extensions.glEnableVertexAttribArray(aColor->attributeID);
//		}
//		else jassertfalse;
//
//		if (aTexCoord != nullptr)
//		{
//			_ogc.extensions.glVertexAttribPointer(aTexCoord->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
//			_ogc.extensions.glEnableVertexAttribArray(aTexCoord->attributeID);
//		}
//		else jassertfalse;
//	}
//
//	void disable()
//	{
//		if (aPos != nullptr)       _ogc.extensions.glDisableVertexAttribArray(aPos->attributeID);
//		else jassertfalse;
//		if (aColor != nullptr)		 _ogc.extensions.glDisableVertexAttribArray(aColor->attributeID);
//		else jassertfalse;
//		if (aTexCoord != nullptr)  _ogc.extensions.glDisableVertexAttribArray(aTexCoord->attributeID);
//		else jassertfalse;
//	}
//
// 
//	ScopedPointer<OpenGLShaderProgram::Attribute> aPos, aColor, aTexCoord;
//	OpenGLContext & _ogc;
//
//private:
//	static OpenGLShaderProgram::Attribute* createAttribute(OpenGLContext& openGLContext,
//		OpenGLShaderProgram& shader,
//		const char* attributeName)
//	{
//		if (openGLContext.extensions.glGetAttribLocation(shader.getProgramID(), attributeName) < 0)
//			return nullptr;
//
//		return new juce::OpenGLShaderProgram::Attribute(shader, attributeName);
//	}
//};