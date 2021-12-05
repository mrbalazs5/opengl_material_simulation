#include "Texture.h"

Texture::Texture(unsigned int width, unsigned int height) {
    float textureBorderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    glGenTextures(1, (GLuint*) &this->id);

    glBindTexture(GL_TEXTURE_2D, id[0]);

    glTexImage2D(
        GL_TEXTURE_2D,
        0, 
        GL_RGBA, 
        width, 
        height, 
        0, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        NULL
    );

    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);
}