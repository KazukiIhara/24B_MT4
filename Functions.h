#pragma once
#include <Novice.h>
#include "math/MathUtility.h"
#include <cmath>

using namespace KamataEngine;

static const int kRowHeight = 24;
static const int kColumnWidth = 60;

// 4x4行列を描画する関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

float Length(Vector3 a);

Vector3 Normalize(Vector3 a);
