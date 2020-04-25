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
    int m_nSourceX = 0;
    int m_nSourceY = 0;
    int m_nSourceW = 0;
    int m_nSourceH = 0;
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

    inline void SetSourceX(int nSourceX){m_nSourceX = nSourceX;}
    inline void SetSourceY(int nSourceY){m_nSourceY = nSourceY;}
    inline void SetSourceW(int nSourceW){m_nSourceW = nSourceW;}
    inline void SetSourceH(int nSourceH){m_nSourceH = nSourceH;}

    inline int GetSourceX(){return m_nSourceX;}
    inline int GetSourceY(){return m_nSourceY;}
    inline int GetSourceW(){return m_nSourceW;}
    inline int GetSourceH(){return m_nSourceH;}

    glm::vec2 g_vec2TextCoordTopLeft = glm::vec2(0.0f, 1.0f);
    glm::vec2 g_vec2TextCoordTopRight = glm::vec2(1.0f, 1.0f);
    glm::vec2 g_vec2TextCoordBottomRight = glm::vec2(1.0f, 0.0f);
    glm::vec2 g_vec2TextCoordBottomLeft = glm::vec2(0.0f, 0.0f);

};

#endif
