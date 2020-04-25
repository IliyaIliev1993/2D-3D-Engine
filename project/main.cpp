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
    Shader shaderRect("src/shaders/vertex_rect.vs", "src/shaders/fragment_rect.fs");
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

        /*Debug Process and Draw*/
        gDebug.Process();
        gRenderer.CallEventsAndSwapBuffers();
    }

    gDebug.Destroy();
    glfwTerminate();
    return 0;
}

