#include "InputBuffer.h"

void InputBuffer::PushInput(Input input, int frame) {
    if (input == Input::None) return;

    // 前回と同じ入力ならスキップ（長押し防止）
    if (!inputHistory_.empty() && inputHistory_.back() == input) return;

    inputBuffer_.push({ input, frame });
    inputHistory_.push_back(input);

    if (inputHistory_.size() > maxHistorySize_) {
        inputHistory_.erase(inputHistory_.begin());
    }
}

void InputBuffer::Update(int currentFrame) {
    while (!inputBuffer_.empty() &&
        currentFrame - inputBuffer_.front().second > maxBufferDuration_) {
        inputBuffer_.pop();
    }
}

void InputBuffer::Clear() {
    while (!inputBuffer_.empty()) inputBuffer_.pop();
    inputHistory_.clear();
}

std::string InputBuffer::GetInputHistory() const {
    std::string history;
    for (const auto& input : inputHistory_) {
        switch (input) {
        case Input::Up: history += "8 "; break;
        case Input::UpRight: history += "9 "; break;
        case Input::UpLeft: history += "7 "; break;
        case Input::Left: history += "4 "; break;
        case Input::Right: history += "6 "; break;
        case Input::DownLeft: history += "1 "; break;
        case Input::Down: history += "2 "; break;
        case Input::DownRight: history += "3 "; break;
        case Input::ChargeUp: history += "C8 "; break;
        case Input::ChargeDown: history += "C2 "; break;
        case Input::ChargeLeft: history += "C4 "; break;
        case Input::ChargeRight: history += "C6 "; break;
        case Input::Punch: history += "P "; break;
        case Input::Kick: history += "K "; break;
        case Input::Spin: history += "O "; break;
        case Input::None: history += "N "; break;
        default: break;
        }
    }
    return history;
}

std::queue<std::pair<Input, int>> InputBuffer::GetBufferCopy() const {
    return inputBuffer_;  // コマンド判定などに使う
}
