#include "Quaternion.h"
#include "Mathf.h"
#include "Matrix4x4.h"
Quaternion::Quaternion()
{
    m_w = 0.0f;
    m_x = 0.0f;
    m_y = 0.0f;
    m_z = 0.0f;
}

Quaternion::Quaternion(const Quaternion& value)
{
    m_w = value.m_w;
    m_x = value.m_x;
    m_y = value.m_y;
    m_z = value.m_z;
}

Quaternion::Quaternion(const float w, const float x, const float y, const float z)
{
    m_w = w;
    m_x = x;
    m_y = y;
    m_z = z;

}


Quaternion::Quaternion(const Vector3D& axis, float angle)
{
    float angleToRad = angle * Mathf::DegToRad;
    float halfAngle = angleToRad / 2.0f;
    m_w = cosf(halfAngle);
    m_x = sinf(halfAngle) * axis.m_x;
    m_y = sinf(halfAngle) * axis.m_y;
    m_z = sinf(halfAngle) * axis.m_z;
}

Quaternion::Quaternion(const Vector3D& eulerAngles, EulerAngleMode eulerAngleMode, bool intrinsectRotation)
{
    float angleX = eulerAngles.m_x;
    float angleY = eulerAngles.m_y;
    float angleZ = eulerAngles.m_z;
    Vector3D axeX = Vector3D::Right;
    Vector3D axeY = Vector3D::Up;
    Vector3D axeZ = Vector3D::Forward;
    EulerAxesAngles eulerAxesAngles = EulerAxesAngles();

    switch (eulerAngleMode)
    {
    case EulerAngleMode::XYZ:
        eulerAxesAngles = EulerAxesAngles(angleX, angleY, angleZ, axeX, axeY, axeZ);
        break;
    case EulerAngleMode::XZY:
        eulerAxesAngles = EulerAxesAngles(angleX, angleZ, angleY, axeX, axeZ, axeY);
        break;
    case EulerAngleMode::YXZ:
        eulerAxesAngles = EulerAxesAngles(angleY, angleX, angleZ, axeY, axeX, axeZ);
        break;
    case EulerAngleMode::YZX:
        eulerAxesAngles = EulerAxesAngles(angleY, angleZ, angleX, axeY, axeZ, axeX);
        break;
    case EulerAngleMode::ZYX:
        eulerAxesAngles = EulerAxesAngles(angleZ, angleY, angleX, axeZ, axeY, axeX);
        break;
    case EulerAngleMode::ZXY:
        eulerAxesAngles = EulerAxesAngles(angleZ, angleX, angleY, axeZ, axeX, axeY);
        break;
    case EulerAngleMode::XYX:
        eulerAxesAngles = EulerAxesAngles(angleX, angleY, angleZ, axeX, axeY, axeX);
        break;
    case EulerAngleMode::YXY:
        eulerAxesAngles = EulerAxesAngles(angleX, angleY, angleZ, axeY, axeX, axeY);
        break;
    case EulerAngleMode::ZYZ:
        eulerAxesAngles = EulerAxesAngles(angleX, angleY, angleZ, axeZ, axeY, axeZ);
        break;
    case EulerAngleMode::YZY:
        eulerAxesAngles = EulerAxesAngles(angleX, angleY, angleZ, axeY, axeZ, axeY);
        break;
    case EulerAngleMode::XZX:
        eulerAxesAngles = EulerAxesAngles(angleX, angleY, angleZ, axeX, axeZ, axeX);
        break;
    case EulerAngleMode::ZXZ:
        eulerAxesAngles = EulerAxesAngles(angleX, angleY, angleZ, axeZ, axeX, axeZ);
        break;
    default:
        break;
    }

    Quaternion angleAxis1 = Quaternion::AxisAngle(eulerAxesAngles.m_firstAxis, eulerAxesAngles.m_angle1);
    Quaternion angleAxis2 = Quaternion::AxisAngle(eulerAxesAngles.m_secondAxis, eulerAxesAngles.m_angle2);
    Quaternion angleAxis3 = Quaternion::AxisAngle(eulerAxesAngles.m_thirdAxis, eulerAxesAngles.m_angle3);

    Quaternion angleAxisResult;
    
    if(!intrinsectRotation)
        angleAxisResult = angleAxis3 * angleAxis2 * angleAxis1;
    else 
        angleAxisResult = angleAxis1 * angleAxis2 * angleAxis3;

    *this = angleAxisResult;

}

void Quaternion::Normalize()
{
    float lengthSquared = LengthSquared();
    if (abs(lengthSquared) < Mathf::Epsilon7)
    {
        *this = Identity();
        return;
    }

    float length = sqrtf(lengthSquared);
    m_w /= length;
    m_x /= length;
    m_y /= length;
    m_z /= length;
}

