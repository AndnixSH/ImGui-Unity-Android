//
// Created by Askan on 10/22/2022.
//

#ifndef IMGUI_UNITY_TOUCH_MENU_HPP
#define IMGUI_UNITY_TOUCH_MENU_HPP

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <imgui.h>
#include <imgui_impl_android.h>
#include <imgui_impl_opengl3.h>
#include <unistd.h>
#include "Misc/log.h"

namespace MENU {
    int (*GlWidth)();
    int (*GlHeight)();

    bool clearMouse = true, setup = false, dark;

    bool open = true, updatePos;
    static ImVec2 iconPos;
    static ImVec2 mainPos;
    static float clickDuration;

    ImGuiWindowFlags wFlags =
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings;

    void Init();
    void SetStyle(bool bStyleDark_, float alpha_);
    void Menu(const char *windowName, bool *open);

    void Init() {
        LOGI(OFC("IMGUI: Setup ...\n H:%i W:%i"), GlHeight(), GlWidth());
        if (!GlHeight && !GlWidth) {
            sleep(2);
            Init();
        }

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;

        io.IniFilename = NULL;
        io.KeyMap[ImGuiKey_UpArrow] = 19;
        io.KeyMap[ImGuiKey_DownArrow] = 20;
        io.KeyMap[ImGuiKey_LeftArrow] = 21;
        io.KeyMap[ImGuiKey_RightArrow] = 22;
        io.KeyMap[ImGuiKey_Enter] = 66;
        io.KeyMap[ImGuiKey_Backspace] = 67;
        io.KeyMap[ImGuiKey_PageUp] = 92;
        io.KeyMap[ImGuiKey_PageDown] = 93;
        io.KeyMap[ImGuiKey_Escape] = 111;
        io.KeyMap[ImGuiKey_Delete] = 112;
        io.KeyMap[ImGuiKey_Home] = 122;
        io.KeyMap[ImGuiKey_End] = 123;
        io.KeyMap[ImGuiKey_Insert] = 124;
        io.KeyMap[ImGuiKey_UpArrow] = 19;
        io.KeyMap[ImGuiKey_DownArrow] = 20;
        io.KeyMap[ImGuiKey_LeftArrow] = 21;
        io.KeyMap[ImGuiKey_RightArrow] = 22;
        io.KeyMap[ImGuiKey_Enter] = 66;
        io.KeyMap[ImGuiKey_Backspace] = 67;
        io.KeyMap[ImGuiKey_PageUp] = 92;
        io.KeyMap[ImGuiKey_PageDown] = 93;
        io.KeyMap[ImGuiKey_Escape] = 111;
        io.KeyMap[ImGuiKey_Delete] = 112;
        io.KeyMap[ImGuiKey_Home] = 122;
        io.KeyMap[ImGuiKey_End] = 123;
        io.KeyMap[ImGuiKey_Insert] = 124;

        SetStyle(true, 0.8f);

        ImGui_ImplOpenGL3_Init(OFC("#version 300 es"));
        ImGui_ImplAndroid_Init(nullptr);
        ImFontConfig font_cfg;
        font_cfg.SizePixels = 24.0f;
        io.Fonts->AddFontDefault(&font_cfg);

        ImGui::GetStyle().ScaleAllSizes(3.0f);
        setup = true;
    }
    void SetStyle(bool bStyleDark_, float alpha_) {
        LOGI(OFC("IMGUI: Set Style ...\n dark: %i alpha: %.2f"), bStyleDark_, alpha_);

        ImGuiStyle &style = ImGui::GetStyle();

        // light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
        style.Alpha = 1.0f;
        style.FrameRounding = 3.0f;
        style.WindowRounding = 5.0f;
        style.FrameRounding = 2.3f;
        style.ScrollbarRounding = 5.0f;
        style.ChildRounding = 4.0f;

        style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

        if (bStyleDark_) {
            for (int i = 0; i <= ImGuiCol_COUNT; i++) {
                ImVec4 &col = style.Colors[i];
                float H, S, V;
                ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

                if (S < 0.1f) {
                    V = 1.0f - V;
                }
                ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
                if (col.w < 1.00f) {
                    col.w *= alpha_;
                }
            }
        } else {
            for (int i = 0; i <= ImGuiCol_COUNT; i++) {
                ImVec4 &col = style.Colors[i];
                if (col.w < 1.00f) {
                    col.x *= alpha_;
                    col.y *= alpha_;
                    col.z *= alpha_;
                    col.w *= alpha_;
                }
            }
        }
    }
    void TextCenter(ImColor color, const char* text, ...)
    {
        float windowWidth = ImGui::GetWindowSize().x;
        float textWidth = ImGui::CalcTextSize(text).x;
        va_list args;
        va_start(args, text);

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::TextColoredV(color, text, args);

        va_end(args);
    }

