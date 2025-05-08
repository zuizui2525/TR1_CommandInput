#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

enum class Input {
    None,
    Down,
    Right,
    Left,
    DownRight,
    DownLeft,
    Punch,
    Kick
};

struct Command {
    std::vector<Input> sequence;
    int maxFrameGap; // 各入力間の猶予フレーム
};

class CommandInput {
public:
    CommandInput();
    void Update(const char* keys, const char* preKeys);
    bool CheckCommand(const Command& command);
    void ClearBuffer(); // 入力バッファをクリアするメソッド
    std::string GetInputHistory() const; // 入力履歴を取得するメソッド

private:
    std::queue<std::pair<Input, int>> inputBuffer; // 入力とそのフレーム数
    std::vector<Input> inputHistory; // 入力履歴
    int currentFrame;

    Input GetInput(const char* keys, const char* preKeys);
    std::string InputToString(Input input) const; // Input を文字列に変換するヘルパー関数
};