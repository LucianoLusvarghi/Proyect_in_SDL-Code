//Author: Luciano Lusvarghi

#ifndef __LUSVARGHI_LIB_H__
#define __LUSVARGHI_LIB_H__ 1

#include <math.h>
#include <stdio.h>

struct Vec2{
	
	float x,y;
	
};

struct Vec3{
	
	float x,y,z;
	
};

struct Vec4{
	
	float x,y,z,w;
	
};

struct Mat2{
	
	float m[4];
	
};

struct Mat3{
	
	float m[9];
	
};

struct Mat4{
	
	float m[16];
	
};

//                    VEC 2

Vec2 SumOfVec (const Vec2& vec_1, const Vec2& vec_2);

Vec2 ResOfVec (const Vec2& vec_1, const Vec2& vec_2);

Vec2 MulOfVecWesc (const Vec2& vec_1, const float esc);

Vec2 DivOfVecWesc (const Vec2& vec_1, const float esc);

float MagOfVec (const Vec2& vec_1);

void PrintVec (const Vec2& vec_1);

Vec2 NorVec (const Vec2& vec_1);

bool BNorVec (const Vec2& vec_1);

//                  VEC 3

Vec3 SumOfVec (const Vec3& vec_1, const Vec3& vec_2);

Vec3 ResOfVec (const Vec3& vec_1, const Vec3& vec_2);

Vec3 MulOfVecWesc (const Vec3& vec_1, const float esc);

Vec3 DivOfVecWesc (const Vec3& vec_1, const float esc);

float MagOfVec (const Vec3& vec_1);

void PrintVec (const Vec3& vec_1);

Vec3 NorVec (const Vec3& vec_1);

bool BNorVec (const Vec3& vec_1);

//                  VEC 4

Vec4 SumOfVec (const Vec4& vec_1, const Vec4& vec_2);

Vec4 ResOfVec (const Vec4& vec_1, const Vec4& vec_2);

Vec4 MulOfVecWesc (const Vec4& vec_1, const float esc);

Vec4 DivOfVecWesc (const Vec4& vec_1, const float esc);

float MagOfVec (const Vec4& vec_1);

void PrintVec (const Vec4& vec_1);

Vec4 NorVec (const Vec4& vec_1);

bool BNorVec (const Vec4& vec_1);

//                MAT 2

Mat2 MulOfMat (const Mat2& mat_1, const Mat2& mat_2);

Mat2 SumOfMat (const Mat2& mat_1, const Mat2& mat_2);

Mat2 MulOfMatWesc (const Mat2& mat_1, const float esc);

void PrintMat (const Mat2& mat_1);

Mat2 DivOfMatWesc (const Mat2& mat_1, const float esc);

Mat2 ResOfMat (const Mat2& mat_1, const Mat2& mat_2);

Vec2 MulOfMatWVec (const Mat2& mat_1, const Vec2& vec_1);

Mat2 TranOfMat (const Mat2& mat_1);

Mat2 Mat2Idt();

float DetOfMat (const Mat2& mat_1);

Mat2 CofOfMat (const Mat2& mat_1);

Mat2 AdjOfMat (const Mat2& mat_1);

Mat2 InvOfMat (const Mat2& mat_1);

//              MAT 3

Mat3 MulOfMat (const Mat3& mat_1, const Mat3& mat_2);

Mat3 SumOfMat (const Mat3& mat_1, const Mat3& mat_2);

Mat3 MulOfMatWesc (const Mat3& mat_1, const float esc);

void PrintMat (const Mat3& mat_1);

Mat3 DivOfMatWesc (const Mat3& mat_1, const float esc);

Mat3 ResOfMat (const Mat3& mat_1, const Mat3& mat_2);

Vec3 MulOfMatWVec (const Mat3& mat_1, const Vec3& vec_1);

Mat3 TranOfMat(const Mat3& mat_1);

Mat3 Mat3Idt();

float DetOfMat(const Mat3& mat_1);

Mat3 CofOfMat(const Mat3& mat_1);

Mat3 AdjOfMat(const Mat3& mat_1);

Mat3 InvOfMat (const Mat3& mat_1);

Mat3 MatOfEscalate (const float x, const float y);

Mat3 MatOfRotation (const float rad);

Mat3 MatOfTranslate (const float x, const float y);

Vec2 MulOfMatWVec(const Mat3& mat_1, const Vec2& vec_1);

Mat3 MatOfTransform(const Mat3& Trans, const Mat3& Rot, const Mat3& Esc);

//                MAT 4

Mat4 MulOfMat (const Mat4& mat_1, const Mat4& mat_2);

Mat4 SumOfMat (const Mat4& mat_1, const Mat4& mat_2);

Mat4 MulOfMatWesc (const Mat4& mat_1, float esc);

void PrintMat (const Mat4& mat_1);

Mat4 DivOfMatWesc (const Mat4& mat_1, float esc);

Mat4 ResOfMat (const Mat4& mat_1, const Mat4& mat_2);

Vec4 MulOfMatWVec (const Mat4& mat_1, const Vec4& vec_1);

Mat4 TranOfMat(const Mat4& mat_1);

Mat4 Mat4Idt();

float DetOfMat(const Mat4& mat_1);

Mat4 CofOfMat(const Mat4& mat_1);

Mat4 AdjOfMat(const Mat4& mat_1);

Mat4 InvOfMat (const Mat4& mat_1);

Mat4 MatOfEscalate (float x, float y, float z);

Mat4 MatOfRotationX(float rad);

Mat4 MatOfRotationY(float rad);

Mat4 MatOfRotationZ(float rad);

Mat4 MatOfTranslate(float x, float y, float z);

Mat4 Mat4Projection();

Mat4 MatOfTransform(const Mat4& Trans, const Mat4& RotX, const Mat4& RotY, const Mat4& RotZ, const Mat4& Esc);

#endif