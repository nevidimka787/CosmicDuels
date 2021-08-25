#include "Texture.h"

Texture2D::Texture2D() :
    id(0)
{

}

bool Texture2D::Initialisate(const char* texture_file_name, GLint format, GLint internal_format, GLint wrap_s_rule, GLint wrap_t_rule, GLint min_filter_parameter, GLint mag_filter_parameter)
{
    int width;
    int height;
    int nr_chanels_count;
    stbi_uc* data = stbi_load(texture_file_name, &width, &height, &nr_chanels_count, 0);

    bool succses_flag = true;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_rule);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_rule);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_parameter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_parameter);

    if (data != nullptr)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::TEXTURE2D::TEXTURE_INITIALISATION::File can't opened." << std::endl
            << "File: " << texture_file_name << std::endl;
        succses_flag = false;
    }
    stbi_image_free((void*)data);

    return succses_flag;
}

void Texture2D::Use()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id);
}