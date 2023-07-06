#include "pch.h"
#include "CppUnitTest.h"
#include "..\QuestEngine\Matrix2x2.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Matrix2x2UnitTest
{
	TEST_CLASS(Matrix2x2UnitTest)
	{
	public:
		
		TEST_METHOD(FloatConstructor)
		{
			float m00 = 1.0f;
			float m01 = 2.0f;
			float m10 = 3.0f;
			float m11 = 4.0f;

			Matrix2x2 matrix = Matrix2x2(m00, m01, m10, m11);
			Assert::AreEqual(m00, matrix.m[0][0]);
			Assert::AreEqual(m01, matrix.m[0][1]);
			Assert::AreEqual(m10, matrix.m[1][0]);
			Assert::AreEqual(m11, matrix.m[1][1]);
		}

		TEST_METHOD(Vector2Constructor)
		{
			Vector2D a = Vector2D(1.0f, 2.0f);
			Vector2D b = Vector2D(3.0f, 4.0f);

			Matrix2x2 matrix = Matrix2x2(a,b);
			Assert::AreEqual(a.m_x, matrix.m[0][0]);
			Assert::AreEqual(a.m_y, matrix.m[0][1]);
			Assert::AreEqual(b.m_x, matrix.m[1][0]);
			Assert::AreEqual(b.m_y, matrix.m[1][1]);
		}

		TEST_METHOD(CopyConstructor)
		{
			float m00 = 1.0f;
			float m01 = 2.0f;
			float m10 = 3.0f;
			float m11 = 4.0f;

			Matrix2x2 matrix = Matrix2x2(m00, m01, m10, m11);
			Matrix2x2 copyMatrix = Matrix2x2(matrix);

			Assert::AreEqual(copyMatrix.m[0][0], matrix.m[0][0]);
			Assert::AreEqual(copyMatrix.m[0][1], matrix.m[0][1]);
			Assert::AreEqual(copyMatrix.m[1][0], matrix.m[1][0]);
			Assert::AreEqual(copyMatrix.m[1][1], matrix.m[1][1]);
		}

		TEST_METHOD(BracketOperator)
		{
			float m00 = 1.0f;
			float m01 = 2.0f;
			float m10 = 3.0f;
			float m11 = 4.0f;

			Matrix2x2 matrix = Matrix2x2(m00, m01, m10, m11);

			Assert::AreEqual(matrix(0, 0), m00);
			Assert::AreEqual(matrix(0, 1), m01);
			Assert::AreEqual(matrix(1, 0), m10);
			Assert::AreEqual(matrix(1, 1), m11);
		}

		TEST_METHOD(BracketConstOperator)
		{
			float m00 = 1.0f;
			float m01 = 2.0f;
			float m10 = 3.0f;
			float m11 = 4.0f;

			const Matrix2x2 matrix = Matrix2x2(m00, m01, m10, m11);

			Assert::AreEqual(matrix(0, 0), m00);
			Assert::AreEqual(matrix(0, 1), m01);
			Assert::AreEqual(matrix(1, 0), m10);
			Assert::AreEqual(matrix(1, 1), m11);
		}

		TEST_METHOD(EqualEqualOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			Matrix2x2 matrix2 = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			
			Assert::IsTrue(matrix == matrix2);
		}

		TEST_METHOD(NotEqualOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			Matrix2x2 matrix2 = Matrix2x2(40.0f, 2.0f, 3.0f, 4.0f);

			Assert::IsTrue(matrix != matrix2);
		}

		TEST_METHOD(PlusEqualOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			Matrix2x2 matrix2 = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			matrix += matrix2;

			Matrix2x2 expectedResult = Matrix2x2(2.0f, 4.0f, 6.0f, 8.0f);
			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MinusEqualOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			Matrix2x2 matrix2 = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			matrix -= matrix2;

			Matrix2x2 expectedResult = Matrix2x2(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MultiplyByScalarEqualOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			float scalar = 5.0f;
			matrix *= scalar;

			Matrix2x2 expectedResult = Matrix2x2(5.0f, 10.0f, 15.0f, 20.0f);
			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MultiplyByMatrixEqualOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 
										 3.0f, 4.0f);
			
			Matrix2x2 matrix2 = Matrix2x2(1.0f, 2.0f, 
										 3.0f, 4.0f);

			matrix *= matrix2;

			Matrix2x2 expectedResult = Matrix2x2(1.0f * 1.0f + 2.0f * 3.0f,
				1.0f * 2.0f + 2.0f * 4.0f,
				3.0f * 1.0f + 4.0f * 3.0f,
				3.0f * 2.0f + 4.0f * 4.0f);

		    Assert::IsTrue(matrix == expectedResult);
			
		}

		TEST_METHOD(TransposeUnitTest)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f,
									     3.0f, 4.0f);

			Matrix2x2 transposedMatrix = matrix.Transpose();

			Matrix2x2 expectedResult = Matrix2x2(1.0f,3.0f,
												 2.0f,4.0f);

			Assert::IsTrue(transposedMatrix == expectedResult);
		}

		TEST_METHOD(Determinant)
		{
			Matrix2x2 matrix = Matrix2x2(5.0f, 3.0f,
										 4.0f, 10.0f);

			float determinant = matrix.Determinant();

			float expectedResult = 5.0f * 10.0f - 3.0f * 4.0f;

			Assert::IsTrue(determinant == expectedResult);
		}

		TEST_METHOD(Inverse)
		{
			Matrix2x2 matrix = Matrix2x2(2.0f, 0,
										 0, 4.0f); //ScaleMatrix

			Matrix2x2 inverseMatrix = matrix.Inverse();

			Matrix2x2 expectedResult = Matrix2x2(1.0f/2.0f, 0,
				0, 1.0f/4.0f);

			Assert::IsTrue(inverseMatrix == expectedResult);
		}

		TEST_METHOD(PlusOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			Matrix2x2 matrix2 = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			Matrix2x2 matrix3 = matrix + matrix2;

			Matrix2x2 expectedResult = Matrix2x2(2.0f, 4.0f, 6.0f, 8.0f);
			Assert::IsTrue(matrix3 == expectedResult);
		}

		TEST_METHOD(MinusOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			Matrix2x2 matrix2 = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			Matrix2x2 matrix3 = matrix - matrix2;

			Matrix2x2 expectedResult = Matrix2x2(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(matrix3 == expectedResult);
		}

		TEST_METHOD(MultiplyByScalarOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			float scalar = 5.0f;
			Matrix2x2 matrix2 = matrix * scalar;

			Matrix2x2 expectedResult = Matrix2x2(5.0f, 10.0f, 15.0f, 20.0f);
			Assert::IsTrue(matrix2 == expectedResult);
		}

		TEST_METHOD(ScalarMultiplyByMatrixOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f, 3.0f, 4.0f);
			float scalar = 5.0f;
			Matrix2x2 matrix2 = scalar * matrix;

			Matrix2x2 expectedResult = Matrix2x2(5.0f, 10.0f, 15.0f, 20.0f);
			Assert::IsTrue(matrix2 == expectedResult);
		}

		TEST_METHOD(MultiplyByMatrixOperator)
		{
			Matrix2x2 matrix = Matrix2x2(1.0f, 2.0f,
				3.0f, 4.0f);

			Matrix2x2 matrix2 = Matrix2x2(1.0f, 2.0f,
				3.0f, 4.0f);

			Matrix2x2 matrix3 = matrix * matrix2;

			Matrix2x2 expectedResult = Matrix2x2(1.0f * 1.0f + 2.0f * 3.0f,
				1.0f * 2.0f + 2.0f * 4.0f,
				3.0f * 1.0f + 4.0f * 3.0f,
				3.0f * 2.0f + 4.0f * 4.0f);

			Assert::IsTrue(matrix3 == expectedResult);
		}

		TEST_METHOD(RotateUnitTest)
		{
			float angle = 90.0f;
			Matrix2x2 rotateMatrix = Matrix2x2::Rotate(angle);
			Vector2D vec = Vector2D(1.0f, 0.0f);
			Vector2D rotateVec = rotateMatrix * vec;

			Vector2D expectedResult = Vector2D(0.0f, 1.0f);


			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(std::fabs(rotateVec.m_x - expectedResult.m_x) < epsilon
				&& std::fabs(rotateVec.m_y - expectedResult.m_y) < epsilon);
		}

		TEST_METHOD(ScaleUnitTest)
		{
			Vector2D scale = Vector2D(2.0f,3.0f);
			Matrix2x2 scaleMatrix = Matrix2x2::Scale(scale);
			Vector2D vec = Vector2D(5, 10);
			Vector2D scaleVec = scaleMatrix * vec;

			Vector2D expectedResult = Vector2D(10.0f, 30.0f);


			Assert::IsTrue(scaleVec == expectedResult);
		}

		TEST_METHOD(RSUnitTest)
		{
			float angle = 90.0f;
			Vector2D scale = Vector2D(2.0f, 2.0f);
			Matrix2x2 RSMatrix = Matrix2x2::RS(angle, scale);
			Vector2D vec = Vector2D(5, 5);
			Vector2D RSVec = RSMatrix * vec;

			Vector2D expectedResult = Vector2D(-10.0f, 10.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(std::fabs(RSVec.m_x - expectedResult.m_x) < epsilon
				&& std::fabs(RSVec.m_y - expectedResult.m_y) < epsilon);
		}

	};
}
