#include "ChargeManager.h"

void ChargeManager::UpdateChargeState(float now, Input rawDirection) {
    for (auto& [dir, state] : chargeStates_) {
        bool isHeld = ToDirection(rawDirection) == dir;

        if (isHeld) {
            if (!state.isCharging) {
                state.isCharging = true;
                state.startTime = now;
            }

            float chargeTime = now - state.startTime;
            if (!state.isValid && chargeTime >= chargeThreshold_) {
                state.isValid = true;
                state.validTime = now;
            }
        } else {
            if (state.isCharging) {
                state.isCharging = false;
                if (state.isValid) {
                    state.validTime = now;
                }
            }

            if (state.isValid && now - state.validTime > chargeKeepTime_) {
                state.isValid = false;
            }
        }
    }
}

Input ChargeManager::GetEffectiveInput(Input input) const {
    auto dirOpt = ToDirection(input);
    if (!dirOpt.has_value()) return input;

    Direction dir = dirOpt.value();
    const auto& state = chargeStates_.at(dir);
    if (state.isValid) {
        switch (dir) {
        case Direction::Down: return Input::ChargeDown;
        case Direction::Up: return Input::ChargeUp;
        case Direction::Left: return Input::ChargeLeft;
        case Direction::Right: return Input::ChargeRight;
        }
    }

    return input;
}

bool ChargeManager::IsChargeValid(Input chargeInput) const {
    Direction dir;
    switch (chargeInput) {
    case Input::ChargeDown: dir = Direction::Down; break;
    case Input::ChargeUp: dir = Direction::Up; break;
    case Input::ChargeLeft: dir = Direction::Left; break;
    case Input::ChargeRight: dir = Direction::Right; break;
    default: return false;
    }

    const auto& state = chargeStates_.at(dir);
    return state.isValid;
}

void ChargeManager::Clear() {
    for (auto& [_, state] : chargeStates_) {
        state = ChargeState{};
    }
}

std::optional<Direction> ChargeManager::ToDirection(Input input) const {
    switch (input) {
    case Input::Down:
    case Input::ChargeDown:
        return Direction::Down;
    case Input::Up:
    case Input::ChargeUp:
        return Direction::Up;
    case Input::Left:
    case Input::ChargeLeft:
        return Direction::Left;
    case Input::Right:
    case Input::ChargeRight:
        return Direction::Right;
    default:
        return std::nullopt;
    }
}
