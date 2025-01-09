#define NOMINMAX
#include "Functions.h"

#include <cmath>
#include <algorithm>
#include <numbers>

static const float EPSILON = 1.0e-6f;

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

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {

	Vector3 fromTmp = from;
	Vector3 toTmp = to;

	// 正規化
	Vector3 f = Normalize(fromTmp);
	Vector3 t = Normalize(toTmp);

	// f と t の内積 (角度計算用) を求める
	float dotFT = Dot(f, t);
	// -1.0f～1.0f の範囲にクリップして acos のドメインエラーを回避
	dotFT = std::max(-1.0f, std::min(dotFT, 1.0f));

	// なす角 θ
	float angle = std::acos(dotFT);

	// クロス積で回転軸を求める (f -> t の回転軸)
	Vector3 axis = Cross(f, t);
	float axisLen = Length(axis);

	// 回転軸の長さがほぼ0なら、f と t は平行か反平行
	if (axisLen < EPSILON) {
		// ほぼ同方向 (dotFT ~ 1.0f) の場合は回転不要→単位行列を返す
		if (dotFT > 0.9999f) {
			return Matrix4x4(); // 単位行列
		} else {
			// ほぼ反対方向 (dotFT ~ -1.0f) の場合
			// 180度回転させる軸が必要（f に直交する任意の軸でOK）
			// 例えば f が (0,0,1) に近ければ x軸やy軸などに対して 180度回転
			// 一例として、f に対して最も小さい成分の軸を選ぶなど
			Vector3 orth(0.0f, 0.0f, 0.0f);
			if (std::fabs(f.x) < std::fabs(f.y) && std::fabs(f.x) < std::fabs(f.z))
				orth = Vector3(1.0f, 0.0f, 0.0f);
			else if (std::fabs(f.y) < std::fabs(f.x) && std::fabs(f.y) < std::fabs(f.z))
				orth = Vector3(0.0f, 1.0f, 0.0f);
			else
				orth = Vector3(0.0f, 0.0f, 1.0f);

			// f と直交するベクトルを作る
			axis = Cross(f, orth);
			axis = Normalize(axis);
			angle = static_cast<float>(std::numbers::pi_v<float>); // 180度回転

			return MakeRotateAxisAngle(axis, angle);
		}
	} else {
		// 軸を正規化して Rodrigues の回転公式により行列を作る
		axis = Vector3(axis.x / axisLen, axis.y / axisLen, axis.z / axisLen);
		return MakeRotateAxisAngle(axis, angle);
	}
}