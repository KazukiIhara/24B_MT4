#include <Novice.h>

#include "Functions.h"

const char kWindowTitle[] = "学籍番号";

using namespace KamataEngine;
using namespace MathUtility;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 axis = { 1.0f,0.4f,-0.2f };
	axis = Normalize(axis);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	Quaternion rotation = MakeRotateAxisAngleQuaternion(axis, 0.45f);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		QuaternionScreenPrintf(0, 0, rotation, "Rotation");
		MatrixScreenPrintf(0, kRowHeight * 2, rotateMatrix, "RotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByQuaternion, "RotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 9, rotateByMatrix, "RotateByMatrix");


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
