#include "SpinDetector.h"
#include <cmath>
#include <set>

void SpinDetector::PushDirection(Input input) {
    if (input <= Input::DownLeft && input != Input::None) {
        if (directionHistory_.empty() || directionHistory_.back() != input) {
            directionHistory_.push_back(input);
            if (directionHistory_.size() > 40) {
                directionHistory_.erase(directionHistory_.begin());
            }
        }
    }
}

float SpinDetector::GetAngle(Input dir) const {
    switch (dir) {
    case Input::Right: return 0.0f;
    case Input::UpRight: return 45.0f;
    case Input::Up: return 90.0f;
    case Input::UpLeft: return 135.0f;
    case Input::Left: return 180.0f;
    case Input::DownLeft: return 225.0f;
    case Input::Down: return 270.0f;
    case Input::DownRight: return 315.0f;
    default: return -1.0f;
    }
}

int SpinDetector::DetectSpinCount() {
    if (directionHistory_.size() < 5) return 0;

    float totalAngle = 0.0f;
    std::set<Input> passedDirs;

    for (size_t i = 1; i < directionHistory_.size(); ++i) {
        float a1 = GetAngle(directionHistory_[i - 1]);
        float a2 = GetAngle(directionHistory_[i]);
        if (a1 < 0 || a2 < 0) continue;

        float delta = a2 - a1;
        if (delta > 180.0f) delta -= 360.0f;
        if (delta < -180.0f) delta += 360.0f;

        totalAngle += delta;
        passedDirs.insert(directionHistory_[i]);
    }

    auto contains = [&](std::initializer_list<Input> group) {
        for (Input dir : group) {
            if (passedDirs.count(dir)) return true;
        }
        return false;
    };

    int dirCount = 0;
    if (contains({ Input::Up, Input::UpRight, Input::UpLeft })) dirCount++;
    if (contains({ Input::Down, Input::DownRight, Input::DownLeft })) dirCount++;
    if (contains({ Input::Left, Input::UpLeft, Input::DownLeft })) dirCount++;
    if (contains({ Input::Right, Input::UpRight, Input::DownRight })) dirCount++;

    int spinCount = static_cast<int>(std::abs(totalAngle) / 270.0f);
    return (spinCount >= 1 && dirCount >= 4) ? spinCount : 0;
}

void SpinDetector::RemoveOneSpin() {
    if (directionHistory_.size() < 5) return;

    float totalAngle = 0.0f;
    size_t removeCount = 0;

    for (size_t i = 1; i < directionHistory_.size(); ++i) {
        float a1 = GetAngle(directionHistory_[i - 1]);
        float a2 = GetAngle(directionHistory_[i]);
        if (a1 < 0 || a2 < 0) continue;

        float delta = a2 - a1;
        if (delta > 180.0f) delta -= 360.0f;
        if (delta < -180.0f) delta += 360.0f;

        totalAngle += delta;
        removeCount++;

        if (std::abs(totalAngle) >= 270.0f) break;
    }

    if (removeCount + 1 < directionHistory_.size()) {
        directionHistory_.erase(directionHistory_.begin(), directionHistory_.begin() + removeCount + 1);
    } else {
        directionHistory_.clear();
    }
}

void SpinDetector::Clear() {
    directionHistory_.clear();
}
