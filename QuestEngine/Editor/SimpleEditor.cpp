#include "SimpleEditor.h"
#include "imgui.h"
#include "../Core/World.h"
#include "../Core/Components/PostProcessingVolume.h"
#include "../Core/PostProcessing/ColorGrading.h"
#include "../Core/PostProcessing/BloomEffect.h"
#include "../Core/PostProcessing/Bloom2Effect.h"
#include "../Core/PostProcessing/Bloom3Effect.h"
#include "../Core/ImGuiCustom.h"
SimpleEditor* SimpleEditor::s_instance = nullptr;

SimpleEditor::SimpleEditor()
{
    m_colorWheelTexture = AssetsManager::CreateTexture2D("ColorWheelTexture", "Assets/ColorWheel.png");
}

SimpleEditor::~SimpleEditor()
{
   
}

SimpleEditor* SimpleEditor::Instance()
{
    if (!s_instance)
        s_instance = new SimpleEditor();
    return s_instance;
}

void SimpleEditor::Destroy()
{
    delete s_instance;
    s_instance = nullptr;
}

void SimpleEditor::UpdateLUTList() {
    m_availableLUTs.clear();
    auto allTextures = AssetsManager::GetsAssets<Texture2D>();
    for (auto* tex : allTextures) {
        m_availableLUTs.push_back(tex);
    }
    if (!m_availableLUTs.empty() && m_selectedLUTIndex >= m_availableLUTs.size()) {
        m_selectedLUTIndex = 0;
    }
}


