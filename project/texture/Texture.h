#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include "../gl_includes/glad/glad.h"
#include <GLFW/glfw3.h>

class Texture
{
private:

    GLuint m_Texture;
    int m_nWidth = 0;
    int m_nHeight = 0;
    int m_nNumberOfChannels = 0;
    std::string m_sCurrentPath = "";

public:
    Texture();
    bool LoadFromPath(std::string sPath);
    GLuint GetTexture(){ return m_Texture; }
    int& GetWidth(){ return m_nWidth; }
    int& GetHeight(){ return m_nHeight; }
    std::string GetPath(){ return m_sCurrentPath; }
    void Bind();
    void Unbind();
    void DeleteTexture();

};

#endif
