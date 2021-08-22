#include "Texture.h"

Texture2D::Texture2D(const char* texture_name)
{
    int width;
    int height;
    int nr_chanels_count;
    stbi_uc* data = stbi_load(texture_name, &width, &height, &nr_chanels_count, 0);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data != nullptr)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::TEXTURE2D::TEXTURE_INITIALISATION::File can't opened." << std::endl
            << "File: " << texture_name << std::endl;
    }
    stbi_image_free((void*)data);
}

void Texture2D::Use()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture2D::~Texture2D()
{

}