void SimpleEditor::Display()
{

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0.0f, displaySize.y), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, 0.0f), ImGuiCond_Always);

    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    ImVec2 startCursorPos = ImGui::GetCursorScreenPos(); 

    static int workspaceIndex = static_cast<int>(ColorManagement::Instance().GetCurrentWorkingSpaceType());
    const char* workspaceLabels[] = { "sRGB", "Linear sRGB", "ACEScc", "ACEScct", "ACEScg" };

    if (ImGui::Combo("Working Space", &workspaceIndex, workspaceLabels, IM_ARRAYSIZE(workspaceLabels))) {
        ColorManagement::Instance().SetWorkingSpace(static_cast<ColorManagement::ColorSpaceType>(workspaceIndex));
    }
    ImGui::Separator();

    static int gpuWorkspaceIndex = static_cast<int>(ColorManagement::Instance().GetCurrentGPUWorkingSpaceType());

    if (ImGui::Combo("GPU Working Space", &gpuWorkspaceIndex, workspaceLabels, IM_ARRAYSIZE(workspaceLabels))) {
        ColorManagement::Instance().SetGPUWorkingSpace(static_cast<ColorManagement::ColorSpaceType>(gpuWorkspaceIndex));
    }
    ImGui::Separator();

    auto volumes = World::Instance()->GetComponentsOfType<PostProcessingVolume>();

    for (size_t i = 0; i < volumes.size(); ++i) {
        PostProcessingVolume* volume = volumes[i];
        if (!volume) continue;

        std::string header = "PostProcessingVolume ##" + std::to_string(i);
        if (ImGui::CollapsingHeader(header.c_str())) {
            bool isGlobal = volume->IsGlobal();
            if (ImGui::Checkbox("Global", &isGlobal))
                volume->SetGlobal(isGlobal);

            auto& effects = volume->GetEffects();
            for (size_t j = 0; j < effects.size(); ++j) {
                auto& effect = effects[j];
                if (!effect) continue;


                std::string effectName = std::string(effect->GetTypeName()) + " " + std::to_string(j);
                if (ImGui::CollapsingHeader(effectName.c_str()))
                {
                    if (auto cg = std::dynamic_pointer_cast<ColorGradingSettings>(effect)) {

                        ImVec2 avail = ImGui::GetContentRegionAvail();


                        ImGui::Checkbox("Use LUT2D", &cg->useLUT);
                        ImGui::Separator();

                        if (cg->useLUT)
                        {
                            // --- LUT selection ---
                            UpdateLUTList();
                            if (!m_availableLUTs.empty()) {
                                static std::vector<std::string> nameStorage;
                                nameStorage.clear();
                                std::vector<const char*> names;

                                for (auto* tex : m_availableLUTs) {
                                    nameStorage.push_back(tex->GetAssetName());
                                    names.push_back(nameStorage.back().c_str());
                                }
                                ImGui::Combo("LUT Texture", &m_selectedLUTIndex, names.data(), static_cast<int>(names.size()));
                            }

                            Texture2D* lutTexture = (m_selectedLUTIndex >= 0 && m_selectedLUTIndex < m_availableLUTs.size()) ? m_availableLUTs[m_selectedLUTIndex] : nullptr;
                            cg->m_lutTexture = lutTexture;

                            ImGui::InputFloat("LUT Size", &cg->lutSize, 1.0f, 10.0f, "%.0f");
                        }
                        else
                        {
                            const char* gradingModes[] = { "Global", "Shadows", "Midtones", "Highlights" };
                            ImGui::Combo("Grading Mode", &gradingMode, gradingModes, IM_ARRAYSIZE(gradingModes));
                            ImGui::Separator();

                            ColorGradingSettings::GradingParams* currentGrading = nullptr;
                            switch (gradingMode)
                            {
                            case 0: currentGrading = &cg->global; break;
                            case 1: currentGrading = &cg->shadow; break;
                            case 2: currentGrading = &cg->midtone; break;
                            case 3: currentGrading = &cg->highlight; break;
                            }

                            if (currentGrading)
                            {
                                if (currentGrading != &cg->global)
                                {
                                    if (ImGui::BeginTable("GradingControls", 3, ImGuiTableFlags_SizingStretchSame))
                                    {
                                        ImGui::TableNextRow();

                                        ImGui::TableSetColumnIndex(0);
                                        ImGui::Text("HueShift");
                                        ImGui::SliderFloat("##HueShift", &currentGrading->HueShift, -1.0f, 1.0f, "%.3f");

                                        ImGui::TableSetColumnIndex(1);
                                        ImGui::Text("MinRange");
                                        ImGui::SliderFloat("##MinRange", &currentGrading->MinRange, 0.0f, 1.0f, "%.3f");

                                        ImGui::TableSetColumnIndex(2);
                                        ImGui::Text("MaxRange");
                                        ImGui::SliderFloat("##MaxRange", &currentGrading->MaxRange, 0.0f, 1.0f, "%.3f");

                                        ImGui::EndTable();
                                    }
                                }
                                else
                                {
                                    if (ImGui::BeginTable("GradingControls", 4, ImGuiTableFlags_SizingStretchSame))
                                    {
                                        ImGui::TableNextRow();


                                        ImGui::TableSetColumnIndex(0);
                                        ImGui::Text("Exposure");
                                        ImGui::SliderFloat("##Exposure", &cg->exposure, -15, 15.0f, "%.3f");

                                        ImGui::TableSetColumnIndex(1);
                                        ImGui::Text("HueShift");
                                        ImGui::SliderFloat("##HueShift", &currentGrading->HueShift, -1.0f, 1.0f, "%.3f");

                                        ImGui::TableSetColumnIndex(2);
                                        ImGui::Text("TemperatureTint");
                                        ImGui::SliderFloat("##TemperatureTint", &cg->temperatureTint, -100.0f, 100.0f, "%.3f");

                                        ImGui::TableSetColumnIndex(3);
                                        ImGui::Text("Temperature");
                                        ImGui::SliderFloat("##Temperature", &cg->temperature, -100, 100, "%.3f");

                                        ImGui::EndTable();
                                    }
                                }



                                ImGui::Spacing();
                                ImGui::Separator();
                                ImGui::Spacing();

                                ImGuiCustom::ColorWheelWithSlider("Saturation", avail, 8, currentGrading->Saturation, 60.0f, m_colorWheelTexture);
                                ImGui::SameLine();
                                ImGuiCustom::ColorWheelWithSlider("Contrast", avail, 8, currentGrading->Contrast, 60.0f, m_colorWheelTexture);
                                ImGui::SameLine();
                                ImGuiCustom::ColorWheelWithSlider("Lift", avail, 8, currentGrading->Lift, 60.0f, m_colorWheelTexture);
                                ImGui::SameLine();
                                ImGuiCustom::ColorWheelWithSlider("Midtone", avail, 8, currentGrading->Midtone, 60.0f, m_colorWheelTexture);
                                ImGui::SameLine();
                                ImGuiCustom::ColorWheelWithSlider("Gain", avail, 8, currentGrading->Gain, 60.0f, m_colorWheelTexture);
                                ImGui::SameLine();
                                ImGuiCustom::ColorWheelWithSlider("Gamma", avail, 8, currentGrading->Gamma, 60.0f, m_colorWheelTexture);
                                ImGui::SameLine();
                                ImGuiCustom::ColorWheelWithSlider("Offset", avail, 8, currentGrading->Offset, 60.0f, m_colorWheelTexture);
                                ImGui::SameLine();
                                ImGuiCustom::ColorWheelWithSlider("Tint", avail, 8, currentGrading->Tint, 60.0f, m_colorWheelTexture);
                            }
                        }




                    }
                    else if (auto bloom = std::dynamic_pointer_cast<BloomSettings>(effect))
                    {
                        ImGui::InputFloat("Threshold", &bloom->threshold, 1.0f, 10.0f, "%.2f");
                        ImGui::InputFloat("Knee", &bloom->knee, 1.0f, 10.0f, "%.2f");
                        ImGui::InputFloat("Intensity", &bloom->intensity, 1.0f, 10.0f, "%.2f");

                        ImGui::SliderFloat("Gaussian Blur Radius", &bloom->radius, 1.0f, 500.0f, "%.0f");
                    }
                    else if (auto bloom2 = std::dynamic_pointer_cast<Bloom2Settings>(effect))
                    {
                        ImGui::InputFloat("Threshold", &bloom2->threshold, 1.0f, 10.0f, "%.2f");
                        ImGui::InputFloat("Knee", &bloom2->knee, 1.0f, 10.0f, "%.2f");
                        ImGui::InputFloat("Intensity", &bloom2->intensity, 1.0f, 10.0f, "%.2f");

                        ImGui::SliderInt("Additive Blur Count", &bloom2->additiveBlurCount, 0, Bloom2Effect::additiveBlurMaxCount);
                    }
                    else if (auto bloom3 = std::dynamic_pointer_cast<Bloom3Settings>(effect))
                    {
                        ImGui::InputFloat("Threshold", &bloom3->threshold, 1.0f, 10.0f, "%.2f");
                        ImGui::InputFloat("Knee", &bloom3->knee, 1.0f, 10.0f, "%.2f");
                        ImGui::InputFloat("Intensity", &bloom3->intensity, 1.0f, 10.0f, "%.2f");

                        ImGui::SliderInt("Additive Blur Count", &bloom3->additiveBlurCount, 0, Bloom3Effect::additiveBlurMaxCount);
                    }
                }
            }
        }      
    }
    ImVec2 endCursorPos = ImGui::GetCursorScreenPos(); // Après ton contenu

    ImGui::End(); // Ferme la fenêtre

    // maintenant on peut obtenir la hauteur réelle de la fenêtre
    ImVec2 editorSize = ImGui::GetWindowSize();
    float screenHeight = displaySize.y;
    float editorHeight = endCursorPos.y - startCursorPos.y;
    std::vector<CameraComponent*> cameraComponents = World::Instance()->GetComponentsOfType<CameraComponent>();

    CameraComponent* main = cameraComponents[0]->GetRenderTexture() == nullptr ? cameraComponents[0] : cameraComponents[1];

    if (!cameraComponents.empty()) {

        main->m_viewportBottomCornerX = 0.0f;
        main->m_viewportBottomCornerY = (editorHeight / screenHeight);
        main->m_viewportTopCornerX = 1.0f;
        main->m_viewportTopCornerY = 1.0f;
    }

}