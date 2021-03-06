#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <algorithm>
#include <dirent.h>
#include "../texture/Texture.h"
#include "../gl_includes/imgui/imgui.h"
#include "../gl_includes/imgui/imgui_impl_glfw_gl3.h"
#include "../timer_mgr/TimerMgr.hpp"
#include "../renderer/Renderer.h"

enum Shaders
{
    eShaderTexture,
    eShaderWave,
    eShaderClip,
    eShaderCrop,
    eShaderLens
};

struct TextureDataDebug
{
    Texture mTexture;
    float fX = 0.0f;
    float fY = 0.0f;
    int nDynamicSourceX = 0;
    int nDynamicSourceY = 0;
    int nDynamicSourceW = mTexture.GetWidth();
    int nDynamicSourceH = mTexture.GetHeight();
    bool bSelected = false;
    float fAngle = 0.0f;
    float fScaleFactor = 1.0f;
    unsigned int unShaderID = 0;
};

class Debug
{

private:

    static const unsigned int m_unBuffSymbols = 1024;

    Shader shaderRect;
    Shader shaderTexture;
    Shader shaderTexture3D;
    Shader shaderWave;
    Shader shaderClip;
    Shader shaderCrop;
    Shader shaderLens;

    glm::vec4 m_vec4ClipCrop = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    bool m_bWriteIntoFile = false;
    bool m_bShowImguiHelp = false;
    bool m_bCreateTexture = false;
    bool m_bCreateAnimation = false;
    bool m_bReplaceCurrentTexture = false;
    bool m_bEnableDrag = false;
    bool m_bEnableCut = false;
    bool m_bSimulateRotation = false;
    bool m_bShaders = false;
    bool m_bEnable3D = false;

    std::vector<Shader>m_vecShaders;
    ImVec4 m_vec4Color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    std::vector<TextureDataDebug>m_vecTextureData;
    std::vector<TextureDataDebug>m_vecAnimData;
    std::vector<std::string>m_vecStrFiles;
    char m_cArrSymbols[m_unBuffSymbols] = {"/home/default/"};
    char m_cArrSymbolsReload[m_unBuffSymbols] = {"/home/default/"};
    char m_cArrSymbolsAnim[m_unBuffSymbols] = {"/home/default/Animation/scatter_anim_"};

    int m_nAnimFPS = 0;
    int m_unCurrFrame = 0;
    int m_nCurrShader = 0;
    bool m_bAnimAlreadyLoaded = false;
    bool m_bRotateAroundAxisX = false;
    bool m_bRotateAroundAxisY = false;
    bool m_bRotateAroundAxisZ = true;
    float m_fSpeedRotation = 0.1f;
    float m_fMemoryUsage = 0.0f;

    TimerMgr m_timerAnim;
    TimerMgr m_timerRotation;

    void MainWindow();
    void TextureAndRectModify();
    void RadioButtonTextures();
    void CreateTextureAndTextureOptions();
    void CreateAnimAndAnimOpitions();
    void WriteIntoFile();
    void EnableDisableDrag();
    void SimulateRotation();
    void SimulateRotationLabel();
    void CalculateMemoryOfPNG();
    void EnableDisableShader();
    void EnableDisableImageCut();
    void RightClickAndSelect();
    void TimerProcess();
    void Draw();
    void CurrentShaderEffect(unsigned int nShaderToApply);

    bool GetDirectoryFiles(std::string sPathToDirectory, std::vector<std::string>& vecStrFiles);

public:

    Debug();
    bool Init();
    void Process();
    void Destroy();
    void NewFrame();
    bool DragFromOutside(int nPathCount, const char* strPaths[]);
    inline std::vector<TextureDataDebug>& GetVecTextureData() {return m_vecTextureData;}
    inline std::vector<TextureDataDebug>& GetVecAnimData() {return m_vecAnimData;}
    inline int& GetCurrFrameIndex() {return m_unCurrFrame;}
    inline bool& GetRotateAroundX() {return m_bRotateAroundAxisX;}
    inline bool& GetRotateAroundY() {return m_bRotateAroundAxisY;}
    inline bool& GetRotateAroundZ() {return m_bRotateAroundAxisZ;}
    inline bool& GetEnable3D() {return m_bEnable3D;}

    float g_fXCoord = 0.0f;
    float g_fYCoord = 0.0f;
    float g_fZCoord = gRenderer.SCREEN_DEPTH;
    float g_fXCoordMemory = 0.0f;
    float g_fYCoordMemory = 0.0f;

    float g_fAngle = 0.0f;
    float g_fScale = 0.0f;

};
extern Debug gDebug;
#endif
