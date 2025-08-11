#pragma once
#include "Program.h"

class TextureLoader
{
private:
	int imageWidth, imageHeight, imageComponents;
	unsigned char* imageData;
	GLuint textureID;
public:
	TextureLoader();
	~TextureLoader();

	// Load texture from file
	void LoadTexture(const char* _filename, bool _flipY = true);
	// Set texture parameters
	void SetTextureParameters(GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT, 
		GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR);

	// Getters functions
	int GetImageWidth() const { return imageWidth; }
	int GetImageHeight() const { return imageHeight; }
	int GetImageComponents() const { return imageComponents; }
	unsigned char* GetImageData() const { return imageData; }
	GLuint GetTextureID() const { return textureID; }
};

