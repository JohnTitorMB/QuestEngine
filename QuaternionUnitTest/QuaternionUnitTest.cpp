#include "pch.h"
#include "CppUnitTest.h"
#include "../QuestEngine/Math/Quaternion.h"
#include "../QuestEngine/Math/Matrix4x4.h"
#include "../QuestEngine/Math/Vector3D.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QuaternionUnitTest
{
	TEST_CLASS(QuaternionUnitTest)
	{
	public:
		
		TEST_METHOD(QuaternionConstructor0)
		{
			Quaternion q = Quaternion();
			Assert::AreEqual(0.0f, q.m_w);
			Assert::AreEqual(0.0f, q.m_x);
			Assert::AreEqual(0.0f, q.m_y);
			Assert::AreEqual(0.0f, q.m_z);
		}

		TEST_METHOD(QuaternionConstructorCopy)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w, x, y, z);
			Quaternion q2 = Quaternion(q);
			Assert::AreEqual(q.m_w, q2.m_w);
			Assert::AreEqual(q.m_x, q2.m_x);
			Assert::AreEqual(q.m_y, q2.m_y);
			Assert::AreEqual(q.m_z, q2.m_z);
		}

		TEST_METHOD(QuaternionConstructor1)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w,x,y,z);
			Assert::AreEqual(w, q.m_w);
			Assert::AreEqual(x, q.m_x);
			Assert::AreEqual(y, q.m_y);
			Assert::AreEqual(z, q.m_z);
		}

		TEST_METHOD(QuaternionConstructorAxisAngle)
		{
			float angle = 10.0f;
			Vector3D axis = Vector3D(1, 1, 0).Normalized();
			Quaternion q = Quaternion(axis, angle);
			float angleInRadian = angle * Mathf::DegToRad;
			Assert::AreEqual(cos(angleInRadian / 2.0f), q.m_w);
			Assert::AreEqual(sin(angleInRadian / 2.0f) * axis.m_x, q.m_x);
			Assert::AreEqual(sin(angleInRadian / 2.0f) * axis.m_y, q.m_y);
			Assert::AreEqual(sin(angleInRadian / 2.0f) * axis.m_z, q.m_z);
		}

		TEST_METHOD(QuaternionConstructorEulerAngle)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 10);
			Quaternion q = Quaternion(eulerAngles, Quaternion::EulerAngleMode::XYZ);
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D quaternionResult = q * v;
			Vector3D matrixResult = m * v;
			
			float threshold = 0.001f;
			Assert::AreEqual(quaternionResult.m_x, matrixResult.m_x, threshold);
			Assert::AreEqual(quaternionResult.m_y, matrixResult.m_y, threshold);
			Assert::AreEqual(quaternionResult.m_z, matrixResult.m_z, threshold);
		}

		TEST_METHOD(NormalizeTest)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w, x, y, z);
			q.Normalize();
			float length = q.Length();

			Assert::AreEqual(length, 1.0f);
		}

		TEST_METHOD(NormalizedTest)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w, x, y, z);
			Quaternion normalizedQuaternion = q.Normalized();
			float length = normalizedQuaternion.Length();

			Assert::AreEqual(length, 1.0f);
			Assert::AreEqual(w, q.m_w);
			Assert::AreEqual(x, q.m_x);
			Assert::AreEqual(y, q.m_y);
			Assert::AreEqual(z, q.m_z);
		}

		TEST_METHOD(InverseTest)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w, x, y, z);
			Quaternion QuaternionInverse = q.Inverse();

			Quaternion q2 = q * QuaternionInverse;
			Quaternion Idenity = Quaternion::Identity();

			float threshold = 0.001f;
			Assert::AreEqual(Idenity.m_w, q2.m_w, threshold);
			Assert::AreEqual(Idenity.m_x, q2.m_x, threshold);
			Assert::AreEqual(Idenity.m_y, q2.m_y, threshold);
			Assert::AreEqual(Idenity.m_z, q2.m_z, threshold);
		}

		TEST_METHOD(LengthTest)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w, x, y, z);
			float length = q.Length();
			float expectedResult = sqrtf(10.0f * 10.0f + 5.0f * 5.0f + 8.0f * 8.0f + 12.0f * 12.0f);
	
			Assert::AreEqual(length, expectedResult);
		}

		TEST_METHOD(LengthSquaredTest)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w, x, y, z);
			float lengthSquared = q.LengthSquared();
			float expectedResult = 10.0f * 10.0f + 5.0f * 5.0f + 8.0f * 8.0f + 12.0f * 12.0f;

			Assert::AreEqual(lengthSquared, expectedResult);
		}

		TEST_METHOD(IsIdentityTest)
		{
			float w = 1.0f;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			Quaternion q = Quaternion(w, x, y, z);
			bool isIdentity = q.IsIdentity();

			Assert::AreEqual(isIdentity, true);
		}

		TEST_METHOD(IsIdentityTest2)
		{
			float w = 1.0f;
			float x = 6.0f;
			float y = 0.0f;
			float z = 3.0f;
			Quaternion q = Quaternion(w, x, y, z);
			bool isIdentity = q.IsIdentity();

			Assert::AreEqual(isIdentity, false);
		}

		TEST_METHOD(IsNormalizedTest)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w, x, y, z);
			q.Normalize();
			bool isNormalize = q.IsNormalized(0.01f);

			Assert::AreEqual(isNormalize, true);
		}

		TEST_METHOD(GetForwardDirectionTest)
		{
			float angle = 90;
			Vector3D axis = Vector3D::Up;
			Quaternion q = Quaternion(axis, angle);
			Vector3D forwardDirection = q.GetForwardDirection();
			Vector3D result = Vector3D(1.0f, 0.0f, 0.0f);
			float threshold = 0.001f;

			Assert::AreEqual(forwardDirection.m_x, result.m_x, threshold);
			Assert::AreEqual(forwardDirection.m_y, result.m_y, threshold);
			Assert::AreEqual(forwardDirection.m_z, result.m_z, threshold);
		}

		TEST_METHOD(GetRightDirectionTest)
		{
			float angle = 90;
			Vector3D axis = Vector3D::Up;
			Quaternion q = Quaternion(axis, angle);
			Vector3D rightDirection = q.GetRightDirection();
			Vector3D result = Vector3D(0, 0.0f, -1.0f);
			float threshold = 0.001f;

			Assert::AreEqual(rightDirection.m_x, result.m_x, threshold);
			Assert::AreEqual(rightDirection.m_y, result.m_y, threshold);
			Assert::AreEqual(rightDirection.m_z, result.m_z, threshold);
		}

		TEST_METHOD(GetUpDirectionTest)
		{
			float angle = 90;
			Vector3D axis = Vector3D::Forward;
			Quaternion q = Quaternion(axis, angle);
			Vector3D upDirection = q.GetUpDirection();
			Vector3D result = Vector3D(-1.0f, 0.0f, 0.0f);
			float threshold = 0.001f;

			Assert::AreEqual(upDirection.m_x, result.m_x, threshold);
			Assert::AreEqual(upDirection.m_y, result.m_y, threshold);
			Assert::AreEqual(upDirection.m_z, result.m_z, threshold);
		}


		TEST_METHOD(RotateVec3Test)
		{
			Vector3D eulerAngles = Vector3D(90, 90, 90);
			Quaternion q = Quaternion(eulerAngles, Quaternion::EulerAngleMode::XYZ);
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D quaternionResult = q.RotateVector3D(v);
			Vector3D matrixResult = m * v;

			float threshold = 0.001f;
			Assert::AreEqual(quaternionResult.m_x, matrixResult.m_x, threshold);
			Assert::AreEqual(quaternionResult.m_y, matrixResult.m_y, threshold);
			Assert::AreEqual(quaternionResult.m_z, matrixResult.m_z, threshold);
		}

		TEST_METHOD(RotateAxeAngleRelativeTest)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 10);
			Quaternion q = Quaternion(Vector3D::Forward, eulerAngles.m_z);
			q.Rotate(Vector3D::Up, eulerAngles.m_y, false);
			q.Rotate(Vector3D::Right, eulerAngles.m_x, false);
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D quaternionResult = q * v;
			Vector3D matrixResult = m * v;

			float threshold = 0.001f;
			Assert::AreEqual(quaternionResult.m_x, matrixResult.m_x, threshold);
			Assert::AreEqual(quaternionResult.m_y, matrixResult.m_y, threshold);
			Assert::AreEqual(quaternionResult.m_z, matrixResult.m_z, threshold);
		}

		TEST_METHOD(RotateAxeAngleWorldTest)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 10);
			Quaternion q = Quaternion(Vector3D::Right, eulerAngles.m_x);
			q.Rotate(Vector3D::Up, eulerAngles.m_y, true);
			q.Rotate(Vector3D::Forward, eulerAngles.m_z, true);
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D quaternionResult = q * v;
			Vector3D matrixResult = m * v;

			float threshold = 0.001f;
			Assert::AreEqual(quaternionResult.m_x, matrixResult.m_x, threshold);
			Assert::AreEqual(quaternionResult.m_y, matrixResult.m_y, threshold);
			Assert::AreEqual(quaternionResult.m_z, matrixResult.m_z, threshold);
		}

		TEST_METHOD(RotateQuaternionRelativeTest)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 10);
			Quaternion q = Quaternion(Vector3D::Forward, eulerAngles.m_z);
			Quaternion q2 = Quaternion(Vector3D::Up, eulerAngles.m_y);
			Quaternion q3 = Quaternion(Vector3D::Right, eulerAngles.m_x);
			q.Rotate(q2, false);
			q.Rotate(q3, false);
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D quaternionResult = q * v;
			Vector3D matrixResult = m * v;
			float threshold = 0.001f;

			Assert::AreEqual(quaternionResult.m_x, matrixResult.m_x, threshold);
			Assert::AreEqual(quaternionResult.m_y, matrixResult.m_y, threshold);
			Assert::AreEqual(quaternionResult.m_z, matrixResult.m_z, threshold);
		}

		TEST_METHOD(RotateQuaternionWorldTest)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 10);
			Quaternion q = Quaternion(Vector3D::Right, eulerAngles.m_x);
			Quaternion q2 = Quaternion(Vector3D::Up, eulerAngles.m_y);
			Quaternion q3 = Quaternion(Vector3D::Forward, eulerAngles.m_z);
			q.Rotate(q2, true);
			q.Rotate(q3, true);

			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D quaternionResult = q * v;
			Vector3D matrixResult = m * v;
			float threshold = 0.001f;

			Assert::AreEqual(quaternionResult.m_x, matrixResult.m_x, threshold);
			Assert::AreEqual(quaternionResult.m_y, matrixResult.m_y, threshold);
			Assert::AreEqual(quaternionResult.m_z, matrixResult.m_z, threshold);
		}

		TEST_METHOD(GoToRotation1Test)
		{
			Quaternion q = Quaternion::AxisAngle(Vector3D::Up, 45.0f);
			q.GoToRotation(Vector3D::Forward, Vector3D::Right * -1.0f);
			Quaternion q2 = Quaternion::AxisAngle(Vector3D::Up, 45.0f-90.0f);
			
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D result1 = q * v;
			Vector3D result2 = q2 * v;

			float threshold = 0.001f;
			Assert::AreEqual(result1.m_x, result2.m_x, threshold);
			Assert::AreEqual(result1.m_y, result2.m_y, threshold);
			Assert::AreEqual(result1.m_z, result2.m_z, threshold);
		}

		TEST_METHOD(GoToRotation2Test)
		{
			Quaternion q = Quaternion::AxisAngle(Vector3D::Up, 45.0f);
			q.GoToRotation(Vector3D::Right, Vector3D::Right * -1, Vector3D::Up);
			Quaternion q2 = Quaternion::AxisAngle(Vector3D::Up, 45.0f + 180.0f);

			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D result1 = q * v;
			Vector3D result2 = q2 * v;

			float threshold = 0.001f;
			Assert::AreEqual(result1.m_x, result2.m_x, threshold);
			Assert::AreEqual(result1.m_y, result2.m_y, threshold);
			Assert::AreEqual(result1.m_z, result2.m_z, threshold);
		}

		TEST_METHOD(GoToRotation3Test)
		{
			Quaternion q = Quaternion::AxisAngle(Vector3D::Up, 45.0f);
			q.GoToRotation(Vector3D::Right, Vector3D::Right * -1, Vector3D::Up * -1);
			Quaternion q2 = Quaternion::AxisAngle(Vector3D::Up, 45.0f - 180.0f);

			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D result1 = q * v;
			Vector3D result2 = q2 * v;

			float threshold = 0.001f;
			Assert::AreEqual(result1.m_x, result2.m_x, threshold);
			Assert::AreEqual(result1.m_y, result2.m_y, threshold);
			Assert::AreEqual(result1.m_z, result2.m_z, threshold);
		}

		TEST_METHOD(ToEulerAngleTest)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 10);
			Quaternion q = Quaternion(eulerAngles, Quaternion::EulerAngleMode::XYZ);
			Vector3D eulerAnglesQuaternion = q.ToEulerAngle();
			Quaternion q2 = Quaternion(eulerAnglesQuaternion, Quaternion::EulerAngleMode::XYZ);



			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D result1 = q * v;
			Vector3D result2 = q2 * v;

			float threshold = 0.001f;

			Assert::AreEqual(result1.m_x, result2.m_x, threshold);
			Assert::AreEqual(result1.m_y, result2.m_y, threshold);
			Assert::AreEqual(result1.m_z, result2.m_z, threshold);
		}

		TEST_METHOD(GetAngleTest)
		{
			Vector3D axis = Vector3D(10.4f, 3.6f, 13.0f).Normalized();
			float angle = 50;
			Quaternion q = Quaternion::AxisAngle(axis, angle);
			
			float angleQ = q.GetAngle();
			float threshold = 0.001f;
			Assert::AreEqual(angleQ, angle, threshold);
		}

		TEST_METHOD(GetAxisTest)
		{
			Vector3D axis = Vector3D(10.4f, 3.6f, 13.0f).Normalized();
			float angle = 50;
			Quaternion q = Quaternion::AxisAngle(axis, angle);

			Vector3D axisQ = q.GetAxis();
			Assert::AreEqual(axisQ == axis, true);
		}

		TEST_METHOD(GetAxisAngleTest)
		{
			Vector3D axis = Vector3D(10.4f, 3.6f, 13.0f).Normalized();
			float angle = 50;
			Quaternion q = Quaternion::AxisAngle(axis, angle);
			
			float angleQ;
			Vector3D axisQ;
			q.ToAxisAngle(axisQ, angleQ);
			float threshold = 0.001f;

			Assert::AreEqual(angleQ, angle, threshold);
			Assert::AreEqual(axisQ.m_x, axis.m_x, threshold);
			Assert::AreEqual(axisQ.m_y, axis.m_y, threshold);
			Assert::AreEqual(axisQ.m_z, axis.m_z, threshold);
		}

		TEST_METHOD(ToMatrix4x4Test)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 10);
			Quaternion q = Quaternion(eulerAngles, Quaternion::EulerAngleMode::XYZ);
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);	
			Matrix4x4 m2 = q.ToMatrix4x4();

			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D result = m * v;
			Vector3D result2 = m2 * v;

			float threshold = 0.001f;

			Assert::AreEqual(result.m_x, result2.m_x, threshold);
			Assert::AreEqual(result.m_y, result2.m_y, threshold);
			Assert::AreEqual(result.m_z, result2.m_z, threshold);
		}

		TEST_METHOD(IdentityTest)
		{
			Quaternion q = Quaternion::Identity();
		
			Assert::AreEqual(q.m_w, 1.0f);
			Assert::AreEqual(q.m_x, 0.0f);
			Assert::AreEqual(q.m_y, 0.0f);
			Assert::AreEqual(q.m_z, 0.0f);
		}

		TEST_METHOD(DotProductTest)
		{
			float w = 10.0f;
			float x = 5.0f;
			float y = 8.0f;
			float z = 12.0f;
			Quaternion q = Quaternion(w, x, y, z);

			float w2 = 14.0f;
			float x2 = 8.0f;
			float y2 = 16.0f;
			float z2 = 15.0f;
			Quaternion q2 = Quaternion(w2, x2, y2, z2);
			float dot = Quaternion::DotProduct(q,q2);
			float expectedResult = w * w2 + x * x2 + y * y2 + z * z2;

			Assert::AreEqual(dot, expectedResult);
		}

		TEST_METHOD(AxisAngleTest)
		{
			Vector3D axis = Vector3D(10.4f, 3.6f, 13.0f).Normalized();
			float angle = 50;
			Quaternion q = Quaternion::AxisAngle(axis, angle);
			float angleQ = angle;
			Vector3D axisQ = axis;


			Assert::AreEqual(angleQ == angle, true);
			Assert::AreEqual(axisQ == axis, true);
		}

		TEST_METHOD(SetGoToRotationTest)
		{
			Quaternion q = Quaternion::Identity();
			q.GoToRotation(Vector3D::Forward, Vector3D::Right * -1);

			Quaternion q2 = Quaternion::SetGoToRotation(Vector3D::Forward, Vector3D::Right * -1);

			float threshold = 0.001f;
			Assert::AreEqual(q.m_w, q2.m_w, threshold);
			Assert::AreEqual(q.m_x, q2.m_x, threshold);
			Assert::AreEqual(q.m_y, q2.m_y, threshold);
			Assert::AreEqual(q.m_z, q2.m_z, threshold);
		}

		TEST_METHOD(LookRotationTest)
		{
			Vector3D forwardTarget = Vector3D::Up * -1;
			Vector3D upTarget = Vector3D::Right;

		
			Quaternion q = Quaternion::LookRotation(forwardTarget, upTarget);
	
			Vector3D forward = q.GetForwardDirection();
			Vector3D up = q.GetUpDirection();
			
			float threshold = 0.001f;
			Assert::AreEqual(forwardTarget.m_x, forward.m_x, threshold);
			Assert::AreEqual(forwardTarget.m_y, forward.m_y, threshold);
			Assert::AreEqual(forwardTarget.m_z, forward.m_z, threshold);

			Assert::AreEqual(upTarget.m_x, up.m_x, threshold);
			Assert::AreEqual(upTarget.m_y, up.m_y, threshold);
			Assert::AreEqual(upTarget.m_z, up.m_z, threshold);
		}

		TEST_METHOD(MinusOperatorTest)
		{
			Quaternion q = Quaternion(10.0f,4.0f,3.0f,2.0f);
			Quaternion q2 = Quaternion(3.0f, 2.0f, 0.5, -3.0f);
			Quaternion q3 = q - q2;


			Assert::AreEqual(q3.m_w, 7.0f);
			Assert::AreEqual(q3.m_x, 2.0f);
			Assert::AreEqual(q3.m_y, 2.5f);
			Assert::AreEqual(q3.m_z, 5.0f);
		}

		TEST_METHOD(AdditionOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			Quaternion q2 = Quaternion(3.0f, 2.0f, 0.5, -3.0f);
			Quaternion q3 = q + q2;

			Assert::AreEqual(q3.m_w, 13.0f);
			Assert::AreEqual(q3.m_x, 6.0f);
			Assert::AreEqual(q3.m_y, 3.5f);
			Assert::AreEqual(q3.m_z, -1.0f);
		}

		TEST_METHOD(QuaternionMultiplyOperatorTest)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 10);
			Quaternion q = Quaternion::AxisAngle(Vector3D::Right, eulerAngles.m_x);
			Quaternion q2 = Quaternion::AxisAngle(Vector3D::Up, eulerAngles.m_y);
			Quaternion q4 = Quaternion::AxisAngle(Vector3D::Forward, eulerAngles.m_z);
			Quaternion q3 = q4 * q2 * q;
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D result1 = q3 * v;
			Vector3D result2 = m * v;

			float threshold = 0.001f;
			Assert::AreEqual(result1.m_x, result2.m_x, threshold);
			Assert::AreEqual(result1.m_y, result2.m_y, threshold);
			Assert::AreEqual(result1.m_z, result2.m_z, threshold);
		}

		TEST_METHOD(QuaternionMultiplyByScaleOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			float scale = 3.0f;
			Quaternion q3 = q * scale;

			Assert::AreEqual(q3.m_w, 30.0f);
			Assert::AreEqual(q3.m_x, 12.0f);
			Assert::AreEqual(q3.m_y, 9.0f);
			Assert::AreEqual(q3.m_z, 6.0f);
		}

		TEST_METHOD(QuaternionDivisionByScaleOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			float scale = 2.0f;
			Quaternion q3 = q / scale;

			Assert::AreEqual(q3.m_w, 5.0f);
			Assert::AreEqual(q3.m_x, 2.0f);
			Assert::AreEqual(q3.m_y, 1.5f);
			Assert::AreEqual(q3.m_z, 1.0f);
		}

		TEST_METHOD(QuaternionDivisionByQuaternionOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			Quaternion q2 = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);

			float scale = 2.0f;
			Quaternion q3 = q / q2;

			float threshold = 0.001f;
			Assert::AreEqual(q3.m_w, 1.0f, threshold);
			Assert::AreEqual(q3.m_x, 0.0f, threshold);
			Assert::AreEqual(q3.m_y, 0.0f, threshold);
			Assert::AreEqual(q3.m_z, 0.0f, threshold);
		}

		TEST_METHOD(MinusEqualOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			Quaternion q2 = Quaternion(3.0f, 2.0f, 0.5, -3.0f);
			q -= q2;


			Assert::AreEqual(q.m_w, 7.0f);
			Assert::AreEqual(q.m_x, 2.0f);
			Assert::AreEqual(q.m_y, 2.5f);
			Assert::AreEqual(q.m_z, 5.0f);
		}

		TEST_METHOD(AdditionEqualOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			Quaternion q2 = Quaternion(3.0f, 2.0f, 0.5, -3.0f);
			q += q2;

			Assert::AreEqual(q.m_w, 13.0f);
			Assert::AreEqual(q.m_x, 6.0f);
			Assert::AreEqual(q.m_y, 3.5f);
			Assert::AreEqual(q.m_z, -1.0f);
		}

		TEST_METHOD(QuaternionMultiplyEqualOperatorTest)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 0.0f);
			Quaternion q = Quaternion::AxisAngle(Vector3D::Up, eulerAngles.m_y);
			Quaternion q2 = Quaternion::AxisAngle(Vector3D::Right, eulerAngles.m_x);
			
			q *= q2;
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D result1 = q * v;
			Vector3D result2 = m * v;

			float threshold = 0.001f;
			Assert::AreEqual(result1.m_x, result2.m_x, threshold);
			Assert::AreEqual(result1.m_y, result2.m_y, threshold);
			Assert::AreEqual(result1.m_z, result2.m_z, threshold);
		}

		TEST_METHOD(QuaternionMultiplyByScaleEqualOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			float scale = 3.0f;

			q *= scale;

			Assert::AreEqual(q.m_w, 30.0f);
			Assert::AreEqual(q.m_x, 12.0f);
			Assert::AreEqual(q.m_y, 9.0f);
			Assert::AreEqual(q.m_z, 6.0f);
		}

		TEST_METHOD(QuaternionDivisionByScaleEqualOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			float scale = 2.0f;
			q /= scale;

			Assert::AreEqual(q.m_w, 5.0f);
			Assert::AreEqual(q.m_x, 2.0f);
			Assert::AreEqual(q.m_y, 1.5f);
			Assert::AreEqual(q.m_z, 1.0f);
		}

		TEST_METHOD(QuaternionDivisionByQuaternionEqualOperatorTest)
		{
			Quaternion q = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);
			Quaternion q2 = Quaternion(10.0f, 4.0f, 3.0f, 2.0f);

			float scale = 2.0f;
			q /= q2;
			float threshold = 0.001f;
			Assert::AreEqual(q.m_w, 1.0f, threshold);
			Assert::AreEqual(q.m_x, 0.0f, threshold);
			Assert::AreEqual(q.m_y, 0.0f, threshold);
			Assert::AreEqual(q.m_z, 0.0f, threshold);
		}

		TEST_METHOD(QuaternionMultiplyByVector3DOperatorTest)
		{
			Vector3D eulerAngles = Vector3D(45, 35, 25);
			Quaternion q = Quaternion(eulerAngles, Quaternion::EulerAngleMode::XYZ);
			Matrix4x4 m = Matrix4x4::Rotate(eulerAngles);
			Vector3D v = Vector3D(2.3f, 5.6f, 10.8f);
			Vector3D result1 = q * v;
			Vector3D result2 = m * v;

			float threshold = 0.001f;

			Assert::AreEqual(result1.m_x, result2.m_x, threshold);
			Assert::AreEqual(result1.m_y, result2.m_y, threshold);
			Assert::AreEqual(result1.m_z, result2.m_z, threshold);
		}

		TEST_METHOD(TestEqualOperator)
		{
			Quaternion q = Quaternion(2.0f, 3.0f, 4.0f, 5.0f);
			Quaternion q2 = Quaternion(2.0f, 3.0f, 4.0f, 5.0f);

			Assert::IsTrue(q == q2);
		}

		TEST_METHOD(TestNotEqualToOperator)
		{
			Quaternion q = Quaternion(2.0f, 3.0f, 4.0f, 5.0f);
			Quaternion q2 = Quaternion(2.0f, 3.0f, 4.0f, 6.0f);

			Assert::IsTrue(q != q2);
		}
	};
}
