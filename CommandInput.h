#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

// 入力の種類を定義（方向＋ボタン＋特殊）
enum class Input {
    None,
    Down,
    Right,
    Left,
    Up,
    DownRight,
    DownLeft,
    UpRight,
    UpLeft,
    Punch,
    Kick,
    Spin  // 一回転入力を抽象化
};

// コマンド構造体（コマンドの入力列と許容フレーム）
struct Command {
    std::vector<Input> sequence; // 入力の並び（例：↓↓+P）
    int maxFrameGap;             // 入力間の最大許容フレーム
};

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
};
