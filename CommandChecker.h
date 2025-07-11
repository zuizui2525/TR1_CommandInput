#pragma once
#include <queue>
#include <vector>
#include "Command.h"
#include "InputBuffer.h"
#include "ChargeManager.h"

class CommandChecker {
public:
    CommandChecker(InputBuffer& inputBuffer, ChargeManager& chargeManager);

    // コマンド成立チェック
    bool CheckCommand(const Command& command);

private:
    InputBuffer& inputBuffer_;
    ChargeManager& chargeManager_;
};