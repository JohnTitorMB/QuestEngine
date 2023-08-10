#include "pch.h"
#include "CppUnitTest.h"
#include "../QuestEngine/Math/Vector4D.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Vector4DUnitTest
{
	TEST_CLASS(Vector4DUnitTest)
	{
	public:
		
		TEST_METHOD(TestConstructor)
		{
			float x = 2.0f;
			float y = 3.0f;
			float z = 4.0f;
			float w = 5.0f;
			Vector4D vec = Vector4D(x, y, z, w);
			Assert::AreEqual(x, vec.m_x);
			Assert::AreEqual(y, vec.m_y);
			Assert::AreEqual(z, vec.m_z);
			Assert::AreEqual(w, vec.m_w);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Vector4D vec = Vector4D(2.0f, 3.0f, 4.0f, 5.0f);
			Vector4D vec2 = Vector4D(vec);

			Assert::AreEqual(vec2.m_x, vec.m_x);
			Assert::AreEqual(vec2.m_y, vec.m_y);
			Assert::AreEqual(vec2.m_z, vec.m_z);
			Assert::AreEqual(vec2.m_w, vec.m_w);
		}

		TEST_METHOD(TestEqualOperator)
		{
			Vector4D vec = Vector4D(2.0f, 3.0f, 4.0f, 5.0f);
			Vector4D vec2 = Vector4D(2.0f, 3.0f, 4.0f, 5.0f);

			Assert::IsTrue(vec == vec2);
		}

		TEST_METHOD(TestNotEqualToOperator)
		{
			Vector4D vec = Vector4D(2.0f, 3.0f, 4.0f, 5.0f);
			Vector4D vec2 = Vector4D(2.0f, 3.0f, 4.0f, 6.0f);

			Assert::IsTrue(vec != vec2);
		}

		TEST_METHOD(TestPlusOperator)
		{
			Vector4D vec = Vector4D(2.0f, 3.0f, 4.0f, 5.0f);
			Vector4D vec2 = Vector4D(3.0f, 5.0f, 10.0f, 12.0f);
			Vector4D result = vec + vec2;
			Vector4D expectedResult = Vector4D(5.0f, 8.0f, 14.0f, 17.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMultiplyOperator)
		{
			Vector4D vec = Vector4D(5.0f, 3.0f, 4.0f,2.0f);
			float value = 3.0f;
			Vector4D result = vec * value;
			Vector4D expectedResult = Vector4D(15.0f, 9.0f, 12.0f, 6.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMinusOperator)
		{
			Vector4D vec = Vector4D(15.0f, 9.0f, 11.0f, 5.0f);
			Vector4D vec2 = Vector4D(6.0f, 3.0f, 4.0f, 2.0f);
			Vector4D result = vec - vec2;
			Vector4D expectedResult = Vector4D(9.0f, 6.0f, 7.0f, 3.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestDivisionOperator)
		{
			Vector4D vec = Vector4D(15.0f, 12.0f, 24.0f, 30.0f);
			float value = 3.0f;
			Vector4D result = vec / value;
			Vector4D expectedResult = Vector4D(5.0f, 4.0f, 8.0f, 10.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMagnitude)
		{
			Vector4D vec = Vector4D(3.0f, 4.0f, 5.0f, 6.0f);
			float result = vec.Magnitude();
			float expectedResult = sqrtf(3.0f * 3.0f + 4.0f * 4.0f + 5.0f * 5.0f + 6.0f * 6.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestNormalize)
		{
			Vector4D vec = Vector4D(3.0f, 4.0f, 5.0f, 6.0f);
			vec.Normalize();
			float magnitude = vec.Magnitude();
			float expectedResult = 1.0f;
			Assert::IsTrue(magnitude - expectedResult < FLT_EPSILON);
		}

		TEST_METHOD(TestNormalized)
		{
			Vector4D vec = Vector4D(3.0f, 4.0f, 5.0f, 6.0f);
			Vector4D vec2 = vec.Normalized();
			float magnitude = vec2.Magnitude();
			float expectedResult = 1.0f;
			Assert::IsTrue(magnitude == expectedResult);
		}

		TEST_METHOD(TestPlusEqualOperator)
		{
			Vector4D vec = Vector4D(2.0f, 3.0f, 4.0f, 5.0f);
			vec += Vector4D(3.0f, 5.0f, 4.0f, 2.0f);
			Vector4D expectedResult = Vector4D(5.0f, 8.0f, 8.0f, 7.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestMultiplyEqualOperator)
		{
			Vector4D vec = Vector4D(5.0f, 3.0f, 4.0f, 6.0f);
			float value = 3.0f;
			vec *= value;
			Vector4D expectedResult = Vector4D(15.0f, 9.0f, 12.0f, 18.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestMinusEqualOperator)
		{
			Vector4D vec = Vector4D(15.0f, 9.0f, 12.0f, 16.0f);
			vec -= Vector4D(6.0f, 3.0f, 4.0f, 2.0f);
			Vector4D expectedResult = Vector4D(9.0f, 6.0f, 8.0f, 14.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestDivisionEqualOperator)
		{
			Vector4D vec = Vector4D(15.0f, 12.0f, 24.0f, 60.0f);
			float value = 3.0f;
			vec /= value;

			Vector4D expectedResult = Vector4D(5.0f, 4.0f, 8.0f, 20.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestDotProduct)
		{
			Vector4D vec = Vector4D(5.0f, 3.0f, 2.0f,1.0f);
			Vector4D vec2 = Vector4D(5.0f, 3.0f, 2.0f,1.0f);
			float value = Vector4D::DotProduct(vec, vec2);
			float expectedResult = 39.0f;
			Assert::IsTrue(value == expectedResult);
		}
	};
}