Quaternion Quaternion::Normalized() const
{
    float lengthSquared = LengthSquared();
    if (abs(lengthSquared) < Mathf::Epsilon7)
    {
        return Identity();
    }

    float length = sqrtf(lengthSquared);
    float w = m_w / length;
    float x = m_x / length;
    float y = m_y / length;
    float z = m_z / length;

    return Quaternion(w, x, y, z);
}

Quaternion Quaternion::Inverse() const
{
    float lengthSquared = LengthSquared();
    if (abs(lengthSquared) < Mathf::Epsilon7)
    {
        return Identity();
    }
    return Quaternion(m_w/ lengthSquared,-m_x/ lengthSquared,-m_y/ lengthSquared, -m_z/ lengthSquared);
}

float Quaternion::Length() const
{
    float lengthSquared = LengthSquared();
    if (abs(lengthSquared) < Mathf::Epsilon7)
    {
        return 0.0f;
    }
    float length = sqrtf(lengthSquared);
    return length;
}

float Quaternion::LengthSquared() const
{
    float lengthSquared = m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
    return lengthSquared;
}



bool Quaternion::IsIdentity(float tolerance) const
{
    return abs(m_x) <= tolerance &&
        abs(m_y) <= tolerance &&
        abs(m_z) <= tolerance &&
        abs(m_w - 1.0f) <= tolerance;
}

bool Quaternion::IsNormalized(float tolerance) const
{
    float lengthSquared = LengthSquared();
    return abs(lengthSquared - 1.0f) <= tolerance;
}

Vector3D Quaternion::GetForwardDirection() const
{
    return RotateVector3D(Vector3D::Forward);
}

Vector3D Quaternion::GetRightDirection() const
{
    return RotateVector3D(Vector3D::Right);
}

Vector3D Quaternion::GetUpDirection() const
{
    return RotateVector3D(Vector3D::Up);
}

Vector3D Quaternion::RotateVector3D(const Vector3D& v) const
{
    Vector3D Q = Vector3D(m_x, m_y, m_z);
    Vector3D QVCrossProduct = Vector3D::CrossProduct(Q, v);
    return v + QVCrossProduct * 2.0f * m_w + Vector3D::CrossProduct(Q, QVCrossProduct)*2.0f;
}


void Quaternion::Rotate(const Vector3D& axis, float angle, bool inWorld)
{
    Quaternion q = Quaternion::AxisAngle(axis, angle);
    if (inWorld)
        *this = q * *this;
    else
        *this *= q;  
}

void Quaternion::Rotate(const Quaternion& q, bool inWorld)
{
    if (inWorld)
        *this = q * *this;
    else
        *this *= q;
}

/// <summary>
/// Apply to this quaternion the smallest rotation between two vector in world. 
/// </summary>
/// <param name="fromDirection">The starting direction vector, which must be normalized.</param>
/// <param name="toDirection">The target direction vector, which must be normalized.</param>
void Quaternion::GoToRotation(const Vector3D& fromDirection, const Vector3D& toDirection)
{
    Vector3D axis = Vector3D();
    float cosAngle = Vector3D::DotProduct(fromDirection, toDirection);
    float angle = 0;
    //Direction is already aligned
    if (cosAngle >= 1.0f - Mathf::Epsilon7)
    {
        return;
    }
    //Direction is opposed
    else if (cosAngle <= -1.0f + Mathf::Epsilon7) 
    {
        axis = Vector3D::Orthogonal(fromDirection);
        angle = 180;
    }
    else
    {
        axis = Vector3D::CrossProduct(fromDirection, toDirection).Normalized();
        angle = acosf(std::max(-1.0f, std::min(1.0f, cosAngle))) * Mathf::RadToDeg;
    }

    Rotate(axis, angle, true);
}

/// <summary>
/// Apply to this quaternion the smallest rotation between two vector in world. 
/// </summary>
/// <param name="fromDirection">The starting direction vector, which must be normalized.</param>
/// <param name="toDirection">The target direction vector, which must be normalized.</param>
/// <param name="orthogonalDirection">Optional. A non-zero vector used as the rotation axis when 'fromDirection' and 'toDirection' are directly opposed.</param>
void Quaternion::GoToRotation(const Vector3D& fromDirection, const Vector3D& toDirection, const Vector3D& orthogonalDirection)
{
    Vector3D axis = Vector3D();
    float cosAngle = Vector3D::DotProduct(fromDirection, toDirection);
    float angle = 0;
    //Direction is already aligned
    if (cosAngle >= 1.0f - Mathf::Epsilon7)
    {
        return;
    }
    //Direction is opposed
    else if (cosAngle <= -1.0f + Mathf::Epsilon7)
    {
        axis = orthogonalDirection;
        angle = 180;
    }
    else
    {
        axis = Vector3D::CrossProduct(fromDirection, toDirection).Normalized();
        angle = acosf(std::max(-1.0f, std::min(1.0f, cosAngle)));
    }

    Rotate(axis, angle, true);
}

