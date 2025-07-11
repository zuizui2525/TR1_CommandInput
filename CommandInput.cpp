#include "CommandInput.h"
#include <Novice.h>
#include <cmath>
#include <set>
#include <algorithm> // std::unique
#include <optional>

CommandInput::CommandInput()
    : currentFrame(0), lastInput(Input::None) {
}

void CommandInput::Update(const char* keys, const char* preKeys) {
    currentFrame++;

    // 入力を取得して currentInput を定義
    Input currentInput = inputManager_.GetInput(keys, preKeys);

    // ★チャージ状態の更新と方向差し替え
    UpdateChargeStates(currentFrame / 60.0f, currentInput);

    // currentInput が方向キーのときのみチャージ置き換えを検討
    if (currentInput >= Input::Up && currentInput <= Input::DownLeft) {
        for (const auto& [dir, state] : chargeStates) {
            if (state.isValid) {
                // 「成立中のチャージ方向」と「現在の入力方向」が一致しているときだけ置き換える
                switch (dir) {
                case Direction::Down:  if (currentInput == Input::Down)  currentInput = Input::ChargeDown;  break;
                case Direction::Up:    if (currentInput == Input::Up)    currentInput = Input::ChargeUp;    break;
                case Direction::Left:  if (currentInput == Input::Left)  currentInput = Input::ChargeLeft;  break;
                case Direction::Right: if (currentInput == Input::Right) currentInput = Input::ChargeRight; break;
                }
            }
        }
    }

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
        Input bufferedInput = currentInput;

        for (const auto& [dir, state] : chargeStates) {
            if (state.isValid) {
                switch (dir) {
                case Direction::Down:  if (currentInput == Input::Down)  bufferedInput = Input::ChargeDown;  break;
                case Direction::Up:    if (currentInput == Input::Up)    bufferedInput = Input::ChargeUp;    break;
                case Direction::Left:  if (currentInput == Input::Left)  bufferedInput = Input::ChargeLeft;  break;
                case Direction::Right: if (currentInput == Input::Right) bufferedInput = Input::ChargeRight; break;
                }
            }
        }

        inputBuffer_.PushInput(bufferedInput, currentFrame);
    }

    // バッファが古くなったら削除（最大60F保持）
    inputBuffer_.Update(currentFrame);

    // 次フレーム用に保持
    lastInput = currentInput;
}

// 特定のコマンドが入力されているかをチェック
bool CommandInput::CheckCommand(const Command& command) {
    // ① まずチャージ条件を満たしているか確認
    for (const auto& cond : command.chargeConditions) {
        Direction dir;
        switch (cond.input) {
        case Input::ChargeDown: dir = Direction::Down; break;
        case Input::ChargeUp: dir = Direction::Up; break;
        case Input::ChargeLeft: dir = Direction::Left; break;
        case Input::ChargeRight: dir = Direction::Right; break;
        default: return false;
        }

        const auto& state = chargeStates.at(dir);
        if (!state.isValid) return false;
    }

    // ② 通常の入力シーケンスのチェック（Up→Kickなど）
    const std::vector<Input>& sequence = command.sequence;
    int maxFrameGap = command.maxFrameGap;

    std::queue<std::pair<Input, int>> tempQueue = inputBuffer_.GetBufferCopy();
    std::vector<std::pair<Input, int>> inputs;
    while (!tempQueue.empty()) {
        inputs.push_back(tempQueue.front());
        tempQueue.pop();
    }

    size_t seqIndex = 0;
    int lastMatchedFrame = -1;

    for (const auto& [input, frame] : inputs) {
        if (seqIndex >= sequence.size()) break;

        if (input == sequence[seqIndex]) {
            if (lastMatchedFrame != -1 && frame - lastMatchedFrame > maxFrameGap) {
                return false;
            }
            lastMatchedFrame = frame;
            seqIndex++;
        }
    }

    return (seqIndex == sequence.size());
}

// バッファクリア（再スタート時など）
void CommandInput::ClearBuffer() {
    inputBuffer_.Clear();
    spinDetector_.Clear();
    lastInput = Input::None;
}

void CommandInput::UpdateChargeStates(float now, Input rawDirection) {
    auto getDirection = [](Input input) -> std::optional<Direction> {
        switch (input) {
        case Input::Down:       return Direction::Down;
        case Input::Up:         return Direction::Up;
        case Input::Left:       return Direction::Left;
        case Input::Right:      return Direction::Right;
        case Input::ChargeDown: return Direction::Down;
        case Input::ChargeUp:   return Direction::Up;
        case Input::ChargeLeft: return Direction::Left;
        case Input::ChargeRight:return Direction::Right;
        default: return std::nullopt;
        }
    };

    for (auto& [dir, state] : chargeStates) {
        bool isHeld = getDirection(rawDirection) == dir;

        if (isHeld) {
            if (!state.isCharging) {
                state.isCharging = true;
                state.startTime = now;
            }

            float chargeTime = now - state.startTime;
            if (!state.isValid && chargeTime >= chargeThreshold) {
                state.isValid = true;
                state.validTime = now;
            }
        } else {
            if (state.isCharging) {
                state.isCharging = false;
                if (state.isValid) {
                    state.validTime = now;
                }
            }

            if (state.isValid && now - state.validTime > chargeKeepTime) {
                state.isValid = false;
            }
        }
    }
}

// 入力 → 文字列
std::string CommandInput::InputToString(Input input) const {
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
std::string CommandInput::GetInputHistory() const {
    return inputBuffer_.GetInputHistory();
}
