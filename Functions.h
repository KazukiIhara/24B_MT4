#pragma once
#include <Novice.h>
#include "math/MathUtility.h"
#include <cmath>

using namespace KamataEngine;
using namespace MathUtility;

static const int kRowHeight = 24;
static const int kColumnWidth = 60;

// 4x4行列を描画する関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);