#pragma once
#include "Program.h"

class TextureLoader
{
private:
	int imageWidth, imageHeight, imageComponents;
	unsigned char* imageData;
	GLuint textureID;

	// Sprite sheet parameters
	int frameIndex = 0, frameCount, SpriteSheetRow;
	bool isSpriteSheet = false;
	float previousTime = 0.0f;
	float frameRate = 16.0f; // How many frame per second

public:
	TextureLoader();
	~TextureLoader();

	// Load texture from file
	void LoadTexture(const char* _filename, bool _flipY = true);
	// Set texture parameters
	void SetTextureParameters(GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT, 
		GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR);
	// Set sprite sheet parameters
	void SetSpriteSheetParameters(int _frameCount, int _spriteSheetRow = 1)
	{
		frameCount = _frameCount;
		SpriteSheetRow = _spriteSheetRow;
		isSpriteSheet = true;
	}
	// Animate sprite sheet
	void AnimateSpriteSheet(float _currentTime);


	// Getters functions
	int GetImageWidth() const { return imageWidth; }
	int GetImageHeight() const { return imageHeight; }
	int GetImageComponents() const { return imageComponents; }
	unsigned char* GetImageData() const { return imageData; }
	GLuint GetTextureID() const { return textureID; }
	int GetFrameIndex() const { return frameIndex; }
	int GetFrameCount() const { return frameCount; }
	int GetSpriteSheetRow() const { return SpriteSheetRow; }
};

