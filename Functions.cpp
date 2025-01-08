#include "Functions.h"

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf
			(
				x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.03f", matrix.m[row][column]
			);
		}
	}
	Novice::ScreenPrintf(x, y, "%s", label);
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {

	Vector3 u = axis;
	float x = u.x;
	float y = u.y;
	float z = u.z;

	float c = std::cos(-angle);
	float s = std::sin(-angle);
	float sub = 1.0f - c;

	Matrix4x4 rotation;

	rotation.m[0][0] = c + x * x * sub;
	rotation.m[0][1] = x * y * sub - z * s;
	rotation.m[0][2] = x * z * sub + y * s;
	rotation.m[0][3] = 0.0f;

	rotation.m[1][0] = y * x * sub + z * s;
	rotation.m[1][1] = c + y * y * sub;
	rotation.m[1][2] = y * z * sub - x * s;
	rotation.m[1][3] = 0.0f;

	rotation.m[2][0] = z * x * sub - y * s;
	rotation.m[2][1] = z * y * sub + x * s;
	rotation.m[2][2] = c + z * z * sub;
	rotation.m[2][3] = 0.0f;

	rotation.m[3][0] = 0.0f;
	rotation.m[3][1] = 0.0f;
	rotation.m[3][2] = 0.0f;
	rotation.m[3][3] = 1.0f;

	return rotation;
}
