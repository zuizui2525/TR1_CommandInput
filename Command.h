#pragma once
#include <vector>
#include <utility>

enum class Direction {
    Down, Up, Left, Right
};

enum class Input {
    None,
    Up, Down, Left, Right,
    UpRight, UpLeft, DownRight, DownLeft,
    Punch, Kick, Spin,
    ChargeDown, ChargeUp, ChargeLeft, ChargeRight
};

struct ChargeCondition {
    Input input;        //!< 溜め方向
};

struct ChargeState {
    bool isCharging = false;
    bool isValid = false;
    float startTime = 0.0f;
    float validTime = 0.0f;
};

struct Command {
    std::vector<Input> sequence;
    int maxFrameGap;
    std::vector<ChargeCondition> chargeConditions;
};