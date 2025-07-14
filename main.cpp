#include <Novice.h>
#include "Command/CommandManager.h"
#include "Command/CommandList.h"

const char kWindowTitle[] = "LE2B_03_イトウカズイ_コマンド入力";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

CommandManager commandManager;

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
		commandManager.Update(keys, preKeys);

		// バカ技コマンドの判定
		if (commandManager.CheckCommand(BakaWaza)) {
			ULT = 12;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// ボリショイストリームバスターコマンドの判定
		if (commandManager.CheckCommand(BolshoiStormBuster)) {
			ULT = 8;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// スクリューパイルドライバーコマンドの判定
		if (commandManager.CheckCommand(ScrewPileDriver)) {
			ULT = 9;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// スピンドライブスマッシャーコマンドの判定
		if (commandManager.CheckCommand(spinDriveSmasher)) {
			ULT = 4;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// 真空波動拳コマンドの判定
		if (commandManager.CheckCommand(sinkuhadouken)) {
			ULT = 5;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// サマーソルトコマンドの判定
		if (commandManager.CheckCommand(Somersault)) {
			ULT = 10;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// ソニックブームコマンドの判定
		if (commandManager.CheckCommand(SonicBoom)) {
			ULT = 11;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// 昇竜拳コマンドの判定
		if (commandManager.CheckCommand(shoryuken)) {
			ULT = 1;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// 波動拳コマンドの判定
		if (commandManager.CheckCommand(hadouken)) {
			ULT = 2;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// 竜巻旋風脚コマンドの判定
		if (commandManager.CheckCommand(tatumakisenpukyaku)) {
			ULT = 3;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// トリグラフコマンドの判定
		if (commandManager.CheckCommand(torigurahu)) {
			ULT = 7;
			commandManager.ClearBuffer(); // 入力バッファをクリア
		}
		// 投げコマンドの判定
		if (commandManager.CheckCommand(nage)) {
			ULT = 6;
			commandManager.ClearBuffer(); // 入力バッファをクリア
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
		case 4:
			Novice::ScreenPrintf(0, 0, "spinDriveSmasher!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x68a88fFF, kFillModeSolid);
			break;
		case 5:
			Novice::ScreenPrintf(0, 0, "sinkuhadouken!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x7cdb8bFF, kFillModeSolid);
			break;
		case 6:
			Novice::ScreenPrintf(0, 0, "nage!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x78b164FF, kFillModeSolid);
			break;
		case 7:
			Novice::ScreenPrintf(0, 0, "torigurahu!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x78b384FF, kFillModeSolid);
			break;
		case 8:
			Novice::ScreenPrintf(0, 0, "BolshoiStormBuster!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x4bc88fFF, kFillModeSolid);
			break;
		case 9:
			Novice::ScreenPrintf(0, 0, "ScrewPileDriver!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x78b874FF, kFillModeSolid);
			break;
		case 10:
			Novice::ScreenPrintf(0, 0, "Somersault!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x6b168fFF, kFillModeSolid);
			break;
		case 11:
			Novice::ScreenPrintf(0, 0, "SonicBoom!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x1b488fFF, kFillModeSolid);
			break;
		case 12:
			Novice::ScreenPrintf(0, 0, "BakaWaza!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x3a789fFF, kFillModeSolid);
			break;
		}

		// キーディスを表示
		std::string inputHistory = commandManager.GetInputHistory();
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