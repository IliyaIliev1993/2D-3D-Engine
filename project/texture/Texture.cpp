#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../gl_includes/stb_image/stb_image.h"

Texture::Texture()
{

}

bool Texture::LoadFromPath(std::string sPath)
{
    m_sCurrentPath = sPath;

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    /*Set Texture Parameters*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*Load texture flipped*/
    stbi_set_flip_vertically_on_load(true);

    /*Load the current texture (Pixel array)*/
    unsigned char* dataTexture = stbi_load(sPath.c_str(), &m_nWidth, &m_nHeight, &m_nNumberOfChannels, 4);
    if(dataTexture)
    {
        std::cout << "Loading ... " << sPath << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataTexture);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to LOAD Texture : " << sPath << std::endl;
        return false;
    }

    m_nSourceW = m_nWidth;
    m_nSourceH = m_nHeight;

    stbi_image_free(dataTexture);

    return true;
}

void Texture::Bind()
{
    if(m_Texture)
    {
        glBindTexture(GL_TEXTURE_2D, m_Texture);
    }
    else
    {
        std::cout << "Failed to BIND Texture" << std::endl;
    }
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::DeleteTexture()
{
    if(m_Texture)
    {
        glDeleteTextures(1, &m_Texture);
        std::cout << "Deleting texture... " << m_sCurrentPath << std::endl;
        m_sCurrentPath = "";
    }
    else
    {
        std::cout << "Failed to DELETE Texture" << std::endl;
    }
}
