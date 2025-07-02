#include <Novice.h>
#include "Struct.h"
#include "CommandInput.h"

const char kWindowTitle[] = "LE2B_03_イトウカズイ_コマンド入力";

// コマンドの定義
Command hadouken = {
	{Input::Down, Input::DownRight, Input::Right, Input::Punch}, // ↓↘→＋P
	7, // 猶予フレーム
};
Command shoryuken = {
	{Input::Right, Input::Down, Input::DownRight, Input::Punch}, // →↓↘＋P
	7, // 猶予フレーム
};
Command tatumakisenpukyaku = {
	{Input::Down, Input::DownLeft, Input::Left, Input::Kick}, // ↓↙←＋K
	7 // 猶予フレーム
};
Command spinDriveSmasher= {
	{Input::Down, Input::DownRight, Input::Right, Input::Down, Input::DownRight, Input::Right, Input::Kick}, // ↓↘→↓↘→＋K
	7 // 猶予フレーム
};
Command sinkuhadouken = {
	{Input::Down, Input::DownRight, Input::Right, Input::Down, Input::DownRight, Input::Right, Input::Punch}, // ↓↘→↓↘→＋P
	7 // 猶予フレーム
};
Command nage = {
	{Input::Punch, Input::Kick}, // P+K
	7 // 猶予フレーム
};
Command torigurahu = {
	{Input::Down, Input::Down, Input::Punch}, // ↓↓+P
	9 // 猶予フレーム
};
Command ScrewPileDriver = {
	{Input::Spin,Input::Punch}, // O+P
	7 // 猶予フレーム
};
Command BolshoiStormBuster = {
	{Input::Spin,Input::Spin,Input::Punch}, // OO+P
	14 // 猶予フレーム
};
Command Somersault = {
	{Input::Up, Input::Kick}, // C↓↑+K
	7, // 猶予フレーム
	{{Input::ChargeDown }}
};
Command SonicBoom = {
	{Input::Right, Input::Punch}, // C←→+P
	7, // 猶予フレーム
	{{Input::ChargeLeft }}
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

		// ボリショイストリームバスターコマンドの判定
		if (commandInput.CheckCommand(BolshoiStormBuster)) {
			ULT = 9;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// スクリューパイルドライバーコマンドの判定
		if (commandInput.CheckCommand(ScrewPileDriver)) {
			ULT = 8;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// スピンドライブスマッシャーコマンドの判定
		if (commandInput.CheckCommand(spinDriveSmasher)) {
			ULT = 4;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// 真空波動拳コマンドの判定
		if (commandInput.CheckCommand(sinkuhadouken)) {
			ULT = 5;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// サマーソルトコマンドの判定
		if (commandInput.CheckCommand(Somersault)) {
			ULT = 10;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// ソニックブームコマンドの判定
		if (commandInput.CheckCommand(SonicBoom)) {
			ULT = 11;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// 昇竜拳コマンドの判定
		if (commandInput.CheckCommand(shoryuken)) {
			ULT = 1;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// 波動拳コマンドの判定
		if (commandInput.CheckCommand(hadouken)) {
			ULT = 2;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// 竜巻旋風脚コマンドの判定
		if (commandInput.CheckCommand(tatumakisenpukyaku)) {
			ULT = 3;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// トリグラフコマンドの判定
		if (commandInput.CheckCommand(torigurahu)) {
			ULT = 7;
			commandInput.ClearBuffer(); // 入力バッファをクリア
		}
		// 投げコマンドの判定
		if (commandInput.CheckCommand(nage)) {
			ULT = 6;
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
			Novice::ScreenPrintf(0, 0, "ScrewPileDriver!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x78b874FF, kFillModeSolid);
			break;
		case 9:
			Novice::ScreenPrintf(0, 0, "BolshoiStormBuster!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x4bc88fFF, kFillModeSolid);
			break;
		case 10:
			Novice::ScreenPrintf(0, 0, "Somersault!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x6b168fFF, kFillModeSolid);
			break;
		case 11:
			Novice::ScreenPrintf(0, 0, "SonicBoom!");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, 0x1b488fFF, kFillModeSolid);
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