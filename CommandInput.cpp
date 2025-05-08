#include "CommandInput.h"
#include <Novice.h>

CommandInput::CommandInput() : currentFrame(0) {}

void CommandInput::Update(const char* keys, const char* preKeys) {
    currentFrame++;

    // 現在の入力を取得
    Input currentInput = GetInput(keys, preKeys);
    if (currentInput != Input::None) {
        // 入力をバッファに追加
        inputBuffer.push({ currentInput, currentFrame });

        // 入力履歴を更新（最大履歴数を制限）
        inputHistory.push_back(currentInput);
        if (inputHistory.size() > 10) { // 履歴は最大10件まで保持
            inputHistory.erase(inputHistory.begin());
        }
    }

    // 古い入力を削除（60フレーム以上前の入力を無効化）
    while (!inputBuffer.empty() && currentFrame - inputBuffer.front().second > 60) {
        inputBuffer.pop();
    }
}

bool CommandInput::CheckCommand(const Command& command) {
    const std::vector<Input>& sequence = command.sequence;
    int maxFrameGap = command.maxFrameGap;

    int lastFrame = -1;
    size_t sequenceIndex = 0;

    // 入力バッファをコピーして操作
    std::queue<std::pair<Input, int>> tempQueue = inputBuffer;
    while (!tempQueue.empty()) {
        const auto& [input, frame] = tempQueue.front();

        // 現在の入力がコマンドの現在のシーケンスに一致する場合
        if (input == sequence[sequenceIndex]) {
            // 入力間のフレーム差を確認
            if (lastFrame != -1 && frame - lastFrame > maxFrameGap) {
                return false; // 猶予フレームを超えた場合
            }
            lastFrame = frame;
            sequenceIndex++;

            // コマンドがすべて一致した場合
            if (sequenceIndex == sequence.size()) {
                return true; // コマンド成功
            }
        }

        // 次の入力を確認
        tempQueue.pop();
    }

    // コマンドが成立しなかった場合
    return false;
}

void CommandInput::ClearBuffer() {
    // 入力バッファをクリア
    while (!inputBuffer.empty()) {
        inputBuffer.pop();
    }
    inputHistory.clear(); // 履歴もクリア
}

Input CommandInput::GetInput(const char* keys, const char* preKeys) {
    // パンチボタン（Uキー）の判定
    if (keys[DIK_U] && !preKeys[DIK_U]) {
        return Input::Punch;
    }
    // キックボタン (Jキー) の判定
    if (keys[DIK_J] && !preKeys[DIK_J]) {
        return Input::Kick;
    }
    // 斜め入力（↘）の判定を柔軟化
    if (keys[DIK_S] && keys[DIK_D]) {
        return Input::DownRight;
    }
    // 斜め入力（↙）の判定を柔軟化
    if (keys[DIK_S] && keys[DIK_A]) {
        return Input::DownLeft;
    }
    // 下入力の判定
    if (keys[DIK_S]) {
        return Input::Down;
    }
    // 右入力の判定
    if (keys[DIK_D]) {
        return Input::Right;
    }
    // 左入力の判定
    if (keys[DIK_A]) {
        return Input::Left;
    }
    return Input::None; // 入力なし
}

std::string CommandInput::InputToString(Input input) const {
    switch (input) {
    case Input::DownLeft: return "1";
    case Input::Down: return "2";
    case Input::DownRight: return "3";
    case Input::Left: return "4";
    case Input::Right: return "6";
    case Input::Punch: return "P";
    case Input::Kick: return "K";
    default: return "";
    }
}

std::string CommandInput::GetInputHistory() const {
    std::string history;
    for (const auto& input : inputHistory) {
        history += InputToString(input) + " ";
    }
    return history;
}