Vector3D Quaternion::ToEulerAngle() const
{
    Vector3D angles;

    // roll (x-axis rotation)
    float sinr_cosp = 2.0f * (m_w * m_x + m_y * m_z);
    float cosr_cosp = 1.0f - 2.0f * (m_x * m_x + m_y * m_y);
    angles.m_x = std::atan2(sinr_cosp, cosr_cosp);

    // yaw (y-axis rotation)
    float sinp = 2.0f * (m_w * m_y - m_z * m_x);
    if (std::abs(sinp) >= 1)
        angles.m_y = std::copysign(Mathf::PI / 2, sinp);
    else
        angles.m_y = std::asin(sinp);

    // pitch (z-axis rotation)
    float siny_cosp = 2.0f * (m_w * m_z + m_x * m_y);
    float cosy_cosp = 1.0f - 2.0f * (m_y * m_y + m_z * m_z);
    angles.m_z = std::atan2(siny_cosp, cosy_cosp);

    return angles * Mathf::RadToDeg;
}

float Quaternion::GetAngle() const
{
    return acos(m_w)*2.0f * Mathf::RadToDeg;
}

Vector3D Quaternion::GetAxis() const
{
    if (abs(m_w) < Mathf::Epsilon7)
        return Vector3D(0, 0, 0);

    float halfAngle = acos(m_w);
    float sinHalfAngle = sin(halfAngle);
    float x = m_x / sinHalfAngle;
    float y = m_y / sinHalfAngle;
    float z = m_z / sinHalfAngle;
    return Vector3D(x, y, z);
}

void Quaternion::ToAxisAngle(Vector3D& Axis, float& angle) const
{
    Axis = GetAxis();
    angle = GetAngle();
}

Matrix4x4 Quaternion::ToMatrix4x4() const
{
    return Matrix4x4::Rotate(*this);
}

Quaternion Quaternion::Identity()
{
    return Quaternion(1,0,0,0);
}

float Quaternion::DotProduct(const Quaternion& a, const Quaternion& b)
{
    return a.m_w * b.m_w + a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
}

Quaternion Quaternion::FromEulerAngle(const Vector3D& eulerAngle, EulerAngleMode eulerAngleMode)
{
    return Quaternion(eulerAngle, eulerAngleMode);
}

Quaternion Quaternion::AxisAngle(const Vector3D& axis, const float angle)
{
    return Quaternion(axis, angle);
}

/// <summary>
/// Generate the smallest rotation between two vector. 
/// Rotation angle is less or equal to 180 degrees
/// </summary>
/// <param name="fromDirection">The starting direction vector, which must be normalized.</param>
/// <param name="toDirection">The target direction vector, which must be normalized.</param>
/// <returns>A Quaternion representing the rotation from 'fromDirection' to 'toDirection'.</returns>
Quaternion Quaternion::SetGoToRotation(const Vector3D& fromDirection, const Vector3D& toDirection)
{
    Quaternion q = Quaternion::Identity();
    q.GoToRotation(fromDirection, toDirection);
    return q;
}

Quaternion Quaternion::LookRotation(const Vector3D& forward, const Vector3D& up)
{
    float cosAngle = Vector3D::DotProduct(forward, up);
    if (cosAngle >= 1.0f - Mathf::Epsilon7)
        return SetGoToRotation(Vector3D::Forward, forward);
    else
    {
        Vector3D tmpUp = up;
        if (cosAngle <= -1.0f + Mathf::Epsilon7)
            tmpUp = Vector3D::Orthogonal(forward);
        else
        {
            Vector3D right = Vector3D::CrossProduct(up, forward).Normalized();
            tmpUp = Vector3D::CrossProduct(forward, right).Normalized();
        }
        Quaternion q = SetGoToRotation(Vector3D::Forward, forward);
        Vector3D upPrime = q.GetUpDirection();
        Quaternion q2 = SetGoToRotation(upPrime, tmpUp);
            
        q.Rotate(q2, true);

        return q;
        
    }

}

Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t, bool longPath)
{
    float dot = Quaternion::DotProduct(a, b);
    float bias = longPath ? dot >= 0 ? -1.0f : 1.0f : dot >= 0 ? 1.0f : -1.0f;
    return a + (b * bias - a) *t;
}

