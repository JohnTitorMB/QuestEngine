#include "pch.h"
#include "CppUnitTest.h"
#include "..\QuestEngine\Math\Matrix4x4.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Matrix4x4UnitTest
{
	TEST_CLASS(Matrix4x4UnitTest)
	{
	public:
		
		TEST_METHOD(FloatConstructor)
		{
			float m00 = 1.0f; float m01 = 2.0f; float m02 = 3.0f; float m03 = 4.0f;
			float m10 = 5.0f; float m11 = 6.0f; float m12 = 7.0f; float m13 = 8.0f;
			float m20 = 9.0f; float m21 = 10.0f; float m22 = 11.0f; float m23 = 12.0f;
			float m30 = 13.0f; float m31 = 14.0f; float m32 = 15.0f; float m33 = 16.0f;

			Matrix4x4 matrix = Matrix4x4(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33);

			Assert::AreEqual(m00, matrix.m[0][0]);
			Assert::AreEqual(m01, matrix.m[0][1]);
			Assert::AreEqual(m02, matrix.m[0][2]);
			Assert::AreEqual(m03, matrix.m[0][3]);

			Assert::AreEqual(m10, matrix.m[1][0]);
			Assert::AreEqual(m11, matrix.m[1][1]);
			Assert::AreEqual(m12, matrix.m[1][2]);
			Assert::AreEqual(m13, matrix.m[1][3]);

			Assert::AreEqual(m20, matrix.m[2][0]);
			Assert::AreEqual(m21, matrix.m[2][1]);
			Assert::AreEqual(m22, matrix.m[2][2]);
			Assert::AreEqual(m23, matrix.m[2][3]);

			Assert::AreEqual(m30, matrix.m[3][0]);
			Assert::AreEqual(m31, matrix.m[3][1]);
			Assert::AreEqual(m32, matrix.m[3][2]);
			Assert::AreEqual(m33, matrix.m[3][3]);
		}

		TEST_METHOD(CopyConstructor)
		{
			float m00 = 1.0f; float m01 = 2.0f; float m02 = 3.0f; float m03 = 4.0f;
			float m10 = 5.0f; float m11 = 6.0f; float m12 = 7.0f; float m13 = 8.0f;
			float m20 = 9.0f; float m21 = 10.0f; float m22 = 11.0f; float m23 = 12.0f;
			float m30 = 13.0f; float m31 = 14.0f; float m32 = 15.0f; float m33 = 16.0f;


			Matrix4x4 matrix = Matrix4x4(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33);

			Matrix4x4 copyMatrix = Matrix4x4(matrix);

			Assert::AreEqual(copyMatrix.m[0][0], matrix.m[0][0]);
			Assert::AreEqual(copyMatrix.m[0][1], matrix.m[0][1]);
			Assert::AreEqual(copyMatrix.m[0][2], matrix.m[0][2]);
			Assert::AreEqual(copyMatrix.m[0][3], matrix.m[0][3]);

			Assert::AreEqual(copyMatrix.m[1][0], matrix.m[1][0]);
			Assert::AreEqual(copyMatrix.m[1][1], matrix.m[1][1]);
			Assert::AreEqual(copyMatrix.m[1][2], matrix.m[1][2]);
			Assert::AreEqual(copyMatrix.m[1][3], matrix.m[1][3]);

			Assert::AreEqual(copyMatrix.m[2][0], matrix.m[2][0]);
			Assert::AreEqual(copyMatrix.m[2][1], matrix.m[2][1]);
			Assert::AreEqual(copyMatrix.m[2][2], matrix.m[2][2]);
			Assert::AreEqual(copyMatrix.m[2][3], matrix.m[2][3]);

			Assert::AreEqual(copyMatrix.m[3][0], matrix.m[3][0]);
			Assert::AreEqual(copyMatrix.m[3][1], matrix.m[3][1]);
			Assert::AreEqual(copyMatrix.m[3][2], matrix.m[3][2]);
			Assert::AreEqual(copyMatrix.m[3][3], matrix.m[3][3]);
		}

		TEST_METHOD(BracketOperator)
		{
			float m00 = 1.0f; float m01 = 2.0f; float m02 = 3.0f; float m03 = 4.0f;
			float m10 = 5.0f; float m11 = 6.0f; float m12 = 7.0f; float m13 = 8.0f;
			float m20 = 9.0f; float m21 = 10.0f; float m22 = 11.0f; float m23 = 12.0f;
			float m30 = 13.0f; float m31 = 14.0f; float m32 = 15.0f; float m33 = 16.0f;


			Matrix4x4 matrix = Matrix4x4(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33);

			Assert::AreEqual(matrix(0, 0), m00);
			Assert::AreEqual(matrix(0, 1), m01);
			Assert::AreEqual(matrix(0, 2), m02);
			Assert::AreEqual(matrix(0, 3), m03);

			Assert::AreEqual(matrix(1, 0), m10);
			Assert::AreEqual(matrix(1, 1), m11);
			Assert::AreEqual(matrix(1, 2), m12);
			Assert::AreEqual(matrix(1, 3), m13);

			Assert::AreEqual(matrix(2, 0), m20);
			Assert::AreEqual(matrix(2, 1), m21);
			Assert::AreEqual(matrix(2, 2), m22);
			Assert::AreEqual(matrix(2, 3), m23);

			Assert::AreEqual(matrix(3, 0), m30);
			Assert::AreEqual(matrix(3, 1), m31);
			Assert::AreEqual(matrix(3, 2), m32);
			Assert::AreEqual(matrix(3, 3), m33);
		}

		TEST_METHOD(BracketConstOperator)
		{
			float m00 = 1.0f; float m01 = 2.0f; float m02 = 3.0f; float m03 = 4.0f;
			float m10 = 5.0f; float m11 = 6.0f; float m12 = 7.0f; float m13 = 8.0f;
			float m20 = 9.0f; float m21 = 10.0f; float m22 = 11.0f; float m23 = 12.0f;
			float m30 = 13.0f; float m31 = 14.0f; float m32 = 15.0f; float m33 = 16.0f;


			const Matrix4x4 matrix = Matrix4x4(m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33);

			Assert::AreEqual(matrix(0, 0), m00);
			Assert::AreEqual(matrix(0, 1), m01);
			Assert::AreEqual(matrix(0, 2), m02);
			Assert::AreEqual(matrix(0, 3), m03);

			Assert::AreEqual(matrix(1, 0), m10);
			Assert::AreEqual(matrix(1, 1), m11);
			Assert::AreEqual(matrix(1, 2), m12);
			Assert::AreEqual(matrix(1, 3), m13);

			Assert::AreEqual(matrix(2, 0), m20);
			Assert::AreEqual(matrix(2, 1), m21);
			Assert::AreEqual(matrix(2, 2), m22);
			Assert::AreEqual(matrix(2, 3), m23);

			Assert::AreEqual(matrix(3, 0), m30);
			Assert::AreEqual(matrix(3, 1), m31);
			Assert::AreEqual(matrix(3, 2), m32);
			Assert::AreEqual(matrix(3, 3), m33);
		}

		TEST_METHOD(EqualEqualOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f,12.0f,
				13.0f, 14.0f, 15.0f ,16.0f);

			Matrix4x4 matrix2 = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Assert::IsTrue(matrix == matrix2);
		}

		TEST_METHOD(NotEqualOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix2 = Matrix4x4(100.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Assert::IsTrue(matrix != matrix2);
		}

		TEST_METHOD(PlusEqualOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix2 = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			matrix += matrix2;

			Matrix4x4 expectedResult = Matrix4x4(2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f,24.0f,
				26.0f, 28.0f, 30.0f , 32.0f);

			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MinusEqualOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix2 = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			matrix -= matrix2;

			Matrix4x4 expectedResult = Matrix4x4(0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,  0.0f,
				0.0f, 0.0f, 0.0f,  0.0f);

			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MultiplyByScalarEqualOperator)
		{

			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);
			float scalar = 5.0f;
			matrix *= scalar;


			Matrix4x4 expectedResult = Matrix4x4(5.0f, 10.0f, 15.0f, 20.0f,
				25.0f, 30.0f, 35.0f, 40.0f,
				45.0f, 50.0f, 55.0f, 60.0f,
				65.0f, 70.0f, 75.0f, 80.0f);

			Assert::IsTrue(matrix == expectedResult);
		}

		TEST_METHOD(MultiplyByMatrixEqualOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix2 = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			matrix *= matrix2;

			Matrix4x4 expectedResult = Matrix4x4(90.0f, 100.0f, 110.0f, 120.0f,
				202.0f, 228.0f, 254.0f, 280.0f,
				314.0f, 356.0f, 398.0f, 440.0f,
				426.0f, 484.0f, 542.0f, 600.0f);

			Assert::IsTrue(matrix == expectedResult);

		}

		TEST_METHOD(TransposeUnitTest)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 transposedMatrix = matrix.Transpose();

			Matrix4x4 expectedResult = Matrix4x4(1.0f, 5.0f, 9.0f, 13.0f,
												 2.0f, 6.0f, 10.0f, 14.0f,
												 3.0f, 7.0f, 11.0f, 15.0f,
												 4.0f, 8.0f, 12.0f, 16.0f);

			Assert::IsTrue(transposedMatrix == expectedResult);
		}

		TEST_METHOD(Determinant)
		{
			Matrix4x4 matrix = Matrix4x4(10.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 20.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 30.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 40.0f);

			float determinant = matrix.Determinant();

			float expectedResult = 240000.0f;

			Assert::IsTrue(abs(determinant - expectedResult) < FLT_EPSILON);
		}

		TEST_METHOD(Inverse)
		{
			Matrix4x4 matrix = Matrix4x4(2.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 4.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 6.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 8.0f);

			Matrix4x4 inverseMatrix = matrix.Inverse();


			Matrix4x4 expectedResult = Matrix4x4(1.0f / 2.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f / 4.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f / 6.0f, 0.0f, //ScaleMatrix
				0.0f, 0.0f, 0.0f, 1.0f / 8.0f); //ScaleMatrix

			Assert::IsTrue(inverseMatrix == expectedResult);
		}

		TEST_METHOD(PlusOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix2 = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix3 = matrix + matrix2;

			Matrix4x4 expectedResult = Matrix4x4(2.0f, 4.0f, 6.0f, 8.0f,
				10.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f,
				26.0f, 28.0f, 30.0f, 32.0f);

			Assert::IsTrue(matrix3 == expectedResult);
		}

		TEST_METHOD(MinusOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix2 = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix3 = matrix - matrix2;

			Matrix4x4 expectedResult = Matrix4x4(0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f);

			Assert::IsTrue(matrix3 == expectedResult);
		}

		TEST_METHOD(MultiplyByScalarOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);
			float scalar = 5.0f;
			Matrix4x4 matrix2 = matrix * scalar;


			Matrix4x4 expectedResult = Matrix4x4(5.0f, 10.0f, 15.0f, 20.0f,
				25.0f, 30.0f, 35.0f, 40.0f,
				45.0f, 50.0f, 55.0f, 60.0f,
				65.0f, 70.0f, 75.0f, 80.0f);

			Assert::IsTrue(matrix2 == expectedResult);
		}

		TEST_METHOD(ScalarMultiplyByMatrixOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);
			float scalar = 5.0f;
			Matrix4x4 matrix2 = scalar * matrix;

			Matrix4x4 expectedResult = Matrix4x4(5.0f, 10.0f, 15.0f, 20.0f,
				25.0f, 30.0f, 35.0f, 40.0f,
				45.0f, 50.0f, 55.0f, 60.0f,
				65.0f, 70.0f, 75.0f, 80.0f);


			Assert::IsTrue(matrix2 == expectedResult);
		}

		TEST_METHOD(MultiplyByMatrixOperator)
		{
			Matrix4x4 matrix = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix2 = Matrix4x4(1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f);

			Matrix4x4 matrix3 = matrix * matrix2;

			Matrix4x4 expectedResult = Matrix4x4(90.0f, 100.0f, 110.0f, 120.0f,
				202.0f, 228.0f, 254.0f, 280.0f,
				314.0f, 356.0f, 398.0f, 440.0f,
				426.0f, 484.0f, 542.0f, 600.0f);

			Assert::IsTrue(matrix3 == expectedResult);
		}

		TEST_METHOD(TranslateUnitTest)
		{
			Vector3D translate = Vector3D(1.0f, 2.0f, 3.0f);
			Matrix4x4 translateMatrix = Matrix4x4::Translate(translate);
			Vector3D vec = Vector3D(1.0f, 5.0f, 8.0f);
			Vector3D translateVec = translateMatrix * vec;

			Vector3D expectedResult = Vector3D(2.0f, 7.0f, 11.0f);


			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(translateVec == expectedResult);
		}

		TEST_METHOD(RotateXUnitTest)
		{
			float angle = 90.0f;
			Matrix4x4 rotateMatrix = Matrix4x4::RotateX(angle);
			Vector3D vec = Vector3D(0.0f, 0.0f, 1.0f);
			Vector3D rotateVec = rotateMatrix * vec;
			Vector3D expectedResult = Vector3D(0.0f, -1.0f,0.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(rotateVec == expectedResult);
		}

		TEST_METHOD(RotateYUnitTest)
		{
			float angle = 90.0f;
			Matrix4x4 rotateMatrix = Matrix4x4::RotateY(angle);
			Vector3D vec = Vector3D(1.0f, 0.0f, 0.0f);
			Vector3D rotateVec = rotateMatrix * vec;
			Vector3D expectedResult = Vector3D(0.0f, 0.0f, -1.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(rotateVec == expectedResult);
		}

		TEST_METHOD(RotateZUnitTest)
		{
			float angle = 90.0f;
			Matrix4x4 rotateMatrix = Matrix4x4::RotateZ(angle);
			Vector3D vec = Vector3D(1.0f, 0.0f, 0.0f);
			Vector3D rotateVec = rotateMatrix * vec;
			Vector3D expectedResult = Vector3D(0.0f, 1.0f, 0.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(rotateVec == expectedResult);
		}

		TEST_METHOD(RotateUnitTest)
		{
			Vector3D angle = Vector3D(90.0f,270.0f,180.0f);
			Matrix4x4 rotateMatrix = Matrix4x4::Rotate(angle);
			Vector3D vec = Vector3D(0.0f, 1.0f, 0.0f);
			Vector3D rotateVec = rotateMatrix * vec;

			//RotateX : 0, 0, 1
			// RotateY : -1, 0, 0
			//RotateZ  : 1, 0, 0
			Vector3D expectedResult = Vector3D(1.0f, 0.0f, 0.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(rotateVec == expectedResult);
		}

		TEST_METHOD(ScaleUnitTest)
		{
			Vector3D scale = Vector3D(2.0f, 3.0f, 4.0f);
			Matrix4x4 scaleMatrix = Matrix4x4::ScaleXYZ(scale);
			Vector3D vec = Vector3D(5, 10, 15.0f);
			Vector3D scaleVec = scaleMatrix * vec;

			Vector3D expectedResult = Vector3D(10.0f, 30.0f, 60.0f);


			Assert::IsTrue(scaleVec == expectedResult);
		}

		TEST_METHOD(TSUnitTest)
		{
			Vector3D translate = Vector3D(1.0f, 2.0f, 3.0f);
			Vector3D scale = Vector3D(2.0f, 2.0f, 2.0f);
			Matrix4x4 TSMatrix = Matrix4x4::TS(translate, scale);
			Vector3D vec = Vector3D(2, 5, 8.0f);
			Vector3D TSVec = TSMatrix * vec;

			Vector3D expectedResult = Vector3D(5.0f, 12.0f, 19.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(TSVec == expectedResult);
		}

		TEST_METHOD(RSUnitTest)
		{
			Vector3D angle = Vector3D(90.0f, 270.0f, 180.0f);
			Vector3D scale = Vector3D(2.0f, 2.0f,2.0f);
			Matrix4x4 RSMatrix = Matrix4x4::RS(angle, scale);
			Vector3D vec = Vector3D(0.0f, 1.0f,0.0f);
			Vector3D RSVec = RSMatrix * vec;

			Vector3D expectedResult = Vector3D(2.0f, 0.0f,0.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(RSVec == expectedResult);
		}

		TEST_METHOD(TRUnitTest)
		{
			Vector3D translate = Vector3D(3.0f, -5.0f, 3.0f);
			Vector3D angle = Vector3D(90.0f, 270.0f, 180.0f);
			Matrix4x4 TRMatrix = Matrix4x4::TR(translate, angle);
			Vector3D vec = Vector3D(0.0f, 1.0f, 0.0f);
			Vector3D TRVec = TRMatrix * vec;

			Vector3D expectedResult = Vector3D(4.0f, -5.0f,3.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(TRVec == expectedResult);
		}

		TEST_METHOD(TRSUnitTest)
		{
			Vector3D translate = Vector3D(3.0f, -5.0f, 3.0f);
			Vector3D angle = Vector3D(90.0f, 270.0f, 180.0f);
			Vector3D scale = Vector3D(2.0f, 2.0f, 2.0f);
			Matrix4x4 TRSMatrix = Matrix4x4::TRS(translate, angle, scale);
			Vector3D vec = Vector3D(0.0f, 1.0f, 0.0f);
			Vector3D TRSVec = TRSMatrix * vec;

			Vector3D expectedResult = Vector3D(5.0f, -5.0f, 3.0f);

			const float epsilon = 1e-5; // ajustez selon vos besoins

			// Utiliser une comparaison epsilon pour les flottants
			Assert::IsTrue(TRSVec == expectedResult);
		}
	};
}
