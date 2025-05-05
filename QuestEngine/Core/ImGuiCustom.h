// ImGuiColorWheel.h
#pragma once
#include "imgui.h"
#include "../Math/Vector4D.h"
#include "../Math/Vector3D.h"
#include "../Math/Vector2D.h"
#include "../Math/Mathf.h"
#include "ColorManagement/HSVModel.h"
#include "ColorManagement/RGBModel.h"
#include "ColorRGBY.h"
#include <cmath>
#include <string>
namespace ImGuiCustom {

    static std::unordered_map<std::string, bool> wheelState;
    static std::unordered_map<std::string, ColorRGBY> wheelColor;
    static std::unordered_map<std::string, bool> multiplierSliderState;


    bool ColorWheelWithSlider(const char* label, ImVec2 avail, int maxGroupCount, ColorRGBY& outColor, float radius, Texture2D* texture, float minRange = -1.0f, float maxRange = 1.0f)
    {
        if (wheelColor.find(label) == wheelColor.end())
            wheelColor[label] = outColor;

        if (multiplierSliderState.find(label) == multiplierSliderState.end())
            multiplierSliderState[label] = false;


        ImGui::BeginGroup();
        float headerHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImGui::Dummy(ImVec2(0.0f, headerHeight)); // réserve l’espace pour tous les groupes
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - headerHeight); // revient pour dessiner le vrai texte
        ImGui::Text("%s", label);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        float totalWidth = (avail.x - ImGui::GetStyle().ItemSpacing.x * (maxGroupCount - 1)) / maxGroupCount;

        float wheelSize = radius * 2.0f;

        // 👇 Calcule le décalage à gauche pour centrer
        float leftPanelX = ImGui::GetCursorScreenPos().x + (totalWidth - wheelSize) * 0.5f;

        // Positionne le curseur ImGui à la position du cercle centré
        ImGui::SetCursorScreenPos(ImVec2(leftPanelX, ImGui::GetCursorScreenPos().y));

        // ⬇️ Mise à jour correcte de center et placement de l'image
        ImVec2 circleScreen = ImGui::GetCursorScreenPos();
        Vector2D center = Vector2D(circleScreen.x + radius, circleScreen.y + radius);

        ImTextureID texID = (ImTextureID)(intptr_t)texture->GetTextureLayerInfo().m_textureID;
        draw_list->AddImage(texID, circleScreen, ImVec2(circleScreen.x + wheelSize, circleScreen.y + wheelSize));

        // Invisible button pour interagir
        ImGui::InvisibleButton(label, ImVec2(wheelSize, wheelSize));

        std::string id = std::string(label);  // unique ID par roue
        bool& wheel_active = wheelState[id]; // référence sur le bool lié à ce label

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            wheel_active = true;

        if (!ImGui::IsMouseDown(0))
            wheel_active = false;
        //map range to 0 -> 1
        Vector3D rgb = Vector3D(outColor.m_r, outColor.m_g, outColor.m_b);
        rgb.m_x = (outColor.m_r - minRange) / (maxRange - minRange);
        rgb.m_y = (outColor.m_g - minRange) / (maxRange - minRange);
        rgb.m_z = (outColor.m_b - minRange) / (maxRange - minRange);

        if (wheel_active)
        {
            //Compute the new color here
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            float dx = mousePos.x - center.m_x;
            float dy = mousePos.y - center.m_y;
            float dist = sqrtf(dx * dx + dy * dy);
            float angle = atan2f(dy, dx);
            if (angle < 0) angle += 2.0f * Mathf::PI;

            float hue = angle / (2.0f * Mathf::PI);
            float sat = Mathf::Clamp(dist / radius, 0.0f, 1.0f);

            Vector3D hsv = RGBModel::ToHSV(rgb);
           
            Vector3D rgbl = HSVModel::ToRGB(Vector3D(hue, sat, 1.0f));

            // 0.5 0.2 0.2 
            // 0.8, 1, 0.2 //new color 
            //

            rgb = rgbl * Mathf::Max(rgb.m_x, rgb.m_y, rgb.m_z);
            //map range to minrange - > maxRange
            outColor.m_r = minRange + rgb.m_x * (maxRange - minRange);
            outColor.m_g = minRange + rgb.m_y * (maxRange - minRange);
            outColor.m_b = minRange + rgb.m_z * (maxRange - minRange);
        }
        
        //Display the cursor here

        Vector3D hsv = RGBModel::ToHSV(rgb);

