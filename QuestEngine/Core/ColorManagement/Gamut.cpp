#include "Gamut.h"
#include <iostream>
// By default, the constructor initializes the gamut using sRGB primaries with a D65 white point.
Gamut::Gamut() {
    m_red = { 0.64f, 0.33f };
    m_green = { 0.30f, 0.60f };
    m_blue = { 0.15f, 0.06f };
    m_whitePoint = { 0.3127f, 0.3290f };
    UpdateMatrices();
}

Gamut::Gamut(const Chromaticity& r, const Chromaticity& g, const Chromaticity& b, const Chromaticity& wp)
    : m_red(r), m_green(g), m_blue(b), m_whitePoint(wp) {
    UpdateMatrices();
}

const Chromaticity& Gamut::GetRed() const {
    return m_red;
}

const Chromaticity& Gamut::GetGreen() const {
    return m_green;
}

const Chromaticity& Gamut::GetBlue() const {
    return m_blue;
}

const Chromaticity& Gamut::GetWhitePoint() const {
    return m_whitePoint;
}

void Gamut::SetRed(const Chromaticity& red) {
    m_red = red;
    UpdateMatrices();
}

void Gamut::SetGreen(const Chromaticity& green) {
    m_green = green;
    UpdateMatrices();
}

void Gamut::SetBlue(const Chromaticity& blue) {
    m_blue = blue;
    UpdateMatrices();
}

void Gamut::SetWhitePoint(const Chromaticity& whitePoint) {
    m_whitePoint = whitePoint;
    UpdateMatrices();
}


// // Based on the method described by Bruce Lindbloom:
// https://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
void Gamut::UpdateMatrices() {
    // 1. Calcul intermédiaire (X, Y, Z) pour chaque primaire
    float Xr = m_red.m_x / m_red.m_y;
    float Yr = 1.0f;
    float Zr = (1.0f - m_red.m_x - m_red.m_y) / m_red.m_y;

    float Xg = m_green.m_x / m_green.m_y;
    float Yg = 1.0f;
    float Zg = (1.0f - m_green.m_x - m_green.m_y) / m_green.m_y;

    float Xb = m_blue.m_x / m_blue.m_y;
    float Yb = 1.0f;
    float Zb = (1.0f - m_blue.m_x - m_blue.m_y) / m_blue.m_y;

    Matrix3x3 primaries(
        Xr, Xg, Xb,
        Yr, Yg, Yb,
        Zr, Zg, Zb
    );

    // 2. Le point blanc en XYZ
    float Xw = m_whitePoint.m_x / m_whitePoint.m_y;
    float Yw = 1.0f;
    float Zw = (1.0f - m_whitePoint.m_x - m_whitePoint.m_y) / m_whitePoint.m_y;

    Vector3D whitePointXYZ(Xw, Yw, Zw);

    // 3. Trouver les coefficients d'échelle
    Vector3D S = primaries.Inverse() * whitePointXYZ;

    // 4. Construire la vraie matrice toXYZ
    m_toXYZ = Matrix3x3(
        S.m_x * Xr, S.m_y * Xg, S.m_z * Xb,
        S.m_x * Yr, S.m_y * Yg, S.m_z * Yb,
        S.m_x * Zr, S.m_y * Zg, S.m_z * Zb
    );

    m_fromXYZ = m_toXYZ.Inverse();

}

const Matrix3x3& Gamut::GetToXYZMatrix() const {
    return m_toXYZ;
}

const Matrix3x3& Gamut::GetFromXYZMatrix() const {
    return m_fromXYZ;
}

Vector3D Gamut::GetWhitePointXYZ() const {
    float X = m_whitePoint.m_x / m_whitePoint.m_y;
    float Y = 1.0f;
    float Z = (1.0f - m_whitePoint.m_x - m_whitePoint.m_y) / m_whitePoint.m_y;
    return Vector3D(X, Y, Z);
}