/*
  ==============================================================================

    OpenGLTextureEx.h
    Created: 10 Oct 2019 7:11:43pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


	class  OpenGLTextureEx
	{
	public:
		OpenGLTextureEx();
		~OpenGLTextureEx();

		/** Creates a texture from the given image.

			Note that if the image's dimensions aren't a power-of-two, the texture may
			be created with a larger size.

			The image will be arranged so that its top-left corner is at texture
			coordinate (0, 1).
		*/
		void loadImage(const Image& image);


		void loadCubeImage(const Image& image, const int index);

		/** Creates a texture from a raw array of pixels.
			If width and height are not powers-of-two, the texture will be created with a
			larger size, and only the subsection (0, 0, width, height) will be initialised.
			The data is sent directly to the OpenGL driver without being flipped vertically,
			so the first pixel will be mapped onto texture coordinate (0, 0).
		*/
		void loadARGB(const PixelARGB* pixels, int width, int height);

		/** Creates a texture from a raw array of pixels.
			This is like loadARGB, but will vertically flip the data so that the first
			pixel ends up at texture coordinate (0, 1), and if the width and height are
			not powers-of-two, it will compensate by using a larger texture size.
		*/
		void loadARGBFlipped(const PixelARGB* pixels, int width, int height);

		/** Creates an alpha-channel texture from an array of alpha values.
			If width and height are not powers-of-two, the texture will be created with a
			larger size, and only the subsection (0, 0, width, height) will be initialised.
			The data is sent directly to the OpenGL driver without being flipped vertically,
			so the first pixel will be mapped onto texture coordinate (0, 0).
		*/
		void loadAlpha(const uint8* pixels, int width, int height);

		/** Frees the texture, if there is one. */
		void release();

		/** Binds the texture to the currently active openGL context. */
		void bind() const;

		void bindCube() const;

		/** Unbinds the texture to the currently active openGL context. */
		void unbind() const;

		/** Returns the GL texture ID number. */
		GLuint getTextureID() const noexcept { return textureID; }

		int getWidth() const noexcept { return width; }
		int getHeight() const noexcept { return height; }

		/** Returns true if a texture can be created with the given size.
			Some systems may require that the sizes are powers-of-two.
		*/
		static bool isValidSize(int width, int height);

	private:
		GLuint textureID;
		int width, height;
		OpenGLContext* ownerContext;

		void create(int w, int h, const void*, GLenum, bool topLeft);
		void createCube(const int w, const int h, const int index, const void* pixels, GLenum type, bool topLeft);
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLTextureEx)
	};