Quaternion Quaternion::LerpNormalized(const Quaternion& a, const Quaternion& b, float t, bool longPath)
{
    return Lerp(a,b,t, longPath).Normalized();
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t, bool longPath)
{
    float dot = Mathf::Clamp(Quaternion::DotProduct(a, b), -1, 1);
    float bias = longPath ? (dot > 0 ? -1.0f : 1.0f) : (dot > 0 ? 1.0f : -1.0f);

    float dotBias = dot * bias;
    if (dotBias > 1.0f - Mathf::Epsilon7) {
        return Lerp(a, b, t, longPath);
    }

    float angle = acos(dotBias);
    float sinAngle = sin(angle);
    float s0 = sinf((1.0f - t) * angle) / sinAngle;
    float s1 = sinf(t * angle) / sinAngle;

    return (a * s0) + (b * s1 * bias);
}

Quaternion Quaternion::SlerpNormalized(const Quaternion& a, const Quaternion& b, float t, bool longPath)
{
    return Slerp(a, b, t, longPath).Normalized();
}

Quaternion Quaternion::operator-(const Quaternion& q)const
{
    return Quaternion(m_w - q.m_w, m_x - q.m_x, m_y - q.m_y, m_z - q.m_z);
}

Quaternion Quaternion::operator+(const Quaternion& q)const
{
    return Quaternion(m_w + q.m_w, m_x + q.m_x, m_y + q.m_y, m_z + q.m_z);
}

Quaternion Quaternion::operator*(const Quaternion& q)const
{
    float w = m_w * q.m_w - m_x * q.m_x - m_y * q.m_y - m_z * q.m_z;
    float x = m_w * q.m_x + m_x * q.m_w + m_y * q.m_z - m_z * q.m_y;
    float y = m_w * q.m_y - m_x * q.m_z + m_y * q.m_w + m_z * q.m_x;
    float z = m_w * q.m_z + m_x * q.m_y - m_y * q.m_x + m_z * q.m_w;
    return Quaternion(w,x,y,z);
}

Quaternion Quaternion::operator*(float scale)const
{
    return Quaternion(m_w * scale, m_x * scale, m_y * scale, m_z * scale);
}

Quaternion Quaternion::operator/(float scale)const
{
    return Quaternion(m_w / scale, m_x / scale, m_y / scale, m_z / scale);
}

Quaternion Quaternion::operator/(const Quaternion& q)const
{
    return *this * q.Inverse();
}


Quaternion Quaternion::operator-=(const Quaternion& q)
{
    m_w -= q.m_w;
    m_x -= q.m_x;
    m_y -= q.m_y;
    m_z -= q.m_z;

    return (*this);
}


Quaternion Quaternion::operator+=(const Quaternion& q)
{
    m_w += q.m_w;
    m_x += q.m_x;
    m_y += q.m_y;
    m_z += q.m_z;

    return (*this);
}


Quaternion Quaternion::operator*=(const Quaternion& q)
{
    float w = m_w * q.m_w - m_x * q.m_x - m_y * q.m_y - m_z * q.m_z;
    float x = m_w * q.m_x + m_x * q.m_w + m_y * q.m_z - m_z * q.m_y;
    float y = m_w * q.m_y - m_x * q.m_z + m_y * q.m_w + m_z * q.m_x;
    float z = m_w * q.m_z + m_x * q.m_y - m_y * q.m_x + m_z * q.m_w;

    m_w = w;
    m_x = x;
    m_y = y;
    m_z = z;

    return (*this);
}

Quaternion Quaternion::operator*=(float scale)
{
    m_w *= scale;
    m_x *= scale;
    m_y *= scale;
    m_z *= scale;

    return (*this);
}


Quaternion Quaternion::operator/=(float scale)
{
    *this = *this / scale;
    return  *this;
}

Quaternion Quaternion::operator/=(const Quaternion& q)
{
    *this = *this * q.Inverse();

    return  *this;
}


Vector3D Quaternion::operator*(const Vector3D& v)const
{
    return RotateVector3D(v);
}


bool Quaternion::operator!=(const Quaternion& q)const
{
    const float epsilon = Mathf::Epsilon7; // adjust to your needs
    return (abs(m_w - q.m_w) > epsilon || abs(m_x - q.m_x) > epsilon || abs(m_y - q.m_y) > epsilon || abs(m_z - q.m_z) > epsilon);
}

bool Quaternion::operator==(const Quaternion& q)const
{
    const float epsilon = Mathf::Epsilon7; // adjust to your needs

    return (abs(m_w - q.m_w) < epsilon && abs(m_x - q.m_x) < epsilon && abs(m_y - q.m_y) < epsilon && abs(m_z - q.m_z) < epsilon);
}


std::ostream& operator<<(std::ostream& os, const Quaternion& value)
{
    os << "(W : " << value.m_w << ", X : " << value.m_x << ", Y : " << value.m_y << ", Z : " << value.m_z << ")" << std::endl;
    return os;
}