        float angleCursor = hsv.m_x * 2.0f * Mathf::PI;
        float distCursor = hsv.m_y * radius;
        Vector2D cursorPos = center + Vector2D(cosf(angleCursor), sinf(angleCursor)) * distCursor;
        draw_list->AddCircle(ImVec2(cursorPos.m_x, cursorPos.m_y), 5.0f, IM_COL32(255, 255, 255, 255), 12, 2.0f);
        float halfWidth = ((totalWidth) - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
     
        ImGui::PushID("M");
        std::string sliderID = "##" + std::string(label) + "_M_slider";
        ImGui::PushItemWidth(totalWidth);

        float invRange = 1.0f / (maxRange - minRange);
        ColorRGBY inputColor = multiplierSliderState[label] == true ? wheelColor[label] : outColor;

        float multiplier = (Mathf::Max(inputColor.m_r, inputColor.m_g, inputColor.m_b) - minRange) * invRange;
        float newMultiplier = multiplier * 2.0f;

        bool state = ImGui::SliderFloat(sliderID.c_str(), &newMultiplier, 0.0f, 2.0f, "%.3f");

        if (state)
        {
            multiplierSliderState[label] = true;
            if (multiplier > 0.01f)
            {
                float ratio = (newMultiplier * 0.5f) / multiplier;

                inputColor.m_r = minRange + ((inputColor.m_r - minRange) * invRange * ratio) * (maxRange - minRange);
                inputColor.m_g = minRange + ((inputColor.m_g - minRange) * invRange * ratio) * (maxRange - minRange);
                inputColor.m_b = minRange + ((inputColor.m_b - minRange) * invRange * ratio) * (maxRange - minRange);

                outColor = inputColor;
            }
        }

        if (multiplierSliderState[label] == true && state == false)
            multiplierSliderState[label] = false;

        if(multiplierSliderState[label] == false)
            wheelColor[label] = outColor;


        ImGui::PopItemWidth();
        ImGui::PopID();

        // R
        ImGui::PushID("R");
        std::string rSliderID = "##" + std::string(label) + "_R_slider";
        std::string rInputID = "##" + std::string(label) + "_R_input";
        ImGui::PushItemWidth(halfWidth);

        ImGui::SliderFloat(rSliderID.c_str(), &outColor.m_r, -1.0f, 1.0f, "%.3f");
        ImGui::SameLine();
        ImGui::PushItemWidth(halfWidth);
        ImGui::InputFloat(rInputID.c_str(), &outColor.m_r, 0.01f, 0.1f, "%.3f");
        ImGui::PopItemWidth(); ImGui::PopItemWidth();
        ImGui::PopID();

        // G
        ImGui::PushID("G");
        std::string gSliderID = "##" + std::string(label) + "_G_slider";
        std::string gInputID = "##" + std::string(label) + "_G_input";
        ImGui::PushItemWidth(halfWidth);
        ImGui::SliderFloat(gSliderID.c_str(), &outColor.m_g, -1.0f, 1.0f, "%.3f");
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) outColor.m_g = 0.0f;
        ImGui::SameLine();
        ImGui::PushItemWidth(halfWidth);
        ImGui::InputFloat(gInputID.c_str(), &outColor.m_g, 0.01f, 0.1f, "%.3f");
        ImGui::PopItemWidth(); ImGui::PopItemWidth();
        ImGui::PopID();

        // B
        ImGui::PushID("B");
        std::string bSliderID = "##" + std::string(label) + "_B_slider";
        std::string bInputID = "##" + std::string(label) + "_B_input";
        ImGui::PushItemWidth(halfWidth);
        ImGui::SliderFloat(bSliderID.c_str(), &outColor.m_b, -1.0f, 1.0f, "%.3f");
        ImGui::SameLine();
        ImGui::PushItemWidth(halfWidth);
        ImGui::InputFloat(bInputID.c_str(), &outColor.m_b, 0.01f, 0.1f, "%.3f");
        ImGui::PopItemWidth(); ImGui::PopItemWidth();
        ImGui::PopID();

        // Y
        ImGui::PushID("Y");
        std::string ySliderID = "##" + std::string(label) + "_Y_slider";
        std::string yInputID = "##" + std::string(label) + "_Y_input";
        ImGui::PushItemWidth(halfWidth);
        ImGui::SliderFloat(ySliderID.c_str(), &outColor.m_y, 0.0f, 1.0f, "%.3f");
        ImGui::SameLine();
        ImGui::PushItemWidth(halfWidth);
        ImGui::InputFloat(yInputID.c_str(), &outColor.m_y, 0.01f, 0.1f, "%.3f");
        ImGui::PopItemWidth(); ImGui::PopItemWidth();
        ImGui::PopID();
        ImGui::EndGroup();
       
        return wheel_active;
    }

    static void DisplaySeparator()
    {
        // Ligne blanche de séparation
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 lineStart = ImGui::GetCursorScreenPos();
        float lineHeight = 2 * 80.0f + ImGui::GetTextLineHeightWithSpacing() + 4 * ImGui::GetFrameHeight();
        float lineX = lineStart.x - ImGui::GetStyle().ItemSpacing.x * 0.5f;
        draw_list->AddLine(ImVec2(lineX, lineStart.y), ImVec2(lineX, lineStart.y + lineHeight), IM_COL32(255, 255, 255, 100), 1.0f);
    }
} 
