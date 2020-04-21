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

float testTime = 0.0f;
int main()
{
    gRenderer.InitSystem();
    gRenderer.CreateAndFill2DBuffers();
    gRenderer.CreateAndFill3DBuffers();
    gDebug.Init();

    Shader shaderTexture("src/shaders/vertex_2D.vs", "src/shaders/fragment_2D.fs");
    Shader shaderTexture3D("src/shaders/vertex_3D.vs", "src/shaders/fragment_3D.fs");
    Shader shaderWave("src/shaders/vertex_wave.vs", "src/shaders/fragment_wave.fs");

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

        /*Textures from Debug*/
        Shader shaderToUse = shaderTexture;
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
               Currently - "NONE", "Wave", "Lense", "Blur" */

            if(objects.strShader == "NONE")
            {
                shaderToUse = shaderTexture;
            }
            else if(objects.strShader == "Wave")
            {
                shaderToUse = shaderWave;
            }
            else if(objects.strShader == "Lense")
            {
                /*More to add*/
                shaderToUse = shaderTexture;
            }
            else if(objects.strShader == "Blur")
            {
                /*More to add*/
                shaderToUse = shaderTexture;
            }

            gRenderer.DrawPictureDebug(objects.mTexture,
                                       objects.fX,
                                       objects.fY,
                                       objects.fScaleFactor,
                                       objects.fAngle,
                                       0.0f,
                                       shaderToUse);
        }

        shaderWave.setFloat("fTime", testTime);
        testTime+= 0.1f;

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
                                       shaderToUse);

        }



        /*Debug Process and Draw*/
        gDebug.Process();
        gRenderer.CallEventsAndSwapBuffers();
    }

    gDebug.Destroy();
    glfwTerminate();
    return 0;
}

