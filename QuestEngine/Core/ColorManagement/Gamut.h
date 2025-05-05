#ifndef _GAMUT_H_
#define _GAMUT_H_
#include "Chromaticity.h"
#include "../../Math/Matrix3x3.h"

struct Gamut {
  
    Gamut(); // Par défaut
    Gamut(const Chromaticity& r, const Chromaticity& g, const Chromaticity& b, const Chromaticity& wp);

    const Chromaticity& GetRed() const;
    const Chromaticity& GetGreen() const;
    const Chromaticity& GetBlue() const;
    const Chromaticity& GetWhitePoint() const;

    void SetRed(const Chromaticity& red);
    void SetGreen(const Chromaticity& green);
    void SetBlue(const Chromaticity& blue);
    void SetWhitePoint(const Chromaticity& whitePoint);


    const Matrix3x3& GetToXYZMatrix() const;
    const Matrix3x3& GetFromXYZMatrix() const;

    Vector3D GetWhitePointXYZ() const;

private:
    Chromaticity m_red;
    Chromaticity m_green;
    Chromaticity m_blue;
    Chromaticity m_whitePoint;
    void UpdateMatrices();
    Matrix3x3 m_toXYZ;
    Matrix3x3 m_fromXYZ;
};
#endif