#define NOMINMAX
#include "Functions.h"

#include <array>
#include <cmath>
#include <algorithm>
#include <numbers>
#include <stdexcept>

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

void QuaternionScreenPrintf(int x, int y, const Quaternion& q, const char* label) {
	std::array<float, 4> quaternion;
	quaternion[0] = q.x;
	quaternion[1] = q.y;
	quaternion[2] = q.z;
	quaternion[3] = q.w;

	for (int column = 0; column < 4; column++) {
		Novice::ScreenPrintf
		(
			x + column * kColumnWidth, y + 1 * kRowHeight, "%6.02f", quaternion[column]
		);
	}
	Novice::ScreenPrintf(x, y, "%s", label);
}

void VectorScreenPrintf(int x, int y, const Vector3& vector3, const char* label) {
	std::array<float, 3> vector;
	vector[0] = vector3.x;
	vector[1] = vector3.y;
	vector[2] = vector3.z;

	for (int column = 0; column < 3; column++) {
		Novice::ScreenPrintf
		(
			x + column * kColumnWidth, y + 1 * kRowHeight, "%6.02f", vector[column]
		);
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

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	float x1 = lhs.x, y1 = lhs.y, z1 = lhs.z, w1 = lhs.w;
	float x2 = rhs.x, y2 = rhs.y, z2 = rhs.z, w2 = rhs.w;

	Quaternion result;
	// x, y, z, w の順で格納
	result.x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
	result.y = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
	result.z = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;
	result.w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;

	return result;
}

Quaternion IdentityQuaternion() {
	Quaternion q;
	q.x = 0.0f;
	q.y = 0.0f;
	q.z = 0.0f;
	q.w = 1.0f;
	return q;
}

Quaternion Conjugate(const Quaternion& quaternion) {
	Quaternion result;
	result.x = -quaternion.x;
	result.y = -quaternion.y;
	result.z = -quaternion.z;
	result.w = quaternion.w;
	return result;
}

float Norm(const Quaternion& quaternion) {
	return std::sqrt(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);
}

Quaternion Normalize(const Quaternion& quaternion) {
	// ノルムを求める
	float length = Norm(quaternion);

	// ノルムが 0 の場合は正規化できない
	if (length == 0.0f) {
		throw std::runtime_error("Cannot normalize a zero-norm quaternion.");
	}

	// 逆数を計算
	float inv_length = 1.0f / length;

	// 正規化したクオータニオンを返す
	Quaternion result;
	result.x = quaternion.x * inv_length;
	result.y = quaternion.y * inv_length;
	result.z = quaternion.z * inv_length;
	result.w = quaternion.w * inv_length;
	return result;
}

Quaternion Inverse(const Quaternion& quaternion) {
	float norm_sq =
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w;

	if (norm_sq == 0.0f) {
		// ノルムが0の場合は逆が定義できないため例外を投げる
		throw std::runtime_error("Cannot invert a zero-norm quaternion.");
	}

	// 協約
	Quaternion conj = Conjugate(quaternion);

	// 1 / ノルム^2
	float inv_norm = 1.0f / norm_sq;

	// 結果を格納
	Quaternion result;
	result.x = conj.x * inv_norm;
	result.y = conj.y * inv_norm;
	result.z = conj.z * inv_norm;
	result.w = conj.w * inv_norm;

	return result;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	// 軸ベクトルの長さを求める
	float axis_len = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

	// 軸ベクトルがゼロ長なら回転を定義できないので、適宜例外を投げる
	if (axis_len == 0.0f) {
		throw std::runtime_error("Axis vector has zero length. Cannot create rotation quaternion.");
	}

	// 正規化した軸ベクトル
	float inv_len = 1.0f / axis_len;
	float nx = axis.x * inv_len;
	float ny = axis.y * inv_len;
	float nz = axis.z * inv_len;

	// 回転角を2で割った値
	float half_angle = angle * 0.5f;
	float s = std::sin(half_angle);
	float c = std::cos(half_angle);

	// クオータニオン (x, y, z, w) の順で格納
	Quaternion q;
	q.x = nx * s;
	q.y = ny * s;
	q.z = nz * s;
	q.w = c;

	return q;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	// ベクトル -> クオータニオン (w=0)
	Quaternion p;
	p.x = vector.x;
	p.y = vector.y;
	p.z = vector.z;
	p.w = 0.0f;

	// q * p
	// (x, y, z, w) の順で計算
	Quaternion r1;
	r1.x = quaternion.w * p.x + quaternion.y * p.z - quaternion.z * p.y;
	r1.y = quaternion.w * p.y + quaternion.z * p.x - quaternion.x * p.z;
	r1.z = quaternion.w * p.z + quaternion.x * p.y - quaternion.y * p.x;
	// w成分は - (q.x p.x + q.y p.y + q.z p.z)
	r1.w = -(quaternion.x * p.x + quaternion.y * p.y + quaternion.z * p.z);

	// conj(q) = (-x, -y, -z, w)
	Quaternion conj_q;
	conj_q.x = -quaternion.x;
	conj_q.y = -quaternion.y;
	conj_q.z = -quaternion.z;
	conj_q.w = quaternion.w;

	// r1 * conj(q)
	Quaternion r2;
	r2.x = r1.w * conj_q.x + r1.x * conj_q.w + r1.y * conj_q.z - r1.z * conj_q.y;
	r2.y = r1.w * conj_q.y + r1.y * conj_q.w + r1.z * conj_q.x - r1.x * conj_q.z;
	r2.z = r1.w * conj_q.z + r1.z * conj_q.w + r1.x * conj_q.y - r1.y * conj_q.x;
	r2.w = r1.w * conj_q.w - r1.x * conj_q.x - r1.y * conj_q.y - r1.z * conj_q.z;

	// 回転結果ベクトル (r2 の x, y, z を返す)
	Vector3 rotated;
	rotated.x = r2.x;
	rotated.y = r2.y;
	rotated.z = r2.z;

	return rotated;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
	Matrix4x4 mat;

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float yz = q.y * q.z;
	float zx = q.z * q.x;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	// 行 (i), 列 (j) の順で代入
	// row 0
	mat.m[0][0] = 1.0f - 2.0f * (yy + zz);
	mat.m[0][1] = 2.0f * (xy + wz);
	mat.m[0][2] = 2.0f * (zx - wy);
	mat.m[0][3] = 0.0f;

	// row 1
	mat.m[1][0] = 2.0f * (xy - wz);
	mat.m[1][1] = 1.0f - 2.0f * (xx + zz);
	mat.m[1][2] = 2.0f * (yz + wx);
	mat.m[1][3] = 0.0f;

	// row 2
	mat.m[2][0] = 2.0f * (zx + wy);
	mat.m[2][1] = 2.0f * (yz - wx);
	mat.m[2][2] = 1.0f - 2.0f * (xx + yy);
	mat.m[2][3] = 0.0f;

	// row 3
	mat.m[3][0] = 0.0f;
	mat.m[3][1] = 0.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 1.0f;

	return mat;
}
