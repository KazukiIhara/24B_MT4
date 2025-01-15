#pragma once
#include <cmath>

#include <Novice.h>

#include "math/MathUtility.h"
#include "Quaternion.h"

using namespace KamataEngine;
using namespace MathUtility;

static const int kRowHeight = 24;
static const int kColumnWidth = 60;

// 4x4行列を描画する関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

void QuaternionScreenPrintf(int x, int y, const Quaternion& matrix, const char* label);

void VectorScreenPrintf(int x, int y, const Vector3& vector3, const char* label);

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

// クオータニオンの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
// 単位クオータニオンを返す
Quaternion IdentityQuaternion();
// 協約クオータニオンを返す
Quaternion Conjugate(const Quaternion& quaternion);
// クオータニオンのnormを返す
float Norm(const Quaternion& quaternion);
// 正規化したクオータニオンを返す
Quaternion Normalize(const Quaternion& quaternion);
// 逆クオータニオンを返す
Quaternion Inverse(const Quaternion& quaternion);
// 任意回転軸を表すクオータニオンの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
// ベクトルをクオータニオンで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
// Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

// 球面線形補完
Quaternion Slerp(Quaternion q1, Quaternion q2, float t);