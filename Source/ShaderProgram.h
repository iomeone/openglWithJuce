/*
  ==============================================================================

    ShaderProgram.h
    Created: 26 Sep 2019 11:51:46am
    Author:  user

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace OpenGL33Helpers
{
   
	inline String GetVersionString()
	{
		return String("#version 330 core");
	}
    /*
     This is modified version of juce::OpenGLHelpers::translateVertexShaderToV3.
     */
    inline String translateVertexShaderToV3 (const String& code)
    {
#if JUCE_OPENGL3
        if (OpenGLShaderProgram::getLanguageVersion() > 1.2)
        {
            String output;
            
            int numAttributes = 0;
            int last = 0;
            
            for (int p = code.indexOf (0, "attribute "); p >= 0; p = code.indexOf (p + 1, "attribute "))
            {
                output += code.substring (last, p) + "layout(location=" + String (numAttributes++) + ") in ";
                last = p + 10;
            }
            
            output += code.substring (last);
            
            return GetVersionString() + "\n" + output.replace ("varying", "out");
        }
#endif
        
        return code;
    }
    
    /*
     This is modified version of juce::OpenGLHelpers::translateVertexShaderToV3.
     */
	inline String translateFragmentShaderToV3 (const String& code)
    {
#if JUCE_OPENGL3
        if (OpenGLShaderProgram::getLanguageVersion() > 1.2)
            return  GetVersionString() + "\n"
                   "out " JUCE_MEDIUMP " vec4 fragColor;\n"
                    + code.replace ("varying", "in")
                          .replace ("texture2D", "texture")
                          .replace ("gl_FragColor", "fragColor");
#endif
        
        return code;
    }
};

class ShaderProgram :public Thread
{
public:
	ShaderProgram(OpenGLContext & ogc, String vertexFilePath, String fragmentFilePath , String geometryFilePath = "")
		: Thread("Shader Thread"), _openGLContext(ogc)
	{
		if (geometryFilePath != "")
		{
			_wantUseGeometry = true;
		}

		_fVertex = vertexFilePath;
		_fFragment = fragmentFilePath;
		_fGeometry = geometryFilePath;
		
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

			if (_wantUseGeometry)
			{
				if (_fGeometry.existsAsFile())
				{

					lastModiyGeometry = _fGeometry.getLastModificationTime();
					juce::StringArray sa;
					_fGeometry.readLines(sa);
					_strGeometry = sa.joinIntoString("\n");
					_oldGeometry = _strGeometry;

				}
				else
				{
					AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "error", "Please create Geometry shader source file at: " + geometryFilePath);
					bfind = false;
				}

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

			if(_wantUseGeometry)
			{
				juce::Time tGeometry = _fGeometry.getLastModificationTime();

				if (lastModiyGeometry != tGeometry)
				{
					lastModiyFragment = tGeometry;
					juce::StringArray sa;
					_fGeometry.readLines(sa);

					_strGeometry = sa.joinIntoString("\n");
					_oldGeometry = _strFragment;
				}
			}

			if (!_strFragment.isEmpty() || !_strVertex.isEmpty() || !_strGeometry.isEmpty())
			{
				if (_strFragment.isEmpty())
				{
					_strFragment = _oldFragment;
				}
				if (_strVertex.isEmpty())
				{
					_strVertex = _oldVertex;
				}
				if (_strGeometry.isEmpty())
				{
					_strGeometry = _oldGeometry;
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
		if (_wantUseGeometry && _strGeometry.length() == 0)
			return -1;

		
		ScopedPointer<OpenGLShaderProgram> newShader(new OpenGLShaderProgram(_openGLContext));
		jassert(_openGLContext.isActive());
		jassert(_openGLContext.isAttached());
        
        auto translatedVertexShader = OpenGL33Helpers::translateVertexShaderToV3(_strVertex);
        auto translatedFragmentShader = OpenGL33Helpers::translateFragmentShaderToV3(_strFragment);
		auto translatedGeometryShader = OpenGL33Helpers::translateVertexShaderToV3(_strGeometry);
        
        DBG("---- Translated vertex shader -----");
        DBG(translatedVertexShader);
        
        DBG("---- Translated fragment shader -----");
        DBG(translatedFragmentShader);

		DBG("---- Translated fragment shader -----");
		DBG(translatedGeometryShader);
        
		bool bag = true;
		if (_wantUseGeometry)
		{
			bag = newShader->addGeometryShader(translatedGeometryShader);
		}
		if (newShader->addVertexShader(translatedVertexShader)
			&& newShader->addFragmentShader(translatedFragmentShader)
			&& bag
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
	juce::File _fGeometry;

	String _strVertex, _oldVertex;
	String _strFragment, _oldFragment;
	String _strGeometry, _oldGeometry;

	juce::Time lastModiyFragment;
	juce::Time lastModiyVertex;
	juce::Time lastModiyGeometry;
 
	bool _wantUseGeometry{ false };
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
