#include "Renderer.h"

Renderer::Renderer()
{

}

bool Renderer::InitSystem()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scene Creator", NULL, NULL);

    if(window == nullptr)
    {
        std::cout << "Failed to create WINDOW" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void Renderer::CreateAndFill2DBuffers()
{
    float fVertices2D[] =
    {
    /*Top Left*/ 0.0f, 0.0f,                                   /*Texture Top Left*/ 0.0f, 1.0f,
   /*Top Right*/(float)SCREEN_WIDTH, 0.0f,                    /*Texture Top Right*/ 1.0f, 1.0f,
/*Bottom Right*/(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, /*Texture Bottom Right*/ 1.0f, 0.0f,
  /*Bottom Left*/0.0f, (float)SCREEN_HEIGHT,                /*Texture Bottom Left*/ 0.0f, 0.0f,
    };

    unsigned int unIndices[] =
    {
        0, 1, 3, /*First Triangle*/
        1, 2, 3  /*Second Triangle*/
    };

    /*Creates the VertexArrayObject, VertexBufferObject, ElementBufferObject*/
    glGenVertexArrays(1, &VAO2D);
    glGenBuffers(1, &VBO2D);
    glGenBuffers(1, &EBO2D);

    /*Bind current VAO to push all the datas*/
    glBindVertexArray(VAO2D);

    /*Fill VBO*/
    glBindBuffer(GL_ARRAY_BUFFER, VBO2D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fVertices2D), fVertices2D, GL_STATIC_DRAW);

    /*Fill EBO*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2D);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unIndices), unIndices, GL_STATIC_DRAW);

    /*Set Position Attributes*/
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*Set Texture Attributes*/
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*** DEBUG BUFFERS ***/
    float fVertices2DDebug[] =
    {
        m_vec2TopLeft.x, m_vec2TopLeft.y,        /*Texture Top Left*/ 0.0f, 1.0f,
        m_vec2TopRight.x, m_vec2TopRight.y,      /*Texture Top Right*/ 1.0f, 1.0f,
        m_vec2BottomRight.x, m_vec2BottomRight.y,/*Texture Bottom Right*/ 1.0f, 0.0f,
        m_vec2BottomLeft.x, m_vec2BottomLeft.y,  /*Texture Bottom Left*/ 0.0f, 0.0f,
    };

    unsigned int unIndicesDebug[] =
    {
        0, 1, 3, /*First Triangle*/
        1, 2, 3  /*Second Triangle*/
    };

    /*Creates the VertexArrayObject, VertexBufferObject, ElementBufferObject*/
    glGenVertexArrays(1, &VAO2DDebug);
    glGenBuffers(1, &VBO2DDebug);
    glGenBuffers(1, &EBO2DDebug);

    /*Bind current VAO to push all the datas*/
    glBindVertexArray(VAO2DDebug);

    /*Fill VBO*/
    glBindBuffer(GL_ARRAY_BUFFER, VBO2DDebug);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fVertices2DDebug), fVertices2DDebug, GL_DYNAMIC_DRAW);

    /*Fill EBO*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2DDebug);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unIndicesDebug), unIndicesDebug, GL_DYNAMIC_DRAW);

    /*Set Position Attributes*/
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*Set Texture Attributes*/
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*Enable Blending*/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Renderer::CreateAndFill3DBuffers()
{
    float fVertices3D[] =
    {
    -1.0f, 1.0f, 0.0f,  /*Texture Top Left*/     0.0f, 1.0f,
     1.0f, 1.0f, 0.0f,  /*Texture Top Right*/    1.0f, 1.0f,
     1.0f,-1.0f, 0.0f,  /*Texture Bottom Right*/ 1.0f, 0.0f,
    -1.0f,-1.0f, 0.0f,  /*Texture Bottom Left*/  0.0f, 0.0f,
    };

    unsigned int unIndices3D[] =
    {
        0, 1, 3, /*First Triangle*/
        1, 2, 3  /*Second Triangle*/
    };

    /*Creates the VertexArrayObject, VertexBufferObject, ElementBufferObject*/
    glGenVertexArrays(1, &VAO3D);
    glGenBuffers(1, &VBO3D);
    glGenBuffers(1, &EBO3D);

    /*Bind current VAO to push all the datas*/
    glBindVertexArray(VAO3D);

    /*Fill VBO*/
    glBindBuffer(GL_ARRAY_BUFFER, VBO3D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fVertices3D), fVertices3D, GL_STATIC_DRAW);

    /*Fill EBO*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3D);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unIndices3D), unIndices3D, GL_STATIC_DRAW);

    /*Set Position Attributes*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*Set Texture Attributes*/
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*Enable Blending*/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Renderer::SetColor(float fRed, float fGreen, float fBlue, float fAlpha)
{
    m_vColor = glm::vec4(fRed, fGreen, fBlue, fAlpha);
}

