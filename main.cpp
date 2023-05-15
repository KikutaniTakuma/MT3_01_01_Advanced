#include <Novice.h>
#include "Mat4x4/Mat4x4.h"
#include "Vector3D/Vector3D.h"
#include <array>
#include <numbers>

const char kWindowTitle[] = "LE2A_04_キクタニ_タクマ_MT3_01";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	int kWindowWidth = 1280;
	int kWindowHeight = 720;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3D v1{ 1.2f, -3.9f, 2.5f };
	Vector3D v2{ 2.8f, 0.4f,-1.3f };
	Vector3D result = v1.Cross(v2);

	Vector3D rotate{};
	Vector3D translate{ 0.0f,0.0f,30.0f };
	std::array<const Vector3D, 3> kLocalVertices = {
		Vector3D{ 0.0f,15.0f,0.0f },
		Vector3D{ -20.0f,-20.0f,0.0f },
		Vector3D{ 20.0f,-20.0f,0.0f}
	};

	const float kTriangleSpd = 1.0f;
	const float kRotateY = std::numbers::pi_v<float> / 60.0f;

	Vector3D cameraPos{};
	cameraPos.z = -100.0f;

	Mat4x4 worldMatrix;
	Mat4x4 cameraMatrix;
	Mat4x4 viewMatrix;
	Mat4x4 projectionMatrix;
	Mat4x4 worldViewProjectionMatrix;
	Mat4x4 viewPortMatirx;
	std::array<Vector3D, 3> screenVertices;


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
		Vector3D move;
		if (keys[DIK_W]) {
			move.z += kTriangleSpd;
		}
		if (keys[DIK_S] && translate.z - kTriangleSpd > cameraPos.z) {
			move.z -= kTriangleSpd;
		}
		if (keys[DIK_A]) {
			move.x -= kTriangleSpd;
		}
		if (keys[DIK_D]) {
			move.x += kTriangleSpd;
		}
		translate += move;

		rotate.y += kRotateY;


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Vec3ScreenPrintf(0, 0, result, "Cross");

		cameraPos.z = -100.0f;

		worldMatrix = MakeMatrixAffin({ 1.0f,1.0f,1.0f }, rotate, translate);
		cameraMatrix = MakeMatrixAffin({ 1.0f,1.0f,1.0f }, Vector3D(), cameraPos);
		viewMatrix = MakeMatrixInverse(cameraMatrix);
		projectionMatrix = MakeMatrixPerspectiveFov(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		worldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
		viewPortMatirx = MakeMatrixViewPort(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);
		auto localVertices = kLocalVertices.begin();
		for (auto& i : screenVertices) {
			i = (*localVertices * worldViewProjectionMatrix) * viewPortMatirx;
			if (localVertices != kLocalVertices.end()) { localVertices++; }
		}

		if (cameraPos.Dot((screenVertices[1] - screenVertices[0]).Cross(screenVertices[2] - screenVertices[1])) <= 0.0f) {
			Novice::DrawTriangle(
				static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y), static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
				static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y), RED, kFillModeSolid
			);
		}
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
