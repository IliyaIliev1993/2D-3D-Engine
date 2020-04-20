#include "Debug.h"
#include "../renderer/Renderer.h"

Debug::Debug()
{

}

bool Debug::Init()
{
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(gRenderer.GetWindow(), true);
    ImGui::StyleColorsDark();

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
            m_vecTextureData.at(i).bSelected = !m_vecTextureData.at(i).bSelected;
        }

        if(m_vecTextureData.at(i).bSelected)
        {
            m_vecTextureData.at(i).fX = g_fXCoord;
            m_vecTextureData.at(i).fY = g_fYCoord;
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
                m_vecAnimData.at(i).bSelected = !m_vecAnimData.at(i).bSelected;
            }
        }

        if(m_vecAnimData.at(0).bSelected)
        {
            for(unsigned int i = 0; i < m_vecAnimData.size(); i++)
            {
                m_vecAnimData.at(i).fX = g_fXCoord;
                m_vecAnimData.at(i).fY = g_fYCoord;
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
        ImGui::Text("Animation Path, need frame without indexing number:");
        ImGui::InputText("",m_cArrSymbolsAnim,m_unBuffSymbols);

        if(ImGui::TreeNode("Anim Settings"))
        {
            ImGui::Text("Frame Begin, Count of Frames, FPS:");
            ImGui::InputInt3("", m_nAnimOption);
            // 0 - Begin, 1 - Count, 2 - FPS
            if(m_nAnimOption[1] != 0 && m_nAnimOption[2] != 0)
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
                    }

                    for(unsigned int i = 0; i < m_nAnimOption[1]; i++)
                    {
                        TextureDataDebug mData;
                        std::string sIndex = "";
                        if(m_nAnimOption[1] >= 100)
                        {
                            if(i + m_nAnimOption[0] <= 9)
                            {
                                sIndex = "00" + std::to_string(i + m_nAnimOption[0]);
                            }
                            else if(i + m_nAnimOption[0] >= 10 && i + m_nAnimOption[0] < 100)
                            {
                                sIndex = "0" + std::to_string(i + m_nAnimOption[0]);
                            }
                            else if(i + m_nAnimOption[0] >= 100)
                            {
                                sIndex = std::to_string(i + m_nAnimOption[0]);
                            }

                        }
                        else if(m_nAnimOption[1] < 100)
                        {
                            if(i + m_nAnimOption[0] <= 9)
                            {
                                sIndex = "0" + std::to_string(i + m_nAnimOption[0]);
                            }
                            else if(i + m_nAnimOption[0] >= 10)
                            {
                                sIndex = std::to_string(i + m_nAnimOption[0]);
                            }

                        }
                        std::string sPath = m_cArrSymbolsAnim + sIndex + ".png";

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
                        m_timerAnim.Start(1, (int)(1000 / m_nAnimOption[2]));
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
                        m_timerAnim.Start(1, (int)(1000 / m_nAnimOption[2]));
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
                     " x=\"0\" y=\"0\" width=" + "\"" + std::to_string(m_vecTextureData.at(i).mTexture.GetWidth()) + "\"" +
                     " height=" + "\"" + std::to_string(m_vecTextureData.at(i).mTexture.GetHeight()) + "\"" +
                     " priority=\"200\"/>\n";

        }
        sData += "</game_rss_list>\n";
        sData += "DEST: \n";

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

    m_timerAnim.Tick(callbackFrame);
}

void Debug::MainWindow()
{
    ImGui::Text("Texture Debug");
    ImGui::SameLine();
    ImGui::Text("RENDERING %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SameLine();
    ImGui::Text("Resolution: %d x %d", gRenderer.SCREEN_WIDTH, gRenderer.SCREEN_HEIGHT);
    ImGui::SliderFloat("X Coords", &g_fXCoord, 0.0f, (float)gRenderer.SCREEN_WIDTH);
    ImGui::SliderFloat("Y Coords", &g_fYCoord, 0.0f, (float)gRenderer.SCREEN_HEIGHT);
    ImGui::InputFloat("X Precision", &g_fXCoord, 1.0f);
    ImGui::InputFloat("Y Precision", &g_fYCoord, 1.0f);
    ImGui::ColorEdit4("Texture color", (float*)&m_vec4Color);

    ImGui::Checkbox("Texture Options", &m_bCreateTexture);
    ImGui::SameLine();
    ImGui::Checkbox("Animation Options", &m_bCreateAnimation);
    ImGui::SameLine();
    ImGui::Checkbox("Imgui HELP", &m_bShowImguiHelp);
    ImGui::SameLine();
}

void Debug::Process()
{

    /*Timer Process call function*/
    TimerProcess();

    /*Main Window*/
    MainWindow();

    /*Write into txt file all the data*/
    WriteIntoFile();

    /*Selectable Radio Button Textures*/
    RadioButtonTextures();

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

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
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
