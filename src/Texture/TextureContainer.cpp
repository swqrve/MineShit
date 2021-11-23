#include "TextureContainer.h"

TextureContainer::TextureContainer(std::string texturePath, bool flipVertically) {
    glGenTextures(1, &this->ID);

    stbi_set_flip_vertically_on_load(flipVertically);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGBA;

        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4); 


        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        std::cout << "Texture Loaded At: " << texturePath << std::endl;
    } else {
        stbi_image_free(data);
        std::cout << "Texture failed to load at path: " << texturePath << std::endl;
    }

    glActiveTexture(GL_TEXTURE0);
}

void TextureContainer::use(unsigned int textureNumber) {
    glActiveTexture(textureNumber);
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

int TextureContainer::getID() { return this->ID; }

void TextureContainer::setImageToClamp(unsigned int textureNumber) {
    use(textureNumber);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}