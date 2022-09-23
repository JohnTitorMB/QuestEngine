#include "pch.h"
#include "CppUnitTest.h"
#include "../QuestEngine/Vector2D.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Vector2DUnitTest
{
	TEST_CLASS(Vector2DUnitTest)
	{
	public:
		
		TEST_METHOD(TestConstructor)
		{
			float x = 2.0f;
			float y = 3.0f;
			Vector2D vec = Vector2D(x, y);
			Assert::AreEqual(x, vec.m_x);
			Assert::AreEqual(y, vec.m_y);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Vector2D vec = Vector2D(2.0f, 3.0f);
			Vector2D vec2 = Vector2D(vec);

			Assert::AreEqual(vec2.m_x, vec.m_x);
			Assert::AreEqual(vec2.m_y, vec.m_y);
		}

		TEST_METHOD(TestEqualOperator)
		{
			Vector2D vec = Vector2D(2.0f, 3.0f);
			Vector2D vec2 = Vector2D(2.0f, 3.0f);

			Assert::IsTrue(vec == vec2);
		}

		TEST_METHOD(TestNotEqualToOperator)
		{
			Vector2D vec = Vector2D(2.0f, 3.0f);
			Vector2D vec2 = Vector2D(2.0f, 2.0f);

			Assert::IsTrue(vec != vec2);
		}

		TEST_METHOD(TestPlusOperator)
		{
			Vector2D vec = Vector2D(2.0f, 3.0f);
			Vector2D vec2 = Vector2D(3.0f, 5.0f);
			Vector2D result = vec + vec2;
			Vector2D expectedResult = Vector2D(5.0f, 8.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMultiplyOperator)
		{
			Vector2D vec = Vector2D(5.0f, 3.0f);
			float value = 3.0f;
			Vector2D result = vec * value;
			Vector2D expectedResult = Vector2D(15.0f, 9.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMinusOperator)
		{
			Vector2D vec = Vector2D(15.0f, 9.0f);
			Vector2D vec2 = Vector2D(6.0f, 3.0f);
			Vector2D result = vec - vec2;
			Vector2D expectedResult = Vector2D(9.0f, 6.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestDivisionOperator)
		{
			Vector2D vec = Vector2D(15.0f, 12.0f);
			float value = 3.0f;
			Vector2D result = vec / value;
			Vector2D expectedResult = Vector2D(5.0f, 4.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestMagnitude)
		{
			Vector2D vec = Vector2D(3.0f, 4.0f);
			float result = vec.Magnitude();
			float expectedResult = sqrtf(3.0f * 3.0f + 4.0f * 4.0f);
			Assert::IsTrue(result == expectedResult);
		}

		TEST_METHOD(TestNormalize)
		{
			Vector2D vec = Vector2D(3.0f, 4.0f);
			vec.Normalize();
			float magnitude = vec.Magnitude();
			float expectedResult = 1.0f;
			Assert::IsTrue(magnitude == expectedResult);
		}

		TEST_METHOD(TestNormalized)
		{
			Vector2D vec = Vector2D(3.0f, 4.0f);
			Vector2D vec2 = vec.Normalized();
			float magnitude = vec2.Magnitude();
			float expectedResult = 1.0f;
			Assert::IsTrue(magnitude == expectedResult);
		}

		TEST_METHOD(TestPlusEqualOperator)
		{
			Vector2D vec = Vector2D(2.0f, 3.0f);
			vec += Vector2D(3.0f, 5.0f);
			Vector2D expectedResult = Vector2D(5.0f, 8.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestMultiplyEqualOperator)
		{
			Vector2D vec = Vector2D(5.0f, 3.0f);
			float value = 3.0f;
			vec *= value;
			Vector2D expectedResult = Vector2D(15.0f, 9.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestMinusEqualOperator)
		{
			Vector2D vec = Vector2D(15.0f, 9.0f);
			vec -= Vector2D(6.0f, 3.0f);
			Vector2D expectedResult = Vector2D(9.0f, 6.0f);
			Assert::IsTrue(vec == expectedResult);
		}

		TEST_METHOD(TestDivisionEqualOperator)
		{
			Vector2D vec = Vector2D(15.0f, 12.0f);
			float value = 3.0f;
			vec /= value;

			Vector2D expectedResult = Vector2D(5.0f, 4.0f);
			Assert::IsTrue(vec == expectedResult);
		}
	};
}
