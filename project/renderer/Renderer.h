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

    glm::vec4 m_vColor = glm::vec4(1.0, 1.0f, 1.0f, 1.0f);

public:
    Renderer();

    const unsigned int SCREEN_WIDTH = 1920;
    const unsigned int SCREEN_HEIGHT = 1080;

    GLFWwindow* GetWindow(){ return window; }
    bool InitSystem();
    void CreateAndFill2DBuffers();
    void CreateAndFill3DBuffers();
    void CallEventsAndSwapBuffers();

    void SetColor(float fRed = 1.0f, float fGreen = 1.0f, float fBlue = 1.0f, float fAlpha = 1.0f);

    /*2D*/
    void DrawPicture(Texture& texture, float fX, float fY, Shader& shaderTexture);
    void DrawPictureScaled(Texture& texture, float fX, float fY, float fScaleFactor, Shader& shaderTexture);
    void DrawPictureRotated(Texture& texture, float fX, float fY, float fDegrees, Shader& shaderTexture);
    void DrawPictureAroundPoint(Texture& texture, float fX, float fY, float fDegrees, float fVertPivotFromCenter, Shader& shaderTexture);

    /*3D*/
    void DrawPicture3D(Texture& texture, Shader& shaderTexture, glm::mat4& matrixModel, glm::mat4& matrixView);

    /*Debug Draw*/
    void DrawPictureDebug(Texture &texture, float fX, float fY, float fScaleFactor, float fDegrees, float fVertPivotFromCenter, Shader &shaderTexture);
};

extern Renderer gRenderer;
#endif
