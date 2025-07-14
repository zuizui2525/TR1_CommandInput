#include "CommandChecker.h"

CommandChecker::CommandChecker(const InputBuffer* inputBuffer, const ChargeManager* chargeManager)
    : inputBuffer_(inputBuffer), chargeManager_(chargeManager) {
}

bool CommandChecker::CheckCommand(const Command& command) const {
    // ① チャージ条件を満たしているかを確認
    for (const auto& cond : command.chargeConditions) {
        Direction dir;
        switch (cond.input) {
        case Input::ChargeDown: dir = Direction::Down; break;
        case Input::ChargeUp: dir = Direction::Up; break;
        case Input::ChargeLeft: dir = Direction::Left; break;
        case Input::ChargeRight: dir = Direction::Right; break;
        default: return false;
        }

        if (!chargeManager_->IsChargeValid(dir)) {
            return false;
        }
    }

    // ② 入力シーケンスの照合（順番とフレーム間隔）
    const std::vector<Input>& sequence = command.sequence;
    int maxFrameGap = command.maxFrameGap;

    std::queue<std::pair<Input, int>> buffer = inputBuffer_->GetBufferCopy();
    std::vector<std::pair<Input, int>> inputs;
    while (!buffer.empty()) {
        inputs.push_back(buffer.front());
        buffer.pop();
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
