#include "Debug.h"

const unsigned int TIMER_ID_ANIMATION = 1;
const unsigned int TIMER_ID_SIMULATE_ROTATION = 2;

Debug::Debug()
{

}

bool Debug::Init()
{
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(gRenderer.GetWindow(), true);
    ImGui::StyleColorsDark();

    shaderRect = Shader("src/shaders/vertex_rect.vs", "src/shaders/fragment_rect.fs");
    shaderTexture = Shader("src/shaders/vertex_2D.vs", "src/shaders/fragment_2D.fs");
    shaderTexture3D = Shader("src/shaders/vertex_3D.vs", "src/shaders/fragment_3D.fs");
    shaderWave = Shader("src/shaders/vertex_wave.vs", "src/shaders/fragment_wave.fs");
    shaderClip = Shader("src/shaders/vertex_clip.vs", "src/shaders/fragment_clip.fs");
    shaderCrop = Shader("src/shaders/vertex_crop.vs", "src/shaders/fragment_crop.fs");
    shaderLens = Shader("src/shaders/vertex_lens.vs", "src/shaders/fragment_lens.fs");

    m_vecShaders = {shaderTexture, shaderWave, shaderClip, shaderCrop, shaderLens};

    return true;
}

bool Debug::DragFromOutside(int nPathCount, const char *strPaths[])
{
    if(m_bCreateAnimation)
    {
        std::string sPath = *strPaths;
        sPath.append("/");
        std::strcpy(m_cArrSymbolsAnim, sPath.c_str());

        return true;
    }

    TextureDataDebug dataToPush;
    for(int i = 0; i < nPathCount; i++)
    {
        if(dataToPush.mTexture.LoadFromPath(strPaths[i]))
        {
            m_vecTextureData.emplace_back(dataToPush);
        }
        else
        {
            continue;
        }
    }

    return true;
}

void Debug::RadioButtonTextures()
{
    ImGui::Text("");
    if(!m_vecTextureData.empty())
    {
        ImGui::Text("Select a Texture to work with");
    }

    for(unsigned int i = 0; i < m_vecTextureData.size(); i++)
    {
        std::string strPath = m_vecTextureData.at(i).mTexture.GetPath() +
                " - " +
                std::to_string(i) +
                " - " +
                "X: " +
                std::to_string((int)m_vecTextureData.at(i).fX) +
                " - " +
                "Y: " +
                std::to_string((int)m_vecTextureData.at(i).fY);

        if(ImGui::RadioButton(strPath.c_str(),
                              m_vecTextureData.at(i).bSelected))
        {
            g_fXCoord = m_vecTextureData.at(i).fX;
            g_fYCoord = m_vecTextureData.at(i).fY;
            g_fXCoordMemory = m_vecTextureData.at(i).fX;
            g_fYCoordMemory = m_vecTextureData.at(i).fY;
            g_fAngle = m_vecTextureData.at(i).fAngle;
            g_fScale = m_vecTextureData.at(i).fScaleFactor;
            m_vecTextureData.at(i).bSelected = !m_vecTextureData.at(i).bSelected;
            m_vecTextureData.at(i).nDynamicSourceX = m_vecTextureData.at(i).mTexture.GetSourceX();
            m_vecTextureData.at(i).nDynamicSourceY = m_vecTextureData.at(i).mTexture.GetSourceY();
            m_vecTextureData.at(i).nDynamicSourceW = m_vecTextureData.at(i).mTexture.GetSourceW();
            m_vecTextureData.at(i).nDynamicSourceH = m_vecTextureData.at(i).mTexture.GetSourceH();
        }

        if(m_vecTextureData.at(i).bSelected)
        {
            m_vecTextureData.at(i).fX = g_fXCoord;
            m_vecTextureData.at(i).fY = g_fYCoord;
            m_vecTextureData.at(i).fAngle = g_fAngle;
            m_vecTextureData.at(i).fScaleFactor = g_fScale;
        }

    }

    ImGui::Text("");
    if(!m_vecAnimData.empty())
    {
        ImGui::Text("Select Animation to work with");

        std::string sPathAnim = m_cArrSymbolsAnim;
        std::string strPath = sPathAnim +
                "   " +
                "X: " +
                std::to_string((int)m_vecAnimData.at(m_unCurrFrame).fX) +
                " - " +
                "Y: " +
                std::to_string((int)m_vecAnimData.at(m_unCurrFrame).fY);

        if(ImGui::RadioButton(strPath.c_str(),
                              m_vecAnimData.at(0).bSelected))
        {
            for(unsigned int i = 0; i < m_vecAnimData.size(); i++)
            {
                g_fXCoord = m_vecAnimData.at(i).fX;
                g_fYCoord = m_vecAnimData.at(i).fY;
                g_fXCoordMemory = m_vecAnimData.at(i).fX;
                g_fYCoordMemory = m_vecAnimData.at(i).fY;
                g_fAngle = m_vecAnimData.at(i).fAngle;
                g_fScale = m_vecAnimData.at(i).fScaleFactor;
                m_vecAnimData.at(i).bSelected = !m_vecAnimData.at(i).bSelected;
            }
        }

        if(m_vecAnimData.at(0).bSelected)
        {
            for(unsigned int i = 0; i < m_vecAnimData.size(); i++)
            {
                m_vecAnimData.at(i).fX = g_fXCoord;
                m_vecAnimData.at(i).fY = g_fYCoord;
                m_vecAnimData.at(i).fAngle = g_fAngle;
                m_vecAnimData.at(i).fScaleFactor = g_fScale;
            }
        }
    }

}

