#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <map>
#include "Command.h"

// コマンド入力検出クラス
class CommandInput {
public:
    CommandInput();

    void Update(const char* keys, const char* preKeys);         // 毎フレームの入力処理
    bool CheckCommand(const Command& command);                  // 特定のコマンドが入力されたかチェック
    void ClearBuffer();                                         // バッファ初期化
    std::string GetInputHistory() const;                        // 入力履歴の文字列を取得

private:
    std::queue<std::pair<Input, int>> inputBuffer;              // 入力バッファ（Input, Frame）
    std::vector<Input> inputHistory;                            // 表示用履歴（20個まで）
    std::vector<Input> directionHistory;                        // 回転検出用の方向履歴
    int currentFrame;                                           // 現在のフレーム
    int lastSpinFrame;                                          // 最後に回転を検出したフレーム
    Input lastInput;                                            // 前フレームの入力

    Input GetInput(const char* keys, const char* preKeys);      // 現在のキー状態からInputを取得
    std::string InputToString(Input input) const;               // Input → 表示用文字列に変換
    bool DetectSpin();                                          // 時計回り一回転を検出（旧）
    int DetectSpinCount();                                      // 回転の回数を検出（新）
    float GetAngle(Input dir);                                  // 方向を角度で表現
    void RemoveOneSpinFromDirectionHistory();

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
