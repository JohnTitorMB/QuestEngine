#include "pch.h"
#include "CppUnitTest.h"
#include "..\QuestEngine\Matrix3x3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Matrix3x3UnitTest
{
	TEST_CLASS(Matrix3x3UnitTest)
	{
	public:
		
		TEST_METHOD(FloatConstructor)
		{
			float m00 = 1.0f; float m01 = 2.0f; float m02 = 3.0f;
			float m10 = 4.0f; float m11 = 5.0f; float m12 = 6.0f;
			float m20 = 7.0f; float m21 = 8.0f; float m22 = 9.0f;
			

			Matrix3x3 matrix = Matrix3x3(m00, m01, m02, 
										 m10, m11, m12, 
										 m20, m21, m22);

			Assert::AreEqual(m00, matrix.m[0][0]);
			Assert::AreEqual(m01, matrix.m[0][1]);
			Assert::AreEqual(m02, matrix.m[0][2]);

			Assert::AreEqual(m10, matrix.m[1][0]);
			Assert::AreEqual(m11, matrix.m[1][1]);
			Assert::AreEqual(m12, matrix.m[1][2]);

			Assert::AreEqual(m20, matrix.m[2][0]);
			Assert::AreEqual(m21, matrix.m[2][1]);
			Assert::AreEqual(m22, matrix.m[2][2]);

		}

		TEST_METHOD(CopyConstructor)
		{
			float m00 = 1.0f; float m01 = 2.0f; float m02 = 3.0f;
			float m10 = 4.0f; float m11 = 5.0f; float m12 = 6.0f;
			float m20 = 7.0f; float m21 = 8.0f; float m22 = 9.0f;


			Matrix3x3 matrix = Matrix3x3(m00, m01, m02,
				m10, m11, m12,
				m20, m21, m22);
			Matrix3x3 copyMatrix = Matrix3x3(matrix);

			Assert::AreEqual(copyMatrix.m[0][0], matrix.m[0][0]);
			Assert::AreEqual(copyMatrix.m[0][1], matrix.m[0][1]);
			Assert::AreEqual(copyMatrix.m[0][2], matrix.m[0][2]);

			Assert::AreEqual(copyMatrix.m[1][0], matrix.m[1][0]);
			Assert::AreEqual(copyMatrix.m[1][1], matrix.m[1][1]);
			Assert::AreEqual(copyMatrix.m[1][2], matrix.m[1][2]);

			Assert::AreEqual(copyMatrix.m[2][0], matrix.m[2][0]);
			Assert::AreEqual(copyMatrix.m[2][1], matrix.m[2][1]);
			Assert::AreEqual(copyMatrix.m[2][2], matrix.m[2][2]);
		}

		TEST_METHOD(BracketOperator)
		{
			float m00 = 1.0f; float m01 = 2.0f; float m02 = 3.0f;
			float m10 = 4.0f; float m11 = 5.0f; float m12 = 6.0f;
			float m20 = 7.0f; float m21 = 8.0f; float m22 = 9.0f;


			Matrix3x3 matrix = Matrix3x3(m00, m01, m02,
				m10, m11, m12,
				m20, m21, m22);

			Assert::AreEqual(matrix(0, 0), m00);
			Assert::AreEqual(matrix(0, 1), m01);
			Assert::AreEqual(matrix(0, 2), m02);

			Assert::AreEqual(matrix(1, 0), m10);
			Assert::AreEqual(matrix(1, 1), m11);
			Assert::AreEqual(matrix(1, 2), m12);

			Assert::AreEqual(matrix(2, 0), m20);
			Assert::AreEqual(matrix(2, 1), m21);
			Assert::AreEqual(matrix(2, 2), m22);
		}

		TEST_METHOD(BracketConstOperator)
		{
			float m00 = 1.0f; float m01 = 2.0f; float m02 = 3.0f;
			float m10 = 4.0f; float m11 = 5.0f; float m12 = 6.0f;
			float m20 = 7.0f; float m21 = 8.0f; float m22 = 9.0f;


			const Matrix3x3 matrix = Matrix3x3(m00, m01, m02,
				m10, m11, m12,
				m20, m21, m22);

			Assert::AreEqual(matrix(0, 0), m00);
			Assert::AreEqual(matrix(0, 1), m01);
			Assert::AreEqual(matrix(0, 2), m02);

			Assert::AreEqual(matrix(1, 0), m10);
			Assert::AreEqual(matrix(1, 1), m11);
			Assert::AreEqual(matrix(1, 2), m12);

			Assert::AreEqual(matrix(2, 0), m20);
			Assert::AreEqual(matrix(2, 1), m21);
			Assert::AreEqual(matrix(2, 2), m22);
		}

		TEST_METHOD(EqualEqualOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
									     4.0f, 5.0f, 6.0f, 
										 7.0f,8.0f,9.0f);

			Matrix3x3 matrix2 = Matrix3x3(1.0f, 2.0f, 3.0f,
										  4.0f, 5.0f, 6.0f,
										  7.0f, 8.0f, 9.0f);

			Assert::IsTrue(matrix == matrix2);
		}

		TEST_METHOD(NotEqualOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix2 = Matrix3x3(40.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Assert::IsTrue(matrix != matrix2);
		}

		TEST_METHOD(PlusEqualOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix2 = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			matrix += matrix2;

			Matrix3x3 expectedResult = Matrix3x3( 2.0f,  4.0f,  6.0f, 
												  8.0f, 10.0f, 12.0f,
												 14.0f, 16.0f, 18.0f);
			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MinusEqualOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix2 = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			matrix -= matrix2;

			Matrix3x3 expectedResult = Matrix3x3(0.0f, 0.0f, 0.0f,
												 0.0f, 0.0f, 0.0f,
												 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MultiplyByScalarEqualOperator)
		{

			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);
			float scalar = 5.0f;
			matrix *= scalar;


			Matrix3x3 expectedResult = Matrix3x3(5.0f, 10.0f, 15.0f,
				20.0f, 25.0f, 30.0f,
				35.0f, 40.0f, 45.0f);

			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MultiplyByMatrixEqualOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix2 = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			matrix *= matrix2;

			Matrix3x3 expectedResult = Matrix3x3(30.0f, 36.0f, 42.0f,
				66.0f, 81.0f, 96.0f,
				102.0f, 126.0f, 150.0f);

			Assert::IsTrue(matrix == expectedResult);

		}

		TEST_METHOD(TransposeUnitTest)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
										 4.0f, 5.0f, 6.0f,
										 7.0f, 8.0f, 9.0f);

			Matrix3x3 transposedMatrix = matrix.Transpose();

			Matrix3x3 expectedResult = Matrix3x3(1.0f, 4.0f, 7.0f,
												 2.0f, 5.0f, 8.0f,
												 3.0f, 6.0f, 9.0f);

			Assert::IsTrue(transposedMatrix == expectedResult);
		}

		TEST_METHOD(Determinant)
		{
			Matrix3x3 matrix = Matrix3x3(10.0f, 0.0f, 0.0f,
										 0.0f, 20.0f,0.0f,
									     0.0f, 00.0f, 30.0f);

			float determinant = matrix.Determinant();

			float expectedResult = 6000.0f;

			Assert::IsTrue(determinant == expectedResult);
		}

		TEST_METHOD(Inverse)
		{
			Matrix3x3 matrix = Matrix3x3(2.0f, 0.0f, 0.0f,
										 0.0f, 4.0f, 0.0f,
										 0.0f, 0.0f, 6.0f); //ScaleMatrix

			Matrix3x3 inverseMatrix = matrix.Inverse();


			Matrix3x3 expectedResult = Matrix3x3(1.0f / 2.0f, 0.0f, 0.0f,
												 0.0f, 1.0f / 4.0f, 0.0f,
												 0.0f, 0.0f, 1.0f / 6.0f); //ScaleMatrix

			Assert::IsTrue(inverseMatrix == expectedResult);
		}

		TEST_METHOD(PlusOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix2 = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix3 = matrix + matrix2;

			Matrix3x3 expectedResult = Matrix3x3(2.0f, 4.0f, 6.0f,
				8.0f, 10.0f, 12.0f,
				14.0f, 16.0f, 18.0f);

			Assert::IsTrue(matrix3 == expectedResult);
		}

		TEST_METHOD(MinusOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix2 = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix3 = matrix - matrix2;

			Matrix3x3 expectedResult = Matrix3x3(0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f);

			Assert::IsTrue(matrix3 == expectedResult);
		}

		TEST_METHOD(MultiplyByScalarOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);
			float scalar = 5.0f;
			Matrix3x3 matrix2 = matrix * scalar;

			Matrix3x3 expectedResult = Matrix3x3(5.0f, 10.0f, 15.0f,
				20.0f, 25.0f, 30.0f,
				35.0f, 40.0f, 45.0f);

			Assert::IsTrue(matrix2 == expectedResult);
		}

		TEST_METHOD(ScalarMultiplyByMatrixOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);
			float scalar = 5.0f;
			Matrix3x3 matrix2 = scalar * matrix;

			Matrix3x3 expectedResult = Matrix3x3(5.0f, 10.0f, 15.0f,
				20.0f, 25.0f, 30.0f,
				35.0f, 40.0f, 45.0f);

			Assert::IsTrue(matrix2 == expectedResult);
		}

		TEST_METHOD(MultiplyByMatrixOperator)
		{
			Matrix3x3 matrix = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix2 = Matrix3x3(1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3x3 matrix3 = matrix * matrix2;

			Matrix3x3 expectedResult = Matrix3x3(30.0f, 36.0f, 42.0f,
				66.0f, 81.0f, 96.0f,
				102.0f, 126.0f, 150.0f);

			Assert::IsTrue(matrix3 == expectedResult);
		}
	};
}
