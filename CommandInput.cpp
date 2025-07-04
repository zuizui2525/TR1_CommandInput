﻿#include "CommandInput.h"
#include <Novice.h>
#include <cmath>
#include <set>
#include <algorithm> // std::unique
#include <optional>

CommandInput::CommandInput()
    : currentFrame(0), lastSpinFrame(-9999), lastInput(Input::None) {
}

void CommandInput::Update(const char* keys, const char* preKeys) {
    currentFrame++;

    // 入力を取得して currentInput を定義
    Input currentInput = GetInput(keys, preKeys);

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


    // 表示用履歴：前回と異なる入力のみ記録（Noneも1つだけ記録する）
    if (inputHistory.empty() || inputHistory.back() != currentInput) {
        inputHistory.push_back(currentInput);
        if (inputHistory.size() > 20) {
            inputHistory.erase(inputHistory.begin());
        }
    }

    // 方向キー履歴：Spin検出用（重複記録防止）
    if (currentInput <= Input::DownLeft && currentInput != Input::None) {
        if (directionHistory.empty() || directionHistory.back() != currentInput) {
            directionHistory.push_back(currentInput);
            if (directionHistory.size() > 40) {
                directionHistory.erase(directionHistory.begin());
            }
        }
    }

    // Spin入力検出（3F以上空けて再検出）
    if (currentFrame - lastSpinFrame > 3) {
        int spinCount = DetectSpinCount();
        if (spinCount > 0) {
            for (int i = 0; i < spinCount; ++i) {
                inputBuffer.push({ Input::Spin, currentFrame });

                // 履歴にも追加（表示用）
                if (inputHistory.empty() || inputHistory.back() != Input::Spin) {
                    inputHistory.push_back(Input::Spin);
                    if (inputHistory.size() > 20) {
                        inputHistory.erase(inputHistory.begin());
                    }
                }
                RemoveOneSpinFromDirectionHistory(); // 検出後はリセット
            }
            lastSpinFrame = currentFrame;
        }
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

        inputBuffer.push({ bufferedInput, currentFrame });
    }

    // バッファが古くなったら削除（最大60F保持）
    while (!inputBuffer.empty() && currentFrame - inputBuffer.front().second > 60) {
        inputBuffer.pop();
    }

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

    std::queue<std::pair<Input, int>> tempQueue = inputBuffer;
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
    while (!inputBuffer.empty()) inputBuffer.pop();
    inputHistory.clear();
    directionHistory.clear();
    lastInput = Input::None;
}

// キー入力から方向・ボタン入力を判定
Input CommandInput::GetInput(const char* keys, const char* preKeys) {
    if (keys[DIK_U] && !preKeys[DIK_U]) return Input::Punch;
    if (keys[DIK_J] && !preKeys[DIK_J]) return Input::Kick;

    bool up = keys[DIK_SPACE] || keys[DIK_W];
    bool down = keys[DIK_S];
    bool left = keys[DIK_A];
    bool right = keys[DIK_D];

    if (up && right) return Input::UpRight;
    if (up && left) return Input::UpLeft;
    if (down && right) return Input::DownRight;
    if (down && left) return Input::DownLeft;
    if (up) return Input::Up;
    if (down) return Input::Down;
    if (left) return Input::Left;
    if (right) return Input::Right;

    return Input::None;
}

// Inputを角度に変換（Spin検出用）
float CommandInput::GetAngle(Input dir) {
    switch (dir) {
    case Input::Right: return 0.0f;
    case Input::UpRight: return 45.0f;
    case Input::Up: return 90.0f;
    case Input::UpLeft: return 135.0f;
    case Input::Left: return 180.0f;
    case Input::DownLeft: return 225.0f;
    case Input::Down: return 270.0f;
    case Input::DownRight: return 315.0f;
    default: return -1.0f;
    }
}

// 時計回り1回転を簡易的に検出（未使用）
bool CommandInput::DetectSpin() {
    return false;
}

// 回転の角度から回転回数を判定
int CommandInput::DetectSpinCount() {
    if (directionHistory.size() < 5) return 0;

    float totalAngle = 0.0f;
    std::set<Input> passedDirs;

    for (size_t i = 1; i < directionHistory.size(); ++i) {
        float a1 = GetAngle(directionHistory[i - 1]);
        float a2 = GetAngle(directionHistory[i]);
        if (a1 < 0 || a2 < 0) continue;

        float delta = a2 - a1;
        if (delta > 180.0f) delta -= 360.0f;
        if (delta < -180.0f) delta += 360.0f;
        totalAngle += delta;

        passedDirs.insert(directionHistory[i]);
    }

    auto contains = [&](std::initializer_list<Input> group) {
        for (Input dir : group) {
            if (passedDirs.count(dir)) return true;
        }
        return false;
        };

    bool hasUp = contains({ Input::Up, Input::UpRight, Input::UpLeft });
    bool hasDown = contains({ Input::Down, Input::DownRight, Input::DownLeft });
    bool hasRight = contains({ Input::Right, Input::UpRight, Input::DownRight });
    bool hasLeft = contains({ Input::Left, Input::UpLeft, Input::DownLeft });

    int spinCount = static_cast<int>(std::abs(totalAngle) / 330.0f);

    int dirCount = 0;
    if (hasUp) dirCount++;
    if (hasDown) dirCount++;
    if (hasRight) dirCount++;
    if (hasLeft) dirCount++;

    if (spinCount >= 1 && dirCount >= 4) {
        return spinCount;
    }

    return 0;
}

void CommandInput::RemoveOneSpinFromDirectionHistory() {
    if (directionHistory.size() < 5) return;

    float totalAngle = 0.0f;
    size_t removeCount = 0;

    for (size_t i = 1; i < directionHistory.size(); ++i) {
        float a1 = GetAngle(directionHistory[i - 1]);
        float a2 = GetAngle(directionHistory[i]);
        if (a1 < 0 || a2 < 0) continue;

        float delta = a2 - a1;
        if (delta > 180.0f) delta -= 360.0f;
        if (delta < -180.0f) delta += 360.0f;

        totalAngle += delta;
        removeCount++;

        if (std::abs(totalAngle) >= 330.0f) {
            break;
        }
    }

    // removeCount + 1 は、開始点も含めて削除
    if (removeCount + 1 < directionHistory.size()) {
        directionHistory.erase(directionHistory.begin(), directionHistory.begin() + removeCount + 1);
    } else {
        directionHistory.clear();
    }
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
    std::string history;
    for (const auto& input : inputHistory) {
        history += InputToString(input) + " ";
    }
    return history;
}