void Renderer::DrawRect(float fX, float fY, float fWidth, float fHeight, Shader &shaderRect)
{
    /*Bind VertexArrayObject, texture and shader to use*/
    glBindVertexArray(VAO2D);
    shaderRect.use();

    /*Create transformations*/
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 projectionToPush = glm::mat4(1.0f);

    /*Transformations of Model*/
    model = glm::translate(model, glm::vec3(fX, fY, 0.0f));
    model = glm::scale(model, glm::vec3(fWidth / (float)SCREEN_WIDTH,
                                        fHeight / (float)SCREEN_HEIGHT,
                                        0.0f));

    /*Transformations of Projection, in this case is Orthographic projection*/
    projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    /*The current matrix projeciton will be pushed in the vertex shader*/
    projectionToPush = projection * view * model;

    shaderRect.setMat4("uProjection", projectionToPush);
    shaderRect.setVec4("vColor", m_vColor);

    /*Current draw call*/
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPicture(Texture &texture, float fX, float fY, Shader &shaderTexture)
{
    /*Bind VertexArrayObject, texture and shader to use*/
    glBindVertexArray(VAO2D);
    texture.Bind();
    shaderTexture.use();

    /*Create transformations*/
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 projectionToPush = glm::mat4(1.0f);

    /*Transformations of Model*/
    model = glm::translate(model, glm::vec3(fX, fY, 0.0f));
    model = glm::scale(model, glm::vec3((float)texture.GetWidth() / (float)SCREEN_WIDTH,
                                        (float)texture.GetHeight() / (float)SCREEN_HEIGHT
                                        , 0.0f));

    /*Transformations of Projection, in this case is Orthographic projection*/
    projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    /*The current matrix projeciton will be pushed in the vertex shader*/
    projectionToPush = projection * view * model;

    shaderTexture.setMat4("uProjection", projectionToPush);
    shaderTexture.setVec4("vColor", m_vColor);

    /*Current draw call*/
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPicture3D(Texture &texture, Shader &shaderTexture, glm::mat4& matrixModel, glm::mat4& matrixView)
{
    /*Bind VertexArrayObject, texture and shader to use*/
    glBindVertexArray(VAO3D);
    texture.Bind();
    shaderTexture.use();

    /*Create transformations*/
    glm::mat4 model         = matrixModel;
    glm::mat4 view          = matrixView;
    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 projectionToPush = glm::mat4(1.0f);

    /*Transformations of Projection, in this case is Perspective projection*/
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    /*The current matrix projeciton will be pushed in the vertex shader*/
    projectionToPush = projection * view * model;

    shaderTexture.setMat4("uProjection", projectionToPush);
    shaderTexture.setVec4("vColor", m_vColor);

    /*Current draw call*/
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPictureScaled(Texture &texture, float fX, float fY, float fScaleFactor, Shader &shaderTexture)
{
    /*Bind VertexArrayObject, texture and shader to use*/
    glBindVertexArray(VAO2D);
    texture.Bind();
    shaderTexture.use();

    /*Create transformations*/
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 projectionToPush = glm::mat4(1.0f);

    /*Transformations of Model*/
    model = glm::translate(model, glm::vec3(fX, fY, 0.0f));
    model = glm::scale(model, glm::vec3(fScaleFactor * ((float)texture.GetWidth() / (float)SCREEN_WIDTH),
                                        fScaleFactor * ((float)texture.GetHeight() / (float)SCREEN_HEIGHT)
                                        , 0.0f));

    /*Transformations of Projection, in this case is Orthographic projection*/
    projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    /*The current matrix projeciton will be pushed in the vertex shader*/
    projectionToPush = projection * view * model;

    shaderTexture.setMat4("uProjection", projectionToPush);
    shaderTexture.setVec4("vColor", m_vColor);

    /*Current draw call*/
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPictureRotated(Texture &texture, float fX, float fY, float fDegrees, Shader &shaderTexture)
{
    /*Bind VertexArrayObject, texture and shader to use*/
    glBindVertexArray(VAO2D);
    texture.Bind();
    shaderTexture.use();

    /*Create transformations*/
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 projectionToPush = glm::mat4(1.0f);

    /*Transformations of Model*/
    model = glm::translate(model, glm::vec3(fX + (float)texture.GetWidth() / 2,
                                            fY + (float)texture.GetHeight() / 2,
                                            0.0f));
    model = glm::rotate(model, glm::radians(fDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-(float)texture.GetWidth() / 2,
                                            -(float)texture.GetHeight() / 2,
                                            0.0f));
    model = glm::scale(model, glm::vec3((float)texture.GetWidth() / (float)SCREEN_WIDTH,
                                        (float)texture.GetHeight() / (float)SCREEN_HEIGHT
                                        , 0.0f));

    /*Transformations of Projection, in this case is Orthographic projection*/
    projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    /*The current matrix projeciton will be pushed in the vertex shader*/
    projectionToPush = projection * view * model;

    shaderTexture.setMat4("uProjection", projectionToPush);
    shaderTexture.setVec4("vColor", m_vColor);

    /*Current draw call*/
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPictureAroundPoint(Texture &texture, float fX, float fY, float fDegrees, float fVertPivotFromCenter, Shader &shaderTexture)
{
    /*Bind VertexArrayObject, texture and shader to use*/
    glBindVertexArray(VAO2D);
    texture.Bind();
    shaderTexture.use();

    /*Create transformations*/
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 projectionToPush = glm::mat4(1.0f);

    /*Transformations of Model*/
    model = glm::translate(model, glm::vec3(fX + (float)texture.GetWidth() / 2,
                                            fY + (float)texture.GetHeight() / 2,
                                            0.0f));
    model = glm::rotate(model, glm::radians(fDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-((float)texture.GetWidth() / 2 - fVertPivotFromCenter),
                                            -(float)texture.GetHeight() / 2,
                                            0.0f));
    model = glm::scale(model, glm::vec3((float)texture.GetWidth() / (float)SCREEN_WIDTH,
                                        (float)texture.GetHeight() / (float)SCREEN_HEIGHT
                                        , 0.0f));

    /*Transformations of Projection, in this case is Orthographic projection*/
    projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    /*The current matrix projeciton will be pushed in the vertex shader*/
    projectionToPush = projection * view * model;

    shaderTexture.setMat4("uProjection", projectionToPush);
    shaderTexture.setVec4("vColor", m_vColor);

    /*Current draw call*/
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawPictureDebug(Texture &texture,
                                float fX,
                                float fY,
                                float fScaleFactor,
                                float fDegrees,
                                float fVertPivotFromCenter,
                                bool bRotateAroundX,
                                bool bRotateAroundY,
                                bool bRotateAroundZ,
                                Shader &shaderTexture)
{

    /*Bind VertexArrayObject, texture and shader to use*/
    float fVertices2DDebug[] =
    {
        m_vec2TopLeft.x, m_vec2TopLeft.y,        /*Texture Top Left*/     m_vec2TextCoordTopLeft.x, m_vec2TextCoordTopLeft.y,
        m_vec2TopRight.x, m_vec2TopRight.y,      /*Texture Top Right*/    m_vec2TextCoordTopRight.x, m_vec2TextCoordTopRight.y,
        m_vec2BottomRight.x, m_vec2BottomRight.y,/*Texture Bottom Right*/ m_vec2TextCoordBottomRight.x, m_vec2TextCoordBottomRight.y,
        m_vec2BottomLeft.x, m_vec2BottomLeft.y,  /*Texture Bottom Left*/  m_vec2TextCoordBottomLeft.x, m_vec2TextCoordBottomLeft.y,
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO2DDebug);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fVertices2DDebug), fVertices2DDebug, GL_DYNAMIC_DRAW);
    glBindVertexArray(VAO2DDebug);
    texture.Bind();
    shaderTexture.use();

    /*Create transformations*/
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 projectionToPush = glm::mat4(1.0f);

    /*Transformations of Model*/
    model = glm::translate(model, glm::vec3(fX + (float)texture.GetWidth() / 2,
                                            fY + (float)texture.GetHeight() / 2,
                                            0.0f));
    model = glm::rotate(model, glm::radians(fDegrees), glm::vec3((float)bRotateAroundX, (float)bRotateAroundY, (float)bRotateAroundZ));
    model = glm::translate(model, glm::vec3(-((float)texture.GetWidth() / 2 - fVertPivotFromCenter),
                                            -(float)texture.GetHeight() / 2,
                                            0.0f));
    model = glm::scale(model, glm::vec3(fScaleFactor * (float)texture.GetWidth() / (float)SCREEN_WIDTH,
                                        fScaleFactor * (float)texture.GetHeight() / (float)SCREEN_HEIGHT
                                        , 0.0f));

    /*Transformations of Projection, in this case is Orthographic projection*/
    projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    /*The current matrix projeciton will be pushed in the vertex shader*/
    projectionToPush = projection * view * model;

    shaderTexture.setMat4("uProjection", projectionToPush);
    shaderTexture.setVec4("vColor", m_vColor);

    /*Current draw call*/
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::SendVertsToVBODebug(glm::vec2 vec2TopLeft,
                                   glm::vec2 vec2TopRight,
                                   glm::vec2 vec2BottomRight,
                                   glm::vec2 vec2BottomLeft)
{
    m_vec2TopLeft = vec2TopLeft;
    m_vec2TopRight = vec2TopRight;
    m_vec2BottomRight = vec2BottomRight;
    m_vec2BottomLeft = vec2BottomLeft;
}

void Renderer::SendTextureCoordToVBODebug(glm::vec2 vec2TextCoordTopLeft,
                                          glm::vec2 vec2TextCoordTopRight,
                                          glm::vec2 vec2TextCoordBottomRight,
                                          glm::vec2 vec2TextCoordBottomLeft)
{
    m_vec2TextCoordTopLeft = vec2TextCoordTopLeft;
    m_vec2TextCoordTopRight = vec2TextCoordTopRight;
    m_vec2TextCoordBottomRight = vec2TextCoordBottomRight;
    m_vec2TextCoordBottomLeft = vec2TextCoordBottomLeft;
}

void Renderer::CallEventsAndSwapBuffers()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

Renderer gRenderer;
