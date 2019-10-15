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
#include "OpenGLTextureEx.h"
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
		std::map<String, OpenGLTextureEx* >& _textureMap = TextureCache::getTextureMap();
		for (auto i : _textureMap)
		{
			delete i.second;
		}
		_textureMap.clear();
			
	}
	static  OpenGLTextureEx* getTexture(String texturePath)
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
				std::map<String, OpenGLTextureEx* >& _textureMap = TextureCache::getTextureMap();
				auto image = juce::ImageFileFormat::loadFrom(f);
				OpenGLTextureEx* pTexture = new OpenGLTextureEx();
				pTexture->loadImage(image);
				_textureMap[texturePath] = pTexture;
				return pTexture;
			}
		}
		
		return mit->second;
	}

	static  OpenGLTextureEx* getCubeTexture(std::vector<String> faces, String cubeMapName)
	{
		//lookup the texture and see if its in the map
		auto mit = getTextureMap().find(cubeMapName);

		//check if its not in the map
		if (mit == getTextureMap().end()) 
		{
			std::map<String, OpenGLTextureEx* >& _textureMap = TextureCache::getTextureMap();
			OpenGLTextureEx* pTexture = new OpenGLTextureEx();
			int index = 0;
			for (auto texturePath : faces)
			{
				File f = texturePath;

				if (!f.existsAsFile())
				{
					AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Error Texture Path", texturePath);
					return nullptr;
				}
				else
				{
					auto image = juce::ImageFileFormat::loadFrom(f);
					pTexture->loadCubeImage(image, index);
					index++;
				}
			}
			_textureMap[cubeMapName] = pTexture;
			return pTexture;

		}

		return mit->second;
	}

public:

	static std::map<String, OpenGLTextureEx* > & getTextureMap()
	{
		static std::map<String, OpenGLTextureEx* > _textureMap;
		return _textureMap;
	}
};