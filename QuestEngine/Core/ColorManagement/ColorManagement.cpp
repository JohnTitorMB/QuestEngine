#include "ColorManagement.h"
#include "sRGBTransferCurve.h"
#include "LinearTransferCurve.h"
#include "ACESccTransferCurve.h"
#include "ACEScctTransferCurve.h"

std::shared_ptr<RGBColorSpace> ColorManagement::m_srgb = nullptr;
std::shared_ptr<RGBColorSpace> ColorManagement::m_linearSRGB = nullptr;
std::shared_ptr<RGBColorSpace> ColorManagement::m_acescc = nullptr;
std::shared_ptr<RGBColorSpace> ColorManagement::m_acescct = nullptr;
std::shared_ptr<RGBColorSpace> ColorManagement::m_linearACES = nullptr;
std::shared_ptr<CIEXYZColorSpace> ColorManagement::m_xyz = nullptr;
std::shared_ptr<LMSColorSpace> ColorManagement::m_lms = nullptr;
ColorManagement::ColorSpaceType ColorManagement::m_currentType = ColorManagement::ColorSpaceType::LinearSRGB;
ColorManagement::ColorSpaceType ColorManagement::m_gpuCurrentType = ColorManagement::ColorSpaceType::LinearSRGB;
ColorManagement::ColorSpaceType ColorManagement::m_mainFBCurrentType = ColorManagement::ColorSpaceType::SRGB;

ColorManagement& ColorManagement::Instance() {
    static ColorManagement instance;
    return instance;
}

ColorManagement::ColorManagement() {
    Initialize();
    if (!m_workingSpace) {
        SetWorkingSpace(m_linearSRGB);
    }

    if (!m_gpuWorkingSpace) {
        SetGPUWorkingSpace(m_linearSRGB);
    }

    if (!m_mainFBWorkingSpace)
    {
        SetMainFBWorkingSpace(m_srgb);
    }
}

void ColorManagement::Initialize() {
    if (!m_srgb) {
        auto srgbCurve = std::make_shared<sRGBTransferCurve>();
        Gamut srgbGamut({ 0.64f, 0.33f }, { 0.30f, 0.60f }, { 0.15f, 0.06f }, { 0.3127f, 0.3290f });
        m_srgb = std::make_shared<RGBColorSpace>("sRGB", srgbGamut, srgbCurve, srgbCurve);
    }

    if (!m_linearSRGB) {
        auto linearCurve = std::make_shared<LinearTransferCurve>();
        m_linearSRGB = std::make_shared<RGBColorSpace>("Linear sRGB", m_srgb->GetGamut(), linearCurve, linearCurve);
    }

    if (!m_acescc) {
        auto acesccCurve = std::make_shared<ACESccTransferCurve>();
        Gamut acesGamut({ 0.7347f, 0.2653f }, { 0.0f, 1.0f }, { 0.0001f, -0.077f }, { 0.32168f, 0.33767f });
        m_acescc = std::make_shared<RGBColorSpace>("ACEScc", acesGamut, acesccCurve, acesccCurve);
    }

    if (!m_acescct) {
        auto acescctCurve = std::make_shared<ACEScctTransferCurve>();
        Gamut acesGamut({ 0.7347f, 0.2653f }, { 0.0f, 1.0f }, { 0.0001f, -0.077f }, { 0.32168f, 0.33767f });
        m_acescct = std::make_shared<RGBColorSpace>("ACEScct", acesGamut, acescctCurve, acescctCurve);
    }

    if (!m_linearACES) {
        auto linearCurve = std::make_shared<LinearTransferCurve>();
        Gamut acesGamut({ 0.713f, 0.293f }, { 0.165f, 0.830f }, { 0.128f, 0.044f }, { 0.32168f, 0.33767f }); 
        m_linearACES = std::make_shared<RGBColorSpace>("Linear ACEScg", acesGamut, linearCurve, linearCurve);
    }

    if (!m_xyz) {
        m_xyz = std::make_shared<CIEXYZColorSpace>();
    }

    if (!m_lms) {
        m_lms = std::make_shared<LMSColorSpace>();
    }
}

std::shared_ptr<RGBColorSpace> ColorManagement::SRGB() {
    Initialize();
    return m_srgb;
}

std::shared_ptr<RGBColorSpace> ColorManagement::LinearSRGB() {
    Initialize();
    return m_linearSRGB;
}

std::shared_ptr<RGBColorSpace> ColorManagement::ACEScc() {
    Initialize();
    return m_acescc;
}

std::shared_ptr<RGBColorSpace> ColorManagement::ACEScct() {
    Initialize();
    return m_acescct;
}

std::shared_ptr<RGBColorSpace> ColorManagement::LinearACES() {
    Initialize();
    return m_linearACES;
}

std::shared_ptr<CIEXYZColorSpace> ColorManagement::XYZ() {
    Initialize();
    return m_xyz;
}

std::shared_ptr<LMSColorSpace> ColorManagement::LMS() {
    Initialize();
    return m_lms;
}

void ColorManagement::SetWorkingSpace(const std::shared_ptr<RGBColorSpace>& space) {
    m_workingSpace = space;

    auto linearCurve = std::make_shared<LinearTransferCurve>();
    m_workingLinearSpace = std::make_shared<RGBColorSpace>(
        m_workingSpace->GetName() + " (Linear)",
        m_workingSpace->GetGamut(),
        linearCurve,
        linearCurve
    );
}

