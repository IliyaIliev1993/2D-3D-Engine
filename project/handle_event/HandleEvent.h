#ifndef HANDLEEVENT_H
#define HANDLEEVENT_H

#include <iostream>
#include "../renderer/Renderer.h"

class HandleEvent
{

private:

    float m_fXMemoryMouse = 0.0f;
    float m_fYMemoryMouse = 0.0f;
    float m_fXCurrPosMouse = 0.0f;
    float m_fYCurrPosMouse = 0.0f;
    bool m_bMousePressedNow = false;


public:
    HandleEvent();
    float f_gXPos = 0.0f;
    float f_gYPos = 0.0f;
    void ProcessInput(GLFWwindow* window);
    void CursorInput(GLFWwindow* window, float fXPos, float fYPos);

    float GetXMouse() {return m_fXCurrPosMouse;}
    float GetYMouse() {return m_fYCurrPosMouse;}
};

extern HandleEvent gHandleEvent;
#endif
