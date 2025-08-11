#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
}

void TextureLoader::LoadTexture(const char* _filename, bool _flipY)
{
	// Set if the image needs to be flipped vertically on load
    stbi_set_flip_vertically_on_load(_flipY);

	// Load the image data from the specified file path
    imageData = stbi_load(_filename, &imageWidth, &imageHeight, &imageComponents, 0);
    
	// Check if the image data was loaded successfully
    if (!imageData) {
        std::cerr << "Failed to load texture image!" << std::endl;
    }
    
    // Create and bind a texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Check how many components the image has (RGBA or RGB)
    GLint loadedComponents = (imageComponents == 4) ? GL_RGBA : GL_RGB;
    // Populate the texture with the image data
    glTexImage2D(   GL_TEXTURE_2D, 0, loadedComponents, imageWidth, imageHeight, 0,
                    loadedComponents, GL_UNSIGNED_BYTE, imageData);

    // Generate the mipmaps, free the memory, and unbind the texture
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureLoader::SetTextureParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter)
{
    // Bind the texture to set its parameters
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    
    // Set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    // Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureLoader::AnimateSpriteSheet(float _currentTime)
{
    if(isSpriteSheet)
    {
        float deltaTime = _currentTime - previousTime;
        float frameTime = 1.0f / frameRate; // Calculate the time per frame based on the frame rate
        if (isSpriteSheet && deltaTime > frameTime) // Adjust the frame rate as needed
        {
            frameIndex = (frameIndex + 1) % frameCount; // Loop through frames
            previousTime = _currentTime;
        }
    }
}
