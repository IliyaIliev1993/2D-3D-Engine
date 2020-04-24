#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include "../gl_includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../texture/Texture.h"
#include "../gl_includes/shader/shader_m.h"
#include "../gl_includes/glm/glm/glm.hpp"
#include "../gl_includes/glm/glm/gtc/matrix_transform.hpp"
#include "../gl_includes/glm/glm/gtc/type_ptr.hpp"

class Renderer
{

private:

    GLFWwindow* window = nullptr;

    GLuint VBO2D;
    GLuint VAO2D;
    GLuint EBO2D;

    GLuint VBO3D;
    GLuint VAO3D;
    GLuint EBO3D;

    GLuint VBO2DDebug;
    GLuint VAO2DDebug;
    GLuint EBO2DDebug;

    /*Used ONLY for DebugDraw*/
    /* TOP LEFT->  *---------------------------------* <-TOP RIGHT
                   |                                 |
                   |                                 |
                   |                                 |
                   |                                 |
    BOTTOM LEFT -> *---------------------------------* <-BOTTOM RIGHT */

    /*Every cicle send the coords to the video card via VBO*/
    glm::vec2 m_vec2TopRight = glm::vec2(0.0f, 0.0f);
    glm::vec2 m_vec2TopLeft = glm::vec2(0.0f, 0.0f);
    glm::vec2 m_vec2BottomLeft = glm::vec2(0.0f, 0.0f);
    glm::vec2 m_vec2BottomRight = glm::vec2(0.0f, 0.0f);

    glm::vec2 m_vec2TextCoordTopLeft = glm::vec2(0.0f, 1.0f);
    glm::vec2 m_vec2TextCoordTopRight = glm::vec2(1.0f, 1.0f);
    glm::vec2 m_vec2TextCoordBottomRight = glm::vec2(1.0f, 0.0f);
    glm::vec2 m_vec2TextCoordBottomLeft = glm::vec2(0.0f, 0.0f);

    glm::vec4 m_vColor = glm::vec4(1.0, 1.0f, 1.0f, 1.0f);

public:
    Renderer();

    static const unsigned int SCREEN_WIDTH = 1920;
    static const unsigned int SCREEN_HEIGHT = 1080;
    static const unsigned int SCREEN_DEPTH = 100;

    GLFWwindow* GetWindow(){ return window; }
    bool InitSystem();
    void CreateAndFill2DBuffers();
    void CreateAndFill3DBuffers();
    void CallEventsAndSwapBuffers();

    void SendVertsToVBODebug(glm::vec2 vec2TopLeft = glm::vec2(0.0f, 0.0f),
                             glm::vec2 vec2TopRight = glm::vec2((float)Renderer::SCREEN_WIDTH, 0.0f),
                             glm::vec2 vec2BottomRight = glm::vec2((float)Renderer::SCREEN_WIDTH, (float)Renderer::SCREEN_HEIGHT),
                             glm::vec2 vec2BottomLeft = glm::vec2(0.0f, (float)Renderer::SCREEN_HEIGHT));


    void SendTextureCoordToVBODebug(glm::vec2 vec2TextCoordTopLeft = glm::vec2(0.0f, 1.0f),
                                    glm::vec2 vec2TextCoordTopRight = glm::vec2(1.0f, 1.0f),
                                    glm::vec2 vec2TextCoordBottomRight = glm::vec2(1.0f, 0.0f),
                                    glm::vec2 vec2TextCoordBottomLeft = glm::vec2(0.0f, 0.0f));

    void SetColor(float fRed = 1.0f, float fGreen = 1.0f, float fBlue = 1.0f, float fAlpha = 1.0f);

    /*2D*/
    void DrawRect(float fX, float fY, float fWidth, float fHeight, Shader& shaderRect);
    void DrawPicture(Texture& texture, float fX, float fY, Shader& shaderTexture);
    void DrawPictureScaled(Texture& texture, float fX, float fY, float fScaleFactor, Shader& shaderTexture);
    void DrawPictureRotated(Texture& texture, float fX, float fY, float fDegrees, Shader& shaderTexture);
    void DrawPictureAroundPoint(Texture& texture, float fX, float fY, float fDegrees, float fVertPivotFromCenter, Shader& shaderTexture);

    /*3D*/
    void DrawPicture3D(Texture& texture, Shader& shaderTexture, glm::mat4& matrixModel, glm::mat4& matrixView);

    /*Debug Draw*/
    void DrawPictureDebug(Texture &texture,
                          float fX,
                          float fY,
                          float fScaleFactor,
                          float fDegrees,
                          float fVertPivotFromCenter,
                          bool bRotateAroundX,
                          bool bRotateAroundY,
                          bool bRotateAroundZ,
                          Shader &shaderTexture);
};

extern Renderer gRenderer;
#endif