    float fov;
    void Menu(const char *windowName, bool isVertical = false) {
        ImGuiIO &io = ImGui::GetIO();
        if (open) {
            isVertical ? ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 1.25f, io.DisplaySize.y / 2.5f), ImGuiCond_Always) : ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 2.5f, io.DisplaySize.y / 1.15f), ImGuiCond_Always);
            ImGui::GetStyle().WindowPadding = ImVec2(0.8f,0.8f);
            if (updatePos) {
                ImGui::SetNextWindowPos(iconPos);
                updatePos = false;
            }

            ImGui::Begin(windowName, nullptr, wFlags);
            TextCenter(ImColor(255,255,0), OFC("Mod By Askan"));

            ImGui::SliderFloat(OFC("FOV"), &fov, 20.0f, 200.0f, OFC("%.3f"), 0);
            if (ImGui::Button(OFC("Set FOV"), ImVec2(ImGui::GetContentRegionAvail().x - 1.0f, ImGui::GetContentRegionAvail().y / 6.0f))) {
                LOGI(OFC("SCREEN: \n W: %i H: %i"), MENU::GlWidth(), MENU::GlHeight());
            }


            if (ImGui::Button(OFC("Close"), ImVec2(ImGui::GetContentRegionAvail().x/4.0f, 50.0f))) {
                mainPos = ImGui::GetMousePos();
                open = false;
                updatePos = true;
            }
            ImGui::End();
        } else {
            float size = isVertical ? io.DisplaySize.y / 12.5f : io.DisplaySize.x / 12.5f;
            ImGui::SetNextWindowSize(ImVec2(size, size), ImGuiCond_Always);
            ImGui::GetStyle().WindowPadding = ImVec2(0.0f,0.0f);
            if (updatePos) {
                ImGui::SetNextWindowPos(mainPos);
                updatePos = false;
            }

            int m_downCount = IM_ARRAYSIZE(io.MouseDown);

            ImGui::Begin(OFC("Mod Icon"), nullptr,
                         ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoSavedSettings);

            if (ImGui::Button(OFC("Mod"), ImGui::GetContentRegionAvail())) {
                if (clickDuration > 0.3f) goto skip;

                iconPos = ImGui::GetWindowPos();
                open = true;
                updatePos = true;

                skip:;
            }
            if (ImGui::IsItemActive()) {
                for (int i = 0; i < m_downCount; i++) {
                    if (ImGui::IsMouseDown(i))  {
                        if  (io.MouseDownDuration[i] > 0.3f)
                                ImGui::SetWindowPos(
                                        ImVec2(ImGui::GetMousePos().x - ImGui::GetWindowSize().x / 2,
                                               ImGui::GetMousePos().y - ImGui::GetWindowSize().y / 2), 0);

                        clickDuration = io.MouseDownDuration[i];
                    }
                }
            }

            ImGui::End();
        }
    }
}


#endif //IMGUI_UNITY_TOUCH_MENU_HPP
