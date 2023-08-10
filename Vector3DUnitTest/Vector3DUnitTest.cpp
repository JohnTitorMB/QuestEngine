#include "pch.h"
#include "CppUnitTest.h"
#include "../QuestEngine/Math/Vector3D.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Vector3DUnitTest
{
	TEST_CLASS(Vector3DUnitTest)
	{
	public:
		
		TEST_METHOD(TestConstructor)
		{
			float x = 2.0f;
			float y = 3.0f;
			float z = 4.0f;
			Vector3D vec = Vector3D(x, y, z);
			Assert::AreEqual(x, vec.m_x);
			Assert::AreEqual(y, vec.m_y);
			Assert::AreEqual(z, vec.m_z);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Vector3D vec = Vector3D(2.0f, 3.0f, 4.0f);
			Vector3D vec2 = Vector3D(vec);

			Assert::AreEqual(vec2.m_x, vec.m_x);
			Assert::AreEqual(vec2.m_y, vec.m_y);
			Assert::AreEqual(vec2.m_z, vec.m_z);
		}

		TEST_METHOD(TestEqualOperator)
		{
			Vector3D vec = Vector3D(2.0f, 3.0f, 4.0f);
			Vector3D vec2 = Vector3D(2.0f, 3.0f, 4.0f);

			Assert::IsTrue(vec == vec2);
		}

		TEST_METHOD(TestNotEqualToOperator)
		{
			Vector3D vec = Vector3D(2.0f, 3.0f, 4.0f);
			Vector3D vec2 = Vector3D(2.0f, 3.0f, 5.0f);

			Assert::IsTrue(vec != vec2);
		}

		TEST_METHOD(TestPlusOperator)
		{
			Vector3D vec = Vector3D(2.0f, 3.0f, 4.0f);
			Vector3D vec2 = Vector3D(3.0f, 5.0f,10.0f);
			Vector3D result = vec + vec2;
			Vector3D expectedResult = Vector3D(5.0f, 8.0f, 14.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMultiplyOperator)
		{
			Vector3D vec = Vector3D(5.0f, 3.0f, 4.0f);
			float value = 3.0f;
			Vector3D result = vec * value;
			Vector3D expectedResult = Vector3D(15.0f, 9.0f, 12.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMinusOperator)
		{
			Vector3D vec = Vector3D(15.0f, 9.0f, 11.0f);
			Vector3D vec2 = Vector3D(6.0f, 3.0f, 4.0f);
			Vector3D result = vec - vec2;
			Vector3D expectedResult = Vector3D(9.0f, 6.0f, 7.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestDivisionOperator)
		{
			Vector3D vec = Vector3D(15.0f, 12.0f, 24.0f);
			float value = 3.0f;
			Vector3D result = vec / value;
			Vector3D expectedResult = Vector3D(5.0f, 4.0f, 8.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMagnitude)
		{
			Vector3D vec = Vector3D(3.0f, 4.0f, 5.0f);
			float result = vec.Magnitude();
			float expectedResult = sqrtf(3.0f * 3.0f + 4.0f * 4.0f + 5.0f * 5.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestNormalize)
		{
			Vector3D vec = Vector3D(3.0f, 4.0f, 5.0f);
			vec.Normalize();
			float magnitude = vec.Magnitude();
			float expectedResult = 1.0f;
			Assert::IsTrue(magnitude == expectedResult);
		}

		TEST_METHOD(TestNormalized)
		{
			Vector3D vec = Vector3D(3.0f, 4.0f, 5.0f);
			Vector3D vec2 = vec.Normalized();
			float magnitude = vec2.Magnitude();
			float expectedResult = 1.0f;
			Assert::IsTrue(magnitude == expectedResult);
		}

		TEST_METHOD(TestPlusEqualOperator)
		{
			Vector3D vec = Vector3D(2.0f, 3.0f, 4.0f);
			vec += Vector3D(3.0f, 5.0f,4.0f);
			Vector3D expectedResult = Vector3D(5.0f, 8.0f,8.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestMultiplyEqualOperator)
		{
			Vector3D vec = Vector3D(5.0f, 3.0f, 4.0f);
			float value = 3.0f;
			vec *= value;
			Vector3D expectedResult = Vector3D(15.0f, 9.0f,12.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestMinusEqualOperator)
		{
			Vector3D vec = Vector3D(15.0f, 9.0f, 12.0f);
			vec -= Vector3D(6.0f, 3.0f, 4.0f);
			Vector3D expectedResult = Vector3D(9.0f, 6.0f, 8.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestDivisionEqualOperator)
		{
			Vector3D vec = Vector3D(15.0f, 12.0f, 24.0f);
			float value = 3.0f;
			vec /= value;

			Vector3D expectedResult = Vector3D(5.0f, 4.0f, 8.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestDotProduct)
		{
			Vector3D vec = Vector3D(5.0f, 3.0f, 2.0f);
			Vector3D vec2 = Vector3D(5.0f, 3.0f, 2.0f);
			float value = Vector3D::DotProduct(vec, vec2);
			float expectedResult = 38.0f;
			Assert::IsTrue(value == expectedResult);
		}


		TEST_METHOD(TestCrossProduct)
		{
			Vector3D vec = Vector3D(1.0f, 0.0f, 0.0f);
			Vector3D vec2 = Vector3D(0.0f, 1.0f, 0.0f);
			Vector3D value = Vector3D::CrossProduct(vec, vec2);
			Vector3D expectedResult = Vector3D(0.0f, 0.0f, 1.0f);
			Assert::IsTrue(value == expectedResult);
		}
	};
}
