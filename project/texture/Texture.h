#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include "../gl_includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../gl_includes/glm/glm/glm.hpp"
#include "../gl_includes/glm/glm/gtc/matrix_transform.hpp"
#include "../gl_includes/glm/glm/gtc/type_ptr.hpp"

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

    glm::vec2 g_vecMinUV = glm::vec2(0.0f, 0.0f);
    glm::vec2 g_vecMaxUV = glm::vec2(1.0f, 1.0f);
    int g_nDynamicSourceX = 0;
    int g_nDynamicSourceY = 0;
    int g_nDynamicSourceW = 0;
    int g_nDynamicSourceH = 0;

};

#endif