void Debug::CreateTextureAndTextureOptions()
{
    if (m_bCreateTexture)
    {
        ImGui::Begin("Texture Options", &m_bCreateTexture);
        ImGui::Text("Texture Path:");

        ImGui::InputText("",m_cArrSymbols,m_unBuffSymbols);
        ImGui::SameLine();

        if (ImGui::Button("Add new texture"))
        {
            TextureDataDebug mData;
            if(mData.mTexture.LoadFromPath(m_cArrSymbols))
            {
                m_vecTextureData.emplace_back(mData);
            }
        }

        ImGui::Text("");
        ImGui::Text("Texture Loaded: %d", m_vecTextureData.size());
        ImGui::Text("");

        //Tree Textures
        for(unsigned int i = 0; i < m_vecTextureData.size(); i++)
        {
            std::string strNodeId = m_vecTextureData.at(i).mTexture.GetPath() + " - " + std::to_string(i);
            if(ImGui::TreeNode(strNodeId.c_str()))
            {
                if (ImGui::Button("Remove texture"))
                {
                    m_vecTextureData.at(i).mTexture.Unbind();
                    m_vecTextureData.at(i).mTexture.DeleteTexture();
                    m_vecTextureData.erase(m_vecTextureData.begin() + i);
                }

                ImGui::SameLine();
                if (ImGui::Button("Replace current texture"))
                {
                    m_bReplaceCurrentTexture = true;
                }
                else if(m_bReplaceCurrentTexture)
                {
                    ImGui::Begin("Replace current texture", &m_bReplaceCurrentTexture);
                    ImGui::Text("New Path:");
                    ImGui::InputText("",m_cArrSymbolsReload,m_unBuffSymbols);
                    if(m_cArrSymbolsReload == m_cArrSymbols)
                    {
                        if(ImGui::Button(""))
                        {
                            m_bReplaceCurrentTexture = false;
                        }
                    }
                    else
                    {
                        if (ImGui::Button("Replace"))
                        {
                            m_vecTextureData.at(i).mTexture.Unbind();
                            m_vecTextureData.at(i).mTexture.DeleteTexture();
                            m_vecTextureData.at(i).mTexture.LoadFromPath(m_cArrSymbolsReload);
                            m_bReplaceCurrentTexture = false;
                        }

                        ImGui::End();
                    }

                }

                ImGui::TreePop();
            }
        }



        if (ImGui::Button("Close"))
        {
            m_bCreateTexture = false;
        }

        ImGui::End();
    }

}

