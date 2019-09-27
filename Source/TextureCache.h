/*
  ==============================================================================

    TextureCache.h
    Created: 27 Sep 2019 9:20:07am
    Author:  user

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
//This caches the textures so that multiple sprites can use the same textures
class TextureCache
{
public:
	TextureCache(){}
	~TextureCache()
	{
	}
	static void clear()
	{
		std::map<String, OpenGLTexture* >& _textureMap = TextureCache::getTextureMap();
		for (auto i : _textureMap)
		{
			delete i.second;
		}
		_textureMap.clear();
			
	}
	static  OpenGLTexture* getTexture(String texturePath)
	{
		//lookup the texture and see if its in the map
		auto mit = getTextureMap().find(texturePath);

		//check if its not in the map
		if (mit == getTextureMap().end()) {
			File f = texturePath;
			
			if (!f.existsAsFile())
			{
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Error Texture Path", texturePath);
				return nullptr;
			}
			else
			{
				std::map<String, OpenGLTexture* >& _textureMap = TextureCache::getTextureMap();
				auto image = juce::ImageFileFormat::loadFrom(f);
				OpenGLTexture* pTexture = new OpenGLTexture();
				pTexture->loadImage(image);
				_textureMap[texturePath] = pTexture;
				return pTexture;
			}
		}
		
		return mit->second;
	}

public:

	static std::map<String, OpenGLTexture* > & getTextureMap()
	{
		static std::map<String, OpenGLTexture* > _textureMap;
		return _textureMap;
	}
};