#include "CommandInput.h"
#include <Novice.h>
#include <cmath>
#include <set>
#include <algorithm> // std::unique

CommandInput::CommandInput()
    : currentFrame(0), lastSpinFrame(-9999), lastInput(Input::None) {
}

void CommandInput::Update(const char* keys, const char* preKeys) {
    currentFrame++;

    // 現在の入力を取得
    Input currentInput = GetInput(keys, preKeys);

    // 表示用履歴：前回と異なる入力のみ記録（Noneも1つだけ記録する）
    if (inputHistory.empty() || inputHistory.back() != currentInput) {
        inputHistory.push_back(currentInput);
        if (inputHistory.size() > 20) {
            inputHistory.erase(inputHistory.begin());
        }
    }

    // 方向キー履歴：Spin検出用（重複記録防止）
    if (currentInput <= Input::UpLeft && currentInput != Input::None) {
        if (directionHistory.empty() || directionHistory.back() != currentInput) {
            directionHistory.push_back(currentInput);
            if (directionHistory.size() > 40) {
                directionHistory.erase(directionHistory.begin());
            }
        }
    }

    // Spin入力検出（10F以上空けて再検出）
    if (currentFrame - lastSpinFrame > 10) {
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
            }
            lastSpinFrame = currentFrame;
            directionHistory.clear(); // 検出後はリセット
        }
    }

    // 入力バッファ：None以外＆前回と違うときだけ記録（長押し無効化）
    if (currentInput != Input::None) {
        inputBuffer.push({ currentInput, currentFrame });
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
    const std::vector<Input>& sequence = command.sequence;  // 判定したいコマンド入力列
    int maxFrameGap = command.maxFrameGap;                  // コマンド間の最大フレーム許容差

    // 入力バッファのコピーを作成（元のinputBufferはqueueで順番保証）
    std::queue<std::pair<Input, int>> tempQueue = inputBuffer;

    // バッファの内容をvectorに展開（重複除去はしない）
    std::vector<std::pair<Input, int>> inputs;
    while (!tempQueue.empty()) {
        inputs.push_back(tempQueue.front());
        tempQueue.pop();
    }

    size_t seqIndex = 0;        // コマンドの現在チェック中の入力インデックス
    int lastMatchedFrame = -1;  // 最後に一致した入力のフレーム番号

    // 入力バッファ内を順に走査してコマンドと比較
    for (const auto& [input, frame] : inputs) {
        if (seqIndex >= sequence.size()) break;  // コマンド全入力が一致済みなら終了

        if (input == sequence[seqIndex]) {
            // フレーム差チェック（最初の一致以降のみ判定）
            if (lastMatchedFrame != -1 && frame - lastMatchedFrame > maxFrameGap) {
                // フレーム間隔が広すぎてコマンド不成立
                return false;
            }

            // 一致した入力のフレーム番号を更新し、次のコマンド入力へ
            lastMatchedFrame = frame;
            seqIndex++;
        }
    }

    // コマンド全シーケンスにマッチしたかを返す
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

    bool up = keys[DIK_SPACE];
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

    // 上下左右をすべて含んでいる必要あり
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

    int spinCount = static_cast<int>(std::abs(totalAngle) / 360.0f);

    if (spinCount > 0 && hasUp && hasDown && hasLeft && hasRight) {
        return spinCount;
    }

    return 0;
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