void Debug::CreateAnimAndAnimOpitions()
{
    if(m_bCreateAnimation)
    {
        ImGui::Begin("Animation Options", &m_bCreateAnimation);
        ImGui::Text("Insert Animation path or Drag Here the folder:");
        ImGui::InputText("",m_cArrSymbolsAnim,m_unBuffSymbols);

        if(ImGui::TreeNode("Anim Settings"))
        {
            if(m_bAnimAlreadyLoaded)
            {
                ImGui::Text("Frame Begin: %s", m_vecStrFiles.at(0).c_str());
                ImGui::Text("Frame End: %s", m_vecStrFiles.at(m_vecStrFiles.size() - 1).c_str());
                ImGui::Text("Frames Count: %d", m_vecStrFiles.size());
            }
            ImGui::Text("FPS:");
            ImGui::InputInt("FPS", &m_nAnimFPS);
            /*FPS*/
            if(m_nAnimFPS != 0)
            {
                std::string sButtonName;
                if(m_bAnimAlreadyLoaded)
                {
                    sButtonName = "Reload ID's and Textures";
                }
                else
                {
                    sButtonName = "Create ID's and Load Textures";
                }

                if (ImGui::Button(sButtonName.c_str()))
                {

                    if(m_bAnimAlreadyLoaded)
                    {
                        for(unsigned int i = 0; i < m_vecAnimData.size(); i++)
                        {
                            m_vecAnimData.at(i).mTexture.Unbind();
                            m_vecAnimData.at(i).mTexture.DeleteTexture();
                        }

                        m_vecAnimData.clear();
                        m_vecStrFiles.clear();
                    }

                    std::string sPathToAnim = m_cArrSymbolsAnim;
                    GetDirectoryFiles(sPathToAnim,m_vecStrFiles);

                    for(unsigned int i = 0; i < m_vecStrFiles.size(); i++)
                    {
                        TextureDataDebug mData;
                        std::string sPath = sPathToAnim + m_vecStrFiles.at(i);

                        if(mData.mTexture.LoadFromPath(sPath))
                        {
                            m_vecAnimData.emplace_back(mData);
                        }

                    }

                    m_bAnimAlreadyLoaded = true;

                }

                if(m_bAnimAlreadyLoaded)
                {
                    ImGui::SameLine();
                    if(ImGui::Button("Change FPS Only"))
                    {
                        m_timerAnim.Stop();
                        m_timerAnim.Start(TIMER_ID_ANIMATION, (int)(1000 / m_nAnimFPS));
                    }

                    /*Remove Animation Button*/
                    if(ImGui::Button("Remove animation"))
                    {
                        m_timerAnim.Stop();
                        m_unCurrFrame = 0;

                        for(unsigned int i = 0; i < m_vecAnimData.size(); i++)
                        {
                            m_vecAnimData.at(i).mTexture.Unbind();
                            m_vecAnimData.at(i).mTexture.DeleteTexture();
                        }

                        m_vecAnimData.clear();
                        m_vecStrFiles.clear();
                        m_bAnimAlreadyLoaded = false;
                    }
                }

                ImGui::Text("After Pressing the buttons LOAD/RELOAD/REMOVE, check the Loading process in console");
                ImGui::Text("");

            }


            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Anim Options"))
        {
            if(!m_vecAnimData.empty())
            {
                if (ImGui::Button("Play"))
                {
                    if(!m_timerAnim.IsStarted())
                    {
                        m_timerAnim.Start(TIMER_ID_ANIMATION, (int)(1000 / m_nAnimFPS));
                    }

                }

                ImGui::SameLine();
                ImGui::SliderInt("Frame", &m_unCurrFrame, 0, m_vecAnimData.size() -1);

                if (ImGui::Button("Pause"))
                {
                    m_timerAnim.Stop();
                }
                if (ImGui::Button("Stop"))
                {
                    m_timerAnim.Stop();
                    m_unCurrFrame = 0;
                }
            }

            ImGui::TreePop();
        }


        if (ImGui::Button("Close"))
        {
            m_bCreateAnimation = false;
        }

        ImGui::End();
    }
}

void Debug::WriteIntoFile()
{
    if (ImGui::Button("Write changes into file..."))
    {
        const char *sPath = "/home/default/Desktop/ExportFromEngine.txt";
        std::ofstream dataToWrite(sPath);

        std::string sData = "<game_rss_list>\n"
                            "<ResourceExpression expression = \"{  }\" />\n";

        for(unsigned int i = 0; i < m_vecTextureData.size(); i++)
        {

            sData += "\t<Image key=\"IMAGE_ID_" + std::to_string(i + 1) + "\"" +
                     " fileName=\"" + m_vecTextureData.at(i).mTexture.GetPath() + "\"" +
                     " x=" + "\"" + std::to_string(m_vecTextureData.at(i).mTexture.GetSourceX()) + "\"" +
                     " y=" + "\"" + std::to_string(m_vecTextureData.at(i).mTexture.GetSourceY()) + "\"" +
                     " width=" + "\"" + std::to_string(m_vecTextureData.at(i).mTexture.GetSourceW()) + "\"" +
                     " height=" + "\"" + std::to_string(m_vecTextureData.at(i).mTexture.GetSourceH()) + "\"" +
                     " priority=\"200\"/>\n";

        }
        sData += "</game_rss_list>\n";
        sData += "DESTINATIONS: \n";

        for(unsigned int i = 0; i < m_vecTextureData.size(); i++)
        {
            sData +="IMAGE_ID_" + std::to_string(i + 1) + " - " +
                    "X = " +
                    std::to_string((int)m_vecTextureData.at(i).fX) +
                    " Y = " +
                    std::to_string((int)m_vecTextureData.at(i).fY) + " \n";
        }

        dataToWrite << sData;
        dataToWrite.close();
    }
}

void Debug::EnableDisableShader()
{
    ImGui::Checkbox("Shaders", &m_bShaders);

    if(!m_bShaders)
    {
        return;
    }

    ImGui::Begin("Shaders", &m_bShaders);

    const unsigned int unSizeOfArrShaders = m_vecShaders.size();
    const char* strArrListOfShaders[unSizeOfArrShaders] = {"NONE", "Wave", "Clip", "Crop", "Lens"};

    ImGui::ListBox("List of Shaders", &m_nCurrShader, strArrListOfShaders, unSizeOfArrShaders);

    switch (m_nCurrShader)
    {
    case eShaderWave:
    {
        ImGui::InputFloat("Center wave X", &m_vec4ClipCrop.x, 0.01f);
        ImGui::InputFloat("Center wave Y", &m_vec4ClipCrop.y, 0.01f);
        ImGui::InputFloat("Aplitude", &m_vec4ClipCrop.w, 0.01f);
        ImGui::InputFloat("Speed", &m_vec4ClipCrop.z, 0.01f);
    }
        break;
    case eShaderClip:
    case eShaderCrop:
    {
        ImGui::InputFloat("X", &m_vec4ClipCrop.x, 1.0f);
        ImGui::InputFloat("Y", &m_vec4ClipCrop.y, 1.0f);
        ImGui::InputFloat("W", &m_vec4ClipCrop.w, 1.0f);
        ImGui::InputFloat("H", &m_vec4ClipCrop.z, 1.0f);
    }
        break;

    case eShaderLens:
    {
        ImGui::InputFloat("X", &m_vec4ClipCrop.x, 0.005f);
        ImGui::InputFloat("Y", &m_vec4ClipCrop.y, 0.005f);
        ImGui::InputFloat("Zoom", &m_vec4ClipCrop.w, 0.005f);
    }
        break;

    default:
        break;
    }

    if(ImGui::Button("Apply..."))
    {
        for(unsigned int i = 0; i < m_vecTextureData.size(); i++)
        {
            if(m_vecTextureData.at(i).bSelected)
            {
                m_vecTextureData.at(i).unShaderID = m_nCurrShader;
            }
        }
    }
    ImGui::End();

}

void Debug::CurrentShaderEffect(unsigned int nShaderToApply)
{
    switch (nShaderToApply)
    {
    case eShaderWave:
    {
        m_vecShaders.at(nShaderToApply).setFloat("fTime", (float)glfwGetTime());
        m_vecShaders.at(nShaderToApply).setVec4("vecDatas", m_vec4ClipCrop);
    }
        break;

    case eShaderClip:
    {
        m_vecShaders.at(nShaderToApply).setVec4("vecClip", m_vec4ClipCrop);
    }
        break;

    case eShaderCrop:
    {
        m_vecShaders.at(nShaderToApply).setVec4("vecCrop", m_vec4ClipCrop);
    }
        break;

    case eShaderLens:
    {
        m_vecShaders.at(nShaderToApply).setVec4("vecDatas", m_vec4ClipCrop);
    }
        break;

    default:
        break;
    }
}

void Debug::EnableDisableDrag()
{
    ImGui::Checkbox("Enable Dragging", &m_bEnableDrag);

    if(!m_bEnableDrag)
    {
        return;
    }

    /* 0 - Mouse Left, 1 - Mouse Right */
    if(ImGui::IsMouseDown(0))
    {
        g_fXCoord = ImGui::GetMouseDragDelta(0).x + g_fXCoordMemory;
        g_fYCoord = ImGui::GetMouseDragDelta(0).y + g_fYCoordMemory;
    }
    else if(ImGui::IsMouseReleased(0))
    {
        g_fXCoordMemory = g_fXCoord;
        g_fYCoordMemory = g_fYCoord;
    }
}

void Debug::EnableDisableImageCut()
{
    ImGui::SameLine();
    ImGui::Checkbox("Enable Image Cut", &m_bEnableCut);
}

void Debug::SimulateRotation()
{
    if(!m_bSimulateRotation)
    {
        if(m_timerRotation.IsStarted())
        {
            m_timerRotation.Stop();
        }

        return;
    }

    if(!m_timerRotation.IsStarted())
    {
        m_timerRotation.Start(TIMER_ID_SIMULATE_ROTATION, 16);
    }
}

void Debug::TimerProcess()
{
    std::function<void()>callbackFrame = [&]
    {
        if(m_vecAnimData.empty())
        {
            return;
        }

        m_unCurrFrame++;
        if(m_unCurrFrame >= m_vecAnimData.size() - 1)
        {
            m_unCurrFrame = 0;
        }

    };

    std::function<void()>callbackRotation = [&]
    {
        g_fAngle += m_fSpeedRotation;
        if(g_fAngle > 360.0f)
        {
            g_fAngle = 0.0f;
        }
        else if(g_fAngle < 0.0f)
        {
            g_fAngle = 360.0f;
        }
    };

    m_timerAnim.Tick(callbackFrame);
    m_timerRotation.Tick(callbackRotation);
}

void Debug::CalculateMemoryOfPNG()
{
    float fCurrCalc = 0.0f;
    for(auto& objects : m_vecTextureData)
    {
        fCurrCalc += (objects.mTexture.GetWidth() * objects.mTexture.GetHeight()) * 4;

    }

    for(auto& objects : m_vecAnimData)
    {
        fCurrCalc += (objects.mTexture.GetWidth() * objects.mTexture.GetHeight()) * 4;
    }

    m_fMemoryUsage = fCurrCalc / 1000000;
}

void Debug::MainWindow()
{
    ImGui::Text("Texture Debug");
    ImGui::SameLine();
    ImGui::Text("RENDERING %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SameLine();
    ImGui::Text(", Resolution: %d x %d", gRenderer.SCREEN_WIDTH, gRenderer.SCREEN_HEIGHT);
    ImGui::SameLine();
    ImGui::Text(", Memory Usage of PNG's : %f MB", m_fMemoryUsage);
    ImGui::SliderFloat("X Coords", &g_fXCoord, 0.0f, (float)gRenderer.SCREEN_WIDTH);
    ImGui::SliderFloat("Y Coords", &g_fYCoord, 0.0f, (float)gRenderer.SCREEN_HEIGHT);
    ImGui::SliderFloat("Angle Degrees", &g_fAngle, 0.0f, 360.0f);
    ImGui::SliderFloat("Scale Factor", &g_fScale, 0.0f, 1.0f);
    ImGui::Text("");
    ImGui::InputFloat("X Precision", &g_fXCoord, 1.0f);
    ImGui::InputFloat("Y Precision", &g_fYCoord, 1.0f);
    ImGui::InputFloat("Angle Precision", &g_fAngle, 0.1f);
    ImGui::InputFloat("Scale Precision", &g_fScale, 0.01f);
    ImGui::ColorEdit4("Texture color", (float*)&m_vec4Color);
    ImGui::Text("");
    ImGui::Checkbox("Texture Options", &m_bCreateTexture);
    ImGui::SameLine();
    ImGui::Checkbox("Animation Options", &m_bCreateAnimation);
    ImGui::SameLine();
    ImGui::Checkbox("Imgui HELP", &m_bShowImguiHelp);
}

void Debug::TextureAndRectModify()
{
    if(m_bEnable3D || !m_bEnableCut)
    {
        return;
    }
    const float fRectToFit = 350.0f;
    const float fWidthVertSliders = 20.0f;
    const float fOffsetHorizontalSliders = 5.0f;
    const float fPosClipX = 53.0f + ImGui::GetWindowPos().x;
    const float fPosClipY = 474.0f + ImGui::GetWindowPos().y;
    float fResizeValue = 1.0f;
    for(unsigned int i = 0; i < m_vecTextureData.size(); i++)
    {
        if(m_vecTextureData.at(i).bSelected)
        {
            if(m_vecTextureData.at(i).mTexture.GetWidth() > fRectToFit ||
               m_vecTextureData.at(i).mTexture.GetHeight() > fRectToFit)
            {
                fResizeValue = 3.0f;
            }

            ImGui::NewLine();
            ImGui::Text("Selected: ");
            const float fWidth = (m_vecTextureData.at(i).mTexture.GetWidth() / fResizeValue);
            const float fHeight = (m_vecTextureData.at(i).mTexture.GetHeight() / fResizeValue);
            /*Fix of problem with STBI_LOAD flipped*/
            glm::vec2 vecFlippedMinUV = glm::vec2(0.0f, 1.0f);
            glm::vec2 vecFlippedMaxUV = glm::vec2(1.0f, 0.0f);

            ImGui::PushItemWidth(fWidth);
            ImGui::NewLine();
            ImGui::SameLine();
            ImGui::Dummy({(fWidthVertSliders/2) + fOffsetHorizontalSliders, 0.0f});
            ImGui::SameLine();
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::SliderInt("X", &m_vecTextureData.at(i).nDynamicSourceX, 0.0f, m_vecTextureData.at(i).mTexture.GetWidth());
            ImGui::NewLine();
            ImGui::VSliderInt(" Y", {fWidthVertSliders, fHeight}, &m_vecTextureData.at(i).nDynamicSourceY, m_vecTextureData.at(i).mTexture.GetHeight(), 0.0f);
            ImGui::SameLine();

            const float fCurrentPosClipX = fPosClipX + (m_vecTextureData.at(i).nDynamicSourceX / fResizeValue);

            const float fCurrentPosClipY = fPosClipY + (m_vecTextureData.at(i).nDynamicSourceY / fResizeValue);

            const float fCurrentPosClipW = (fPosClipX + fWidth + (m_vecTextureData.at(i).nDynamicSourceX / fResizeValue)) -
                    ((m_vecTextureData.at(i).mTexture.GetWidth() - m_vecTextureData.at(i).nDynamicSourceW) / fResizeValue);

            const float fCurrentPosClipH = (fPosClipY + fHeight + m_vecTextureData.at(i).nDynamicSourceY / fResizeValue) -
                    ((m_vecTextureData.at(i).mTexture.GetHeight() - m_vecTextureData.at(i).nDynamicSourceH) / fResizeValue);

            ImGui::PushClipRect({fCurrentPosClipX, fCurrentPosClipY}, {fCurrentPosClipW, fCurrentPosClipH}, true);
            ImGui::Image(ImTextureID((intptr_t)m_vecTextureData.at(i).mTexture.GetTexture()),
                                      {fWidth, fHeight},
                                      {vecFlippedMinUV.x, vecFlippedMinUV.y},
                                      {vecFlippedMaxUV.x, vecFlippedMaxUV.y});
            ImGui::PopClipRect();

            ImGui::SameLine();
            ImGui::Text("H");
            ImGui::SameLine();
            ImGui::VSliderInt("", {fWidthVertSliders, fHeight}, &m_vecTextureData.at(i).nDynamicSourceH, m_vecTextureData.at(i).mTexture.GetHeight(), 0.0f);
            ImGui::NewLine();
            ImGui::NewLine();
            ImGui::SameLine();
            ImGui::Dummy({(fWidthVertSliders/2) + fOffsetHorizontalSliders, 0.0f});
            ImGui::SameLine();
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::SliderInt("W", &m_vecTextureData.at(i).nDynamicSourceW, 0.0f, m_vecTextureData.at(i).mTexture.GetWidth());

            ImGui::Text("Previous values: X: %d Y: %d W: %d H: %d",
                        m_vecTextureData.at(i).mTexture.GetSourceX(),
                        m_vecTextureData.at(i).mTexture.GetSourceY(),
                        m_vecTextureData.at(i).mTexture.GetSourceW(),
                        m_vecTextureData.at(i).mTexture.GetSourceH());
            break;
        }

    }

    if(ImGui::Button("Apply Changes..."))
    {
        for(unsigned int i = 0; i < m_vecTextureData.size(); i++)
        {
            if(m_vecTextureData.at(i).bSelected)
            {
                m_vecTextureData.at(i).mTexture.SetSourceX(m_vecTextureData.at(i).nDynamicSourceX);
                m_vecTextureData.at(i).mTexture.SetSourceY(m_vecTextureData.at(i).nDynamicSourceY);
                m_vecTextureData.at(i).mTexture.SetSourceW(m_vecTextureData.at(i).nDynamicSourceW);
                m_vecTextureData.at(i).mTexture.SetSourceH(m_vecTextureData.at(i).nDynamicSourceH);
                break;
            }
        }
    }
}

void Debug::SimulateRotationLabel()
{
    ImGui::Checkbox("Simulate Rotation", &m_bSimulateRotation);
    ImGui::SameLine();
    ImGui::Checkbox("Enable 3D", &m_bEnable3D);
    if(m_bEnable3D)
    {
        ImGui::SameLine();
        ImGui::Checkbox("Rotate arond X Axis", &m_bRotateAroundAxisX);
        ImGui::SameLine();
        ImGui::Checkbox("Rotate arond Y Axis", &m_bRotateAroundAxisY);
        ImGui::SameLine();
        ImGui::Checkbox("Rotate around Z Axis", &m_bRotateAroundAxisZ);
        ImGui::SliderFloat("Z Coords", &g_fZCoord, -(float)gRenderer.SCREEN_DEPTH * 5, (float)gRenderer.SCREEN_DEPTH * 2);
    }
    else
    {
        m_bRotateAroundAxisX = false;
        m_bRotateAroundAxisY = false;
        ImGui::SameLine();
        ImGui::Checkbox("Rotate around Z Axis", &m_bRotateAroundAxisZ);
    }

    if(!m_bRotateAroundAxisX && !m_bRotateAroundAxisY)
    {
        m_bRotateAroundAxisZ = true;
    }

    if(m_bSimulateRotation)
    {
        const float fSpeedIncrement = 0.1f;
        ImGui::SameLine();
        ImGui::Text("   Speed: %f", m_fSpeedRotation);
        ImGui::SameLine();
        if(ImGui::Button("-"))
        {
            m_fSpeedRotation -= fSpeedIncrement;
        }
        ImGui::SameLine();
        if(ImGui::Button("+"))
        {
            m_fSpeedRotation += fSpeedIncrement;
        }
    }

    /*Simulate Rotation*/
    SimulateRotation();
}

void Debug::Process()
{
    /*Draw Textures and Animations*/
    Draw();

    /*Timer Process call function*/
    TimerProcess();

    /*Write into txt file all the data*/
    WriteIntoFile();

    /*Main Window*/
    MainWindow();

    /*Enable Disable Drag function*/
    EnableDisableDrag();

    /*Enable Disable Image Cut function*/
    EnableDisableImageCut();

    /*Simulate Rotation function*/
    SimulateRotationLabel();

    /*Shaders Menu*/
    EnableDisableShader();

    /*Small Window texture preview*/
    TextureAndRectModify();

    /*Selectable Radio Button Textures*/
    RadioButtonTextures();

    /*Select with double click*/
    RightClickAndSelect();

    //Help IMGUI
    if (m_bShowImguiHelp)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow(&m_bShowImguiHelp);
    }

    //Create Texture
    CreateTextureAndTextureOptions();

    //Create Animations
    CreateAnimAndAnimOpitions();

    /*PNG Memory usage*/
    CalculateMemoryOfPNG();

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void Debug::RightClickAndSelect()
{
    int xMouse = ImGui::GetMousePos().x;
    int yMouse = ImGui::GetMousePos().y;

    /*Right Mouse Button*/
    for (unsigned int i = m_vecTextureData.size(); i-- > 0; )
    {
        bool bInCoords = (xMouse >= m_vecTextureData.at(i).fX) &&
                         (xMouse <= m_vecTextureData.at(i).fX + m_vecTextureData.at(i).mTexture.GetWidth()) &&
                         (yMouse >= m_vecTextureData.at(i).fY) &&
                         (yMouse <= m_vecTextureData.at(i).fY + m_vecTextureData.at(i).mTexture.GetHeight());

        if(ImGui::IsMouseClicked(1) && bInCoords)
        {
            g_fXCoord = m_vecTextureData.at(i).fX;
            g_fYCoord = m_vecTextureData.at(i).fY;
            g_fXCoordMemory = m_vecTextureData.at(i).fX;
            g_fYCoordMemory = m_vecTextureData.at(i).fY;
            g_fAngle = m_vecTextureData.at(i).fAngle;
            g_fScale = m_vecTextureData.at(i).fScaleFactor;
            m_vecTextureData.at(i).bSelected = !m_vecTextureData.at(i).bSelected;
            m_vecTextureData.at(i).nDynamicSourceX = m_vecTextureData.at(i).mTexture.GetSourceX();
            m_vecTextureData.at(i).nDynamicSourceY = m_vecTextureData.at(i).mTexture.GetSourceY();
            m_vecTextureData.at(i).nDynamicSourceW = m_vecTextureData.at(i).mTexture.GetSourceW();
            m_vecTextureData.at(i).nDynamicSourceH = m_vecTextureData.at(i).mTexture.GetSourceH();
            break;
        }

    }
}

bool Debug::GetDirectoryFiles(std::string sPathToDirectory, std::vector<std::string> &vecStrFiles)
{
    DIR *dirPtr = nullptr;
    struct dirent *direntPtr = nullptr;

    if((dirPtr = opendir(sPathToDirectory.c_str())) == nullptr)
    {
      std::cout << "Failed to open directory: " << sPathToDirectory << std::endl;
      return false;
    }

    while ((direntPtr = readdir(dirPtr)) != nullptr)
    {
      vecStrFiles.push_back(std::string(direntPtr->d_name));
//      std::cout << "Reading File ... " << direntPtr->d_name << std::endl;
    }

    closedir(dirPtr);

    for (unsigned int i = 0; i < vecStrFiles.size(); i++)
    {
        std::string sCurrentString = vecStrFiles.at(i);
        std::string sStrToFound = ".png";
        std::size_t strFound = sCurrentString.find(sStrToFound);
        if (!(strFound != std::string::npos))
        {
            vecStrFiles.erase(vecStrFiles.begin() + i);
        }

    }

    std::sort(vecStrFiles.begin(), vecStrFiles.end());

    return true;
}

void Debug::Draw()
{
    gRenderer.SendVertsToVBODebug();
    gRenderer.SendTextureCoordToVBODebug();
    /*Textures from Debug*/
    for(auto& objects : m_vecTextureData)
    {

        if(objects.bSelected)
        {
            gRenderer.SetColor(1.0f,1.0f,1.0f,0.7f);
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
            model = glm::rotate(model, glm::radians(objects.fAngle * -1), glm::vec3((float)m_bRotateAroundAxisX,
                                                                                    (float)m_bRotateAroundAxisY,
                                                                                    (float)m_bRotateAroundAxisZ));
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
                                   m_bRotateAroundAxisX,
                                   m_bRotateAroundAxisY,
                                   m_bRotateAroundAxisZ,
                                   m_vecShaders.at(objects.unShaderID));

        if(objects.bSelected && m_bEnableCut)
        {
            const float fXRect = objects.fX + (float)objects.nDynamicSourceX;
            const float fYRect = objects.fY + (float)objects.nDynamicSourceY;
            const float fWRect = objects.mTexture.GetWidth() - (objects.mTexture.GetWidth() - (float)objects.nDynamicSourceW);
            const float fHRect = objects.mTexture.GetHeight() - (objects.mTexture.GetHeight() - (float)objects.nDynamicSourceH);

            gRenderer.SetColor(1.0f,1.0f,1.0f,0.7f);
            gRenderer.DrawRect(fXRect, fYRect, fWRect, fHRect, shaderRect);
        }

        CurrentShaderEffect(objects.unShaderID);
    }

    gRenderer.SetColor(1.0f,1.0f,1.0f,1.0f);

    /*Animations from Debug*/
    if(!m_vecAnimData.empty())
    {
        unsigned int nFrame = gDebug.GetCurrFrameIndex();
        gRenderer.DrawPictureDebug(m_vecAnimData.at(nFrame).mTexture,
                                   m_vecAnimData.at(nFrame).fX,
                                   m_vecAnimData.at(nFrame).fY,
                                   m_vecAnimData.at(nFrame).fScaleFactor,
                                   m_vecAnimData.at(nFrame).fAngle,
                                   0.0f,
                                   m_bRotateAroundAxisX,
                                   m_bRotateAroundAxisY,
                                   m_bRotateAroundAxisZ,
                                   shaderTexture);

    }

}

void Debug::NewFrame()
{
    ImGui_ImplGlfwGL3_NewFrame();
}

void Debug::Destroy()
{
    ImGui::DestroyContext();
}

Debug gDebug;
