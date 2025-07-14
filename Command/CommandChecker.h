#pragma once
#include <queue>
#include <vector>
#include "Command.h"
#include "InputBuffer.h"
#include "ChargeManager.h"

class CommandChecker {
public:
    // コンストラクタに必要な参照を受け取る
    CommandChecker(const InputBuffer* inputBuffer, const ChargeManager* chargeManager);

    // 指定されたコマンドが成立しているかを判定
    bool CheckCommand(const Command& command) const;

private:
    const InputBuffer* inputBuffer_;
    const ChargeManager* chargeManager_;
};
