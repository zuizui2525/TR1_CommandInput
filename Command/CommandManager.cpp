#include "CommandManager.h"
#include <Novice.h>
#include <cmath>
#include <set>
#include <algorithm> // std::unique
#include <optional>

CommandManager::CommandManager()
    : currentFrame(0), lastInput(Input::None) {
    commandChecker_ = new CommandChecker(&inputBuffer_, &chargeManager_);
}

CommandManager::~CommandManager() {
    delete commandChecker_;
}

void CommandManager::Update(const char* keys, const char* preKeys) {
    currentFrame++;

    // 入力を取得して currentInput を定義
    Input rawInput = inputManager_.GetInput(keys, preKeys);

    // ★チャージ状態の更新と方向差し替え
    chargeManager_.UpdateChargeState(currentFrame / 60.0f, rawInput);

    // currentInput が方向キーのときのみチャージ置き換えを検討
    Input currentInput = chargeManager_.GetEffectiveInput(rawInput);

    // 方向キー履歴：Spin検出用（重複記録防止）
    spinDetector_.PushDirection(currentInput);

    // Spin入力検出（3F以上空けて再検出）
    if (currentFrame - lastSpinFrame_ > 3) {
        int spinCount = spinDetector_.DetectSpinCount();
        if (spinCount > 0) {
            inputBuffer_.PushInput(Input::Spin, currentFrame);
            spinDetector_.RemoveOneSpin();
        }
        lastSpinFrame_ = currentFrame;
    }

    // 入力バッファ：None以外＆前回と違うときだけ記録（長押し無効化）
    if (currentInput != Input::None && currentInput != lastInput) {

        // チャージが成立していたが、方向キーが離された瞬間に通常Inputになってしまうのを防ぐ
        // → チャージ成立中の方向キー入力を保持して強制的にInput::ChargeDownなどで記録
        Input bufferedInput = chargeManager_.GetEffectiveInput(currentInput);

        inputBuffer_.PushInput(bufferedInput, currentFrame);
    }

    // バッファが古くなったら削除（最大60F保持）
    inputBuffer_.Update(currentFrame);

    // 次フレーム用に保持
    lastInput = currentInput;
}

// バッファクリア（再スタート時など）
void CommandManager::ClearBuffer() {
    inputBuffer_.Clear();
    spinDetector_.Clear();
    chargeManager_.Clear();
    lastInput = Input::None;
}

// 入力 → 文字列
std::string CommandManager::InputToString(Input input) const {
    switch (input) {
    case Input::Up: return "8";
    case Input::UpRight: return "9";
    case Input::UpLeft: return "7";
    case Input::DownLeft: return "1";
    case Input::Down: return "2";
    case Input::DownRight: return "3";
    case Input::Left: return "4";
    case Input::Right: return "6";
    case Input::ChargeUp: return "C8";
    case Input::ChargeDown: return "C2";
    case Input::ChargeLeft: return "C4";
    case Input::ChargeRight: return "C6";
    case Input::Punch: return "P";
    case Input::Kick: return "K";
    case Input::Spin: return "O";
    case Input::None: return "N";
    default: return "";
    }
}

// 表示用履歴を文字列に変換
std::string CommandManager::GetInputHistory() const {
    return inputBuffer_.GetInputHistory();
}

bool CommandManager::CheckCommand(const Command& command) {
    return commandChecker_->CheckCommand(command);
}