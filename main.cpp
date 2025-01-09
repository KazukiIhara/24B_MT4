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


	Vector3 from0 = { 1.0f,0.0f,0.0f };
	Vector3 to0 = { -1.0f,0.0f,0.0f };

	Vector3 from1 = { 1.0f,0.7f,0.5f };
	Vector3 to1 = -from1;

	Vector3 from2 = { -0.6f,0.9f,0.2f };
	Vector3 to2 = { 0.4f,0.7f,-0.5f };

	Matrix4x4 rotateMatrix0 = DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix1 = DirectionToDirection(from1, to1);
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from2, to2);

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

		MatrixScreenPrintf(0, 0, rotateMatrix0, "Rotatematrix0");
		MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "Rotatematrix1");
		MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "Rotatematrix2");


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
