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

struct TextureDataDebug
{
    Texture mTexture;
    float fX = 0.0f;
    float fY = 0.0f;
    bool bSelected = false;
};

class Debug
{

private:

    static const unsigned int m_unBuffSymbols = 1024;
    bool m_bWriteIntoFile = false;
    bool m_bShowImguiHelp = false;
    bool m_bCreateTexture = false;
    bool m_bCreateAnimation = false;
    bool m_bReplaceCurrentTexture = false;

    ImVec4 m_vec4Color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    std::vector<TextureDataDebug>m_vecTextureData;
    std::vector<TextureDataDebug>m_vecAnimData;
    std::vector<std::string>m_vecStrFiles;
    char m_cArrSymbols[m_unBuffSymbols] = {"/home/default/"};
    char m_cArrSymbolsReload[m_unBuffSymbols] = {"/home/default/"};
    char m_cArrSymbolsAnim[m_unBuffSymbols] = {"/home/default/Animation/scatter_anim_"};
    /* 0 - Begin
     * 1 - Count
     * 2 - FPS */
    int m_nAnimFPS = 0;
    int m_unCurrFrame = 0;
    bool m_bAnimAlreadyLoaded = false;

    TimerMgr m_timerAnim;

    void MainWindow();
    void RadioButtonTextures();
    void CreateTextureAndTextureOptions();
    void CreateAnimAndAnimOpitions();
    void WriteIntoFile();
    void TimerProcess();

    bool GetDirectoryFiles(std::string sPathToDirectory, std::vector<std::string>& vecStrFiles);

public:

    Debug();
    bool Init();
    void Process();
    void Destroy();
    void NewFrame();
    inline std::vector<TextureDataDebug>& GetVecTextureData() {return m_vecTextureData;}
    inline std::vector<TextureDataDebug>& GetVecAnimData() {return m_vecAnimData;}
    inline int& GetCurrFrameIndex() {return m_unCurrFrame;}

    float g_fXCoord = 0.0f;
    float g_fYCoord = 0.0f;

};
extern Debug gDebug;
#endif