void ColorManagement::SetWorkingSpace(ColorSpaceType type)
{
    m_currentType = type;
    ColorManagement& instance = Instance();
    switch (type) {
    case ColorSpaceType::SRGB:
        instance.SetWorkingSpace(SRGB());
        break;
    case ColorSpaceType::LinearSRGB:
        instance.SetWorkingSpace(LinearSRGB());
        break;
    case ColorSpaceType::ACEScc:
        instance.SetWorkingSpace(ACEScc());
        break;
    case ColorSpaceType::ACEScct:
        instance.SetWorkingSpace(ACEScct());
        break;
    case ColorSpaceType::LinearACEScg:
        instance.SetWorkingSpace(LinearACES());
        break;
    }
}

void ColorManagement::SetGPUWorkingSpace(const std::shared_ptr<RGBColorSpace>& space) {
    m_gpuWorkingSpace = space;
}

void ColorManagement::SetGPUWorkingSpace(ColorSpaceType type)
{
    m_gpuCurrentType = type;
    ColorManagement& instance = Instance();
    switch (type) {
    case ColorSpaceType::SRGB:
        instance.SetGPUWorkingSpace(SRGB());
        break;
    case ColorSpaceType::LinearSRGB:
        instance.SetGPUWorkingSpace(LinearSRGB());
        break;
    case ColorSpaceType::ACEScc:
        instance.SetGPUWorkingSpace(ACEScc());
        break;
    case ColorSpaceType::ACEScct:
        instance.SetGPUWorkingSpace(ACEScct());
        break;
    case ColorSpaceType::LinearACEScg:
        instance.SetGPUWorkingSpace(LinearACES());
        break;
    }
}

void ColorManagement::SetMainFBWorkingSpace(const std::shared_ptr<RGBColorSpace>& space) {
    m_mainFBWorkingSpace = space;
}

void ColorManagement::SetMainFBWorkingSpace(ColorSpaceType type)
{
    m_mainFBCurrentType = type;
    ColorManagement& instance = Instance();
    switch (type) {
    case ColorSpaceType::SRGB:
        instance.SetGPUWorkingSpace(SRGB());
        break;
    case ColorSpaceType::LinearSRGB:
        instance.SetGPUWorkingSpace(LinearSRGB());
        break;
    case ColorSpaceType::ACEScc:
        instance.SetGPUWorkingSpace(ACEScc());
        break;
    case ColorSpaceType::ACEScct:
        instance.SetGPUWorkingSpace(ACEScct());
        break;
    case ColorSpaceType::LinearACEScg:
        instance.SetGPUWorkingSpace(LinearACES());
        break;
    }
}

const std::shared_ptr<RGBColorSpace> ColorManagement::GetWorkingSpace() const {
    return m_workingSpace;
}

const std::shared_ptr<RGBColorSpace> ColorManagement::GetWorkingLinearSpace() const {
    return m_workingLinearSpace;
}

ColorManagement::ColorSpaceType ColorManagement::GetCurrentWorkingSpaceType() {
    return m_currentType;
}

const std::shared_ptr<RGBColorSpace> ColorManagement::GetGPUWorkingSpace() const
{
    return m_gpuWorkingSpace;
}

ColorManagement::ColorSpaceType ColorManagement::GetCurrentGPUWorkingSpaceType()
{
    return m_gpuCurrentType;
}

const std::shared_ptr<RGBColorSpace> ColorManagement::GetMainFBWorkingSpace() const
{
    return m_mainFBWorkingSpace;
}

ColorManagement::ColorSpaceType ColorManagement::GetMainFBWorkingSpaceType()
{
    return m_mainFBCurrentType;
}

std::shared_ptr<RGBColorSpace> ColorManagement::GetRGBColorSpaceFromType(RGBColorSpaceType type)
{
    Initialize();

    switch (type)
    {
        case RGBColorSpaceType::SRGB: return SRGB();
        case RGBColorSpaceType::LinearSRGB: return LinearSRGB();
        case RGBColorSpaceType::ACEScc: return ACEScc();
        case RGBColorSpaceType::ACEScct: return ACEScct();
        case RGBColorSpaceType::LinearACEScg: return LinearACES();
        case RGBColorSpaceType::WorkingSpace: return Instance().GetWorkingSpace();
        default: throw std::runtime_error("Unknown ColorSpaceType");
    }
}

int ColorManagement::RGBColorSpaceGPUIndex(RGBColorSpaceType type)
{
    Initialize();

    switch (type)
    {
        case RGBColorSpaceType::SRGB: return 1;
        case RGBColorSpaceType::LinearSRGB: return 2;
        case RGBColorSpaceType::ACEScc: return 3;
        case RGBColorSpaceType::ACEScct: return 4;
        case RGBColorSpaceType::LinearACEScg: return 5;
        case RGBColorSpaceType::WorkingSpace:
        
            switch (Instance().m_currentType) {
            case ColorSpaceType::SRGB:
                return 1;
                break;
            case ColorSpaceType::LinearSRGB:
                return 2;
                break;
            case ColorSpaceType::ACEScc:
                return 3;
                break;
            case ColorSpaceType::ACEScct:
                return 4;
                break;
            case ColorSpaceType::LinearACEScg:
                return 5;
                break;
            }

        break;
        default: return 0;
    }
}