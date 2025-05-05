#ifndef _COLORMANAGEMENT_H_
#define _COLORMANAGEMENT_H_

#include <memory>
#include "ColorSpace.h"
#include "RGBColorSpace.h"
#include "CIEXYZColorSpace.h"
#include "LMSColorSpace.h"

class ColorManagement {
public:
    static ColorManagement& Instance();
    enum class ColorSpaceType {
        SRGB, //1
        LinearSRGB, //2
        ACEScc, //3
        ACEScct, //4
        LinearACEScg //5
    };

    enum class RGBColorSpaceType
    {
        Undefined, //0
        SRGB, //1
        LinearSRGB, //2
        ACEScc, //3
        ACEScct, //4
        LinearACEScg, //5
        WorkingSpace //6
    };
    void SetWorkingSpace(const std::shared_ptr<RGBColorSpace>& space);
    static void SetWorkingSpace(ColorSpaceType type);
    
    void SetGPUWorkingSpace(const std::shared_ptr<RGBColorSpace>& space);
    static void SetGPUWorkingSpace(ColorSpaceType type);

    void SetMainFBWorkingSpace(const std::shared_ptr<RGBColorSpace>& space);
    static void SetMainFBWorkingSpace(ColorSpaceType type);


    static ColorSpaceType GetCurrentWorkingSpaceType();

    const std::shared_ptr<RGBColorSpace> GetWorkingSpace() const;
    const std::shared_ptr<RGBColorSpace> GetWorkingLinearSpace() const;

    const std::shared_ptr<RGBColorSpace> GetGPUWorkingSpace() const;
    static ColorSpaceType GetCurrentGPUWorkingSpaceType();

    const std::shared_ptr<RGBColorSpace> GetMainFBWorkingSpace() const;
    static ColorSpaceType GetMainFBWorkingSpaceType();



    static std::shared_ptr<RGBColorSpace> SRGB();
    static std::shared_ptr<RGBColorSpace> LinearSRGB();

    static std::shared_ptr<RGBColorSpace> ACEScc();
    static std::shared_ptr<RGBColorSpace> ACEScct();
    static std::shared_ptr<RGBColorSpace> LinearACES();

    static std::shared_ptr<CIEXYZColorSpace> XYZ();
    static std::shared_ptr<LMSColorSpace> LMS();

    static std::shared_ptr<RGBColorSpace> GetRGBColorSpaceFromType(RGBColorSpaceType type);
    static int RGBColorSpaceGPUIndex(RGBColorSpaceType type);

private:
    static std::shared_ptr<RGBColorSpace> m_srgb;
    static std::shared_ptr<RGBColorSpace> m_linearSRGB;

    static std::shared_ptr<RGBColorSpace> m_acescc;
    static std::shared_ptr<RGBColorSpace> m_acescct;
    static std::shared_ptr<RGBColorSpace> m_linearACES;

    static std::shared_ptr<CIEXYZColorSpace> m_xyz;
    static std::shared_ptr<LMSColorSpace> m_lms;
    static ColorSpaceType m_currentType;
    static ColorSpaceType m_gpuCurrentType;
    static ColorSpaceType m_mainFBCurrentType;

    std::shared_ptr<RGBColorSpace> m_workingSpace;
    std::shared_ptr<RGBColorSpace> m_workingLinearSpace;

    std::shared_ptr<RGBColorSpace> m_gpuWorkingSpace;
    std::shared_ptr<RGBColorSpace> m_mainFBWorkingSpace;


    ColorManagement();
    static void Initialize();
};

#endif
