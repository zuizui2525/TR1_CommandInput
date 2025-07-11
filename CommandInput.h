#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <map>
#include "Command.h"
#include "InputManager.h"
#include "InputBuffer.h"
#include "SpinDetector.h"

// コマンド入力検出クラス
class CommandInput {
public:
    CommandInput();

    void Update(const char* keys, const char* preKeys);         // 毎フレームの入力処理
    bool CheckCommand(const Command& command);                  // 特定のコマンドが入力されたかチェック
    void ClearBuffer();                                         // バッファ初期化
    std::string GetInputHistory() const;                        // 入力履歴の文字列を取得

private:
    InputBuffer inputBuffer_;
    SpinDetector spinDetector_;
    int lastSpinFrame_ = -9999;
    int currentFrame;                                           // 現在のフレーム
    Input lastInput;                                            // 前フレームの入力

    InputManager inputManager_;
    std::string InputToString(Input input) const;               // Input → 表示用文字列に変換
   
    std::map<Direction, ChargeState> chargeStates = {
     { Direction::Down,  {} },
     { Direction::Up,    {} },
     { Direction::Left,  {} },
     { Direction::Right, {} },
    };

    // 現在対象としている技のチャージ条件
    float chargeThreshold = 0.75f;    // 溜めに必要な時間 45F = 0.75秒
    float chargeKeepTime = 0.5f;     // 溜め入力の保存時間 30F = 0.5秒
    void UpdateChargeStates(float now, Input rawDirection);
};
