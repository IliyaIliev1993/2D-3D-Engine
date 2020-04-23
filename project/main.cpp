#include <iostream>
#include "renderer/Renderer.h"
#include "handle_event/HandleEvent.h"
#include "timer_mgr/TimerMgr.hpp"
#include "debug/Debug.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /*Resize Window*/
    glViewport(0, 0, width, height);
}


void CursorPosition(GLFWwindow* window, double xPos, double yPos)
{
    gHandleEvent.CursorInput(window, (float)xPos, (float)yPos);
}

void drop_callback(GLFWwindow* window, int nPathCount, const char* strPaths[])
{
    gDebug.DragFromOutside(nPathCount, strPaths);
}

int main()
{
    gRenderer.InitSystem();
    gRenderer.CreateAndFill2DBuffers();
    gRenderer.CreateAndFill3DBuffers();
    gDebug.Init();

    /* Check Debug.cpp:EnableDisableShader() to add more Shaders
       Currently - "NONE", "Wave" */
    Shader shaderTexture("src/shaders/vertex_2D.vs", "src/shaders/fragment_2D.fs");
    Shader shaderTexture3D("src/shaders/vertex_3D.vs", "src/shaders/fragment_3D.fs");
    Shader shaderWave("src/shaders/vertex_wave.vs", "src/shaders/fragment_wave.fs");

    std::vector<Shader>vecShaders = {shaderTexture, shaderWave};

    /*Resize Window*/
//    glfwSetFramebufferSizeCallback(gRenderer.GetWindow(), framebuffer_size_callback);
    /*Mouse Control*/
    glfwSetCursorPosCallback(gRenderer.GetWindow(), CursorPosition);

    Texture textureBackground;

    TimerMgr timerTest;
    timerTest.Start(1, 15);

    textureBackground.LoadFromPath("src/resources/wallpaper.jpg");

    float fDegrees = 0.0f;
    std::function<void()>callbackTest = [&]
    {
        fDegrees += 1.0f;
    };

    while(!glfwWindowShouldClose(gRenderer.GetWindow()))
    {
        /*Called when dragged and dropped from outside window*/
        glfwSetDropCallback(gRenderer.GetWindow(), drop_callback);

        /*Debug New frame calculation*/
        gDebug.NewFrame();

        /*Handle Event*/
        gHandleEvent.ProcessInput(gRenderer.GetWindow());

        /*Tick Timer*/
        timerTest.Tick(callbackTest);

        /*Clear Color*/
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gRenderer.SetColor(1.0f,1.0f,1.0f,1.0f);
        gRenderer.DrawPicture(textureBackground, 0.0f, 0.0f, shaderTexture);

        gRenderer.SendVertsToVBODebug(/*{300.0f, 0.0f}*/);
        /*Textures from Debug*/
        for(auto& objects : gDebug.GetVecTextureData())
        {
            if(objects.bSelected)
            {
                gRenderer.SetColor(1.0f,1.0f,1.0f,0.75f);
            }
            else
            {
                gRenderer.SetColor(1.0f,1.0f,1.0f,1.0f);
            }

            /* Check Debug.cpp:EnableDisableShader() to add more Shaders
               Currently - "NONE", "Wave"*/
            if(gDebug.GetEnable3D())
            {
                float fXNormalized = ((2 * objects.fX) / gRenderer.SCREEN_WIDTH) - 1;
                float fYNormalized = ((2 * objects.fY) / gRenderer.SCREEN_WIDTH) - 1;
                float fZNormalized = ((2 * gDebug.g_fZCoord) / (gRenderer.SCREEN_DEPTH)) - 1;
                fYNormalized = -1 * fYNormalized;

                glm::mat4 view = glm::mat4(1.0f);
                glm::mat4 model = glm::mat4(1.0f);

                model = glm::translate(model, glm::vec3(fXNormalized, fYNormalized, fZNormalized));
                model = glm::rotate(model, glm::radians(objects.fAngle * -1), glm::vec3((float)gDebug.GetRotateAroundX(),
                                                                                        (float)gDebug.GetRotateAroundY(),
                                                                                        (float)gDebug.GetRotateAroundZ()));
                view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
                gRenderer.DrawPicture3D(objects.mTexture, shaderTexture3D, model, view);

                continue;
            }

            gRenderer.DrawPictureDebug(objects.mTexture,
                                       objects.fX,
                                       objects.fY,
                                       objects.fScaleFactor,
                                       objects.fAngle,
                                       0.0f,
                                       gDebug.GetRotateAroundX(),
                                       gDebug.GetRotateAroundY(),
                                       gDebug.GetRotateAroundZ(),
                                       vecShaders.at(objects.unShaderID));
            /*Wave Needs Time*/
            if(objects.unShaderID == 1)
            {
                vecShaders.at(objects.unShaderID).setFloat("fTime", (float)glfwGetTime());
            }
        }

        gRenderer.SetColor(1.0f,1.0f,1.0f,1.0f);

        /*Animations from Debug*/
        if(!gDebug.GetVecAnimData().empty())
        {
            unsigned int nFrame = gDebug.GetCurrFrameIndex();
            gRenderer.DrawPictureDebug(gDebug.GetVecAnimData().at(nFrame).mTexture,
                                       gDebug.GetVecAnimData().at(nFrame).fX,
                                       gDebug.GetVecAnimData().at(nFrame).fY,
                                       gDebug.GetVecAnimData().at(nFrame).fScaleFactor,
                                       gDebug.GetVecAnimData().at(nFrame).fAngle,
                                       0.0f,
                                       gDebug.GetRotateAroundX(),
                                       gDebug.GetRotateAroundY(),
                                       gDebug.GetRotateAroundZ(),
                                       shaderTexture);

        }



        /*Debug Process and Draw*/
        gDebug.Process();
        gRenderer.CallEventsAndSwapBuffers();
    }

    gDebug.Destroy();
    glfwTerminate();
    return 0;
}

