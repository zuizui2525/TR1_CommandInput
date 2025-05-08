#include <Novice.h>
#include "Struct.h"
#include "CommandInput.h"

const char kWindowTitle[] = "LE2B_03_イトウカズイ_コマンド入力";

// コマンドの定義
Command hadouken = {
	{Input::Down, Input::DownRight, Input::Right, Input::Punch}, // ↓↘→＋パンチ
	10 // 各入力間の猶予フレームを10に拡大
};
Command shoryuken = {
	{Input::Right, Input::Down, Input::DownRight, Input::Punch}, // →↓↘＋パンチ
	10 // 各入力間の猶予フレームを10に拡大
};
Command tatumakisenpukyaku = {
	{Input::Down, Input::DownLeft, Input::Left, Input::Kick}, // ↓↙←＋キック
	 10 // 各入力間の猶予フレームを10に拡大
};

CommandInput commandInput;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// 技の判定
	int ULT = 0;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

		// 入力状態を更新
		commandInput.Update(keys, preKeys);

		// 昇竜拳コマンドの判定
		if (commandInput.CheckCommand(shoryuken)) {
			ULT = 1;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// 波動拳コマンドの判定
		else if (commandInput.CheckCommand(hadouken)) {
			ULT = 2;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// 竜巻旋風脚コマンドの判定
		else if (commandInput.CheckCommand(tatumakisenpukyaku)) {
			ULT = 3;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (ULT) {
		case 1:
			Novice::ScreenPrintf(0, 0, "Shoryuken!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x7c71b8FF, kFillModeSolid);
			break;
		case 2:
			Novice::ScreenPrintf(0, 0, "Hadouken!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x86a5b7FF, kFillModeSolid);
			break;
		case 3:
			Novice::ScreenPrintf(0, 0, "tatumakisenpukyaku!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x87a868FF, kFillModeSolid);
			break;
		}

		// キーディスを表示
		std::string inputHistory = commandInput.GetInputHistory();
		Novice::ScreenPrintf(0, 20, "Input History: %s", inputHistory.c_str());

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