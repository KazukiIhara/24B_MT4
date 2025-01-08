#include "Functions.h"

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf
			(
				x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]
			);
		}
	}
	Novice::ScreenPrintf(x, y, "%s", label);
}

float Length(Vector3 a) {
	return std::sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector3 Normalize(Vector3 a) {
	float length = Length(a);
	Vector3 normalizedVector;

	if (length != 0) {
		normalizedVector.x = a.x / length;
		normalizedVector.y = a.y / length;
		normalizedVector.z = a.z / length;
	} else {
		normalizedVector.x = 0;
		normalizedVector.y = 0;
		normalizedVector.z = 0;
	}

	return normalizedVector;
}
