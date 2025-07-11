#pragma once
#include <queue>
#include <vector>
#include <string>
#include "Command.h"

class InputBuffer {
public:
    void PushInput(Input input, int frame);
    void Update(int currentFrame);
    void Clear();
    std::string GetInputHistory() const;
    std::queue<std::pair<Input, int>> GetBufferCopy() const;

private:
    std::queue<std::pair<Input, int>> inputBuffer_;  // 入力バッファ（Input, Frame）
    std::vector<Input> inputHistory_;                // 表示用履歴
    int maxBufferDuration_ = 60;                     // バッファの保持フレーム数
    int maxHistorySize_ = 20;                        // 表示履歴の最大数
};