#pragma once
#include <map>
#include <optional>
#include "Command.h"

class ChargeManager {
public:
    void UpdateChargeState(float now, Input rawDirection);           // チャージ状態の更新
    Input GetEffectiveInput(Input input) const;                      // チャージ成立中なら置き換え
    bool IsChargeValid(Direction chargeInput) const;                     // 指定Inputのチャージが有効か
    void Clear();                                                    // 初期化

private:
    std::map<Direction, ChargeState> chargeStates_ = {
        { Direction::Down, {} },
        { Direction::Up, {} },
        { Direction::Left, {} },
        { Direction::Right, {} },
    };

    float chargeThreshold_ = 0.75f;   // 溜めに必要な時間
    float chargeKeepTime_ = 0.5f;     // チャージ状態維持時間

    std::optional<Direction> ToDirection(Input input) const;
};
