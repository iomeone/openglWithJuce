/*
  ==============================================================================

    OpenGLTextureEx.cpp
    Created: 10 Oct 2019 7:11:43pm
    Author:  user

  ==============================================================================
*/

#include "OpenGLTextureEx.h"


#define JUCE_CHECK_OPENGL_ERROR 


#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A

	static int getAllowedTextureSize(int x)
	{
#if JUCE_OPENGL_ALLOW_NON_POWER_OF_TWO_TEXTURES
		return x;
#else
		return nextPowerOfTwo(x);
#endif
	}

	OpenGLTextureEx::OpenGLTextureEx()
		: textureID(0), width(0), height(0), ownerContext(nullptr)
	{
	}

	OpenGLTextureEx::~OpenGLTextureEx()
	{
		release();
	}

	bool OpenGLTextureEx::isValidSize(int width, int height)
	{
		return isPowerOfTwo(width) && isPowerOfTwo(height);
	}

	void OpenGLTextureEx::create(const int w, const int h, const void* pixels, GLenum type, bool topLeft)
	{
		ownerContext = OpenGLContext::getCurrentContext();

		// Texture objects can only be created when the current thread has an active OpenGL
		// context. You'll need to create this object in one of the OpenGLContext's callbacks.
		jassert(ownerContext != nullptr);

		if (textureID == 0)
		{
			JUCE_CHECK_OPENGL_ERROR
				glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

#if JUCE_OPENGL3
			typedef void(*glGenerateMipmap_type)(GLuint array);
			glGenerateMipmap_type pglGenerateMipmap;
			pglGenerateMipmap = (glGenerateMipmap_type)OpenGLHelpers::getExtensionFunction("glGenerateMipmap");
			if (pglGenerateMipmap)
				pglGenerateMipmap(GL_TEXTURE_2D);
#else
#endif

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//auto glMagFilter = (ownerContext->texMagFilter == OpenGLContext::linear ? GL_LINEAR : GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			JUCE_CHECK_OPENGL_ERROR
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
			JUCE_CHECK_OPENGL_ERROR;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		JUCE_CHECK_OPENGL_ERROR

			width = getAllowedTextureSize(w);
		height = getAllowedTextureSize(h);

		const GLint internalformat = type == GL_ALPHA ? GL_ALPHA : GL_RGBA;

		if (width != w || height != h)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat,
				width, height, 0, type, GL_UNSIGNED_BYTE, nullptr);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, topLeft ? (height - h) : 0, w, h,
				type, GL_UNSIGNED_BYTE, pixels);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat,
				w, h, 0, type, GL_UNSIGNED_BYTE, pixels);
		}

		JUCE_CHECK_OPENGL_ERROR
	}

	template <class PixelType>
	struct Flipper
	{
		static void flip(HeapBlock<PixelARGB>& dataCopy, const uint8* srcData, const int lineStride,
			const int w, const int h)
		{
			dataCopy.malloc(w * h);

			for (int y = 0; y < h; ++y)
			{
				auto* src = (const PixelType*)srcData;
				auto* dst = (PixelARGB*)(dataCopy + w * (h - 1 - y));

				for (int x = 0; x < w; ++x)
					dst[x].set(src[x]);

				srcData += lineStride;
			}
		}
	};

	void OpenGLTextureEx::loadImage(const Image& image)
	{
		const int imageW = image.getWidth();
		const int imageH = image.getHeight();

		HeapBlock<PixelARGB> dataCopy;
		Image::BitmapData srcData(image, Image::BitmapData::readOnly);

		switch (srcData.pixelFormat)
		{
		case Image::ARGB:           Flipper<PixelARGB> ::flip(dataCopy, srcData.data, srcData.lineStride, imageW, imageH); break;
		case Image::RGB:            Flipper<PixelRGB>  ::flip(dataCopy, srcData.data, srcData.lineStride, imageW, imageH); break;
		case Image::SingleChannel:  Flipper<PixelAlpha>::flip(dataCopy, srcData.data, srcData.lineStride, imageW, imageH); break;
		default: break;
		}

		create(imageW, imageH, dataCopy, JUCE_RGBA_FORMAT, true);
	}


	void OpenGLTextureEx::loadCubeImage(const Image& image, const int index)
	{
		const int imageW = image.getWidth();
		const int imageH = image.getHeight();

		HeapBlock<PixelARGB> dataCopy;
		Image::BitmapData srcData(image, Image::BitmapData::readOnly);

		switch (srcData.pixelFormat)
		{
		case Image::ARGB:           Flipper<PixelARGB> ::flip(dataCopy, srcData.data, srcData.lineStride, imageW, imageH); break;
		case Image::RGB:            Flipper<PixelRGB>  ::flip(dataCopy, srcData.data, srcData.lineStride, imageW, imageH); break;
		case Image::SingleChannel:  Flipper<PixelAlpha>::flip(dataCopy, srcData.data, srcData.lineStride, imageW, imageH); break;
		default: break;
		}

		createCube(imageW, imageH, index, dataCopy, JUCE_RGBA_FORMAT, true);
	}

	void OpenGLTextureEx::createCube(const int w, const int h, int index, const void* pixels, GLenum type, bool topLeft)
	{
		ownerContext = OpenGLContext::getCurrentContext();

		// Texture objects can only be created when the current thread has an active OpenGL
		// context. You'll need to create this object in one of the OpenGLContext's callbacks.
		jassert(ownerContext != nullptr);

		if (textureID == 0)
		{
			JUCE_CHECK_OPENGL_ERROR
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//auto glMagFilter = (ownerContext->texMagFilter == OpenGLContext::linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		JUCE_CHECK_OPENGL_ERROR



			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		JUCE_CHECK_OPENGL_ERROR

		width = getAllowedTextureSize(w);
		height = getAllowedTextureSize(h);

		const GLint internalformat = type == GL_ALPHA ? GL_ALPHA : GL_RGBA;

		if (width != w || height != h)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+index, 0, internalformat,
			width, height, 0, type, GL_UNSIGNED_BYTE, nullptr);

			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, 0, topLeft ? (height - h) : 0, w, h,
				type, GL_UNSIGNED_BYTE, pixels);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, internalformat,
				w, h, 0, type, GL_UNSIGNED_BYTE, pixels);
		}

		JUCE_CHECK_OPENGL_ERROR
	}
	
	void OpenGLTextureEx::loadARGB(const PixelARGB* pixels, const int w, const int h)
	{
		create(w, h, pixels, JUCE_RGBA_FORMAT, false);
	}

	void OpenGLTextureEx::loadAlpha(const uint8* pixels, int w, int h)
	{
		create(w, h, pixels, GL_ALPHA, false);
	}

	void OpenGLTextureEx::loadARGBFlipped(const PixelARGB* pixels, int w, int h)
	{
		HeapBlock<PixelARGB> flippedCopy;
		Flipper<PixelARGB>::flip(flippedCopy, (const uint8*)pixels, 4 * w, w, h);

		create(w, h, flippedCopy, JUCE_RGBA_FORMAT, true);
	}

	void OpenGLTextureEx::release()
	{
		if (textureID != 0)
		{
			// If the texture is deleted while the owner context is not active, it's
			// impossible to delete it, so this will be a leak until the context itself
			// is deleted.
			jassert(ownerContext == OpenGLContext::getCurrentContext());

			if (ownerContext == OpenGLContext::getCurrentContext())
			{
				glDeleteTextures(1, &textureID);

				textureID = 0;
				width = 0;
				height = 0;
			}
		}
	}

	void OpenGLTextureEx::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void OpenGLTextureEx::bindCube() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	}

	void OpenGLTextureEx::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
