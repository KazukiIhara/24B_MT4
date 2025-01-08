#pragma once
#include <Novice.h>
#include "math/MathUtility.h"
#include <cmath>

using namespace KamataEngine;

static const int kRowHeight = 24;
static const int kColumnWidth = 60;

// 4x4行列を描画する関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);