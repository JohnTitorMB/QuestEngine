#include "pch.h"
#include "CppUnitTest.h"
#include "..\QuestEngine\Matrix3x3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Matrix3x3UnitTest
{
	TEST_CLASS(Matrix3x3UnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			float x = 2.0f;
			float y = 3.0f;
			Assert::AreEqual(x, x);
			Assert::AreEqual(y, y);
		}
	};
}
