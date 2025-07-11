#include "CommandChecker.h"

CommandChecker::CommandChecker(InputBuffer& inputBuffer, ChargeManager& chargeManager)
    : inputBuffer_(inputBuffer), chargeManager_(chargeManager) {
}

bool CommandChecker::CheckCommand(const Command& command) {
    // ① チャージ条件チェック
    for (const auto& cond : command.chargeConditions) {
        if (!chargeManager_.IsChargeValid(cond.input)) return false;
    }

    // ② 入力シーケンスチェック
    const auto& sequence = command.sequence;
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

    return seqIndex == sequence.size();
}
