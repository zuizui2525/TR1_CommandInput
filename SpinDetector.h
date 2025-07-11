#pragma once
#include <vector>
#include "Command.h"

class SpinDetector {
public:
    void PushDirection(Input input);   // 方向入力の履歴追加
    int DetectSpinCount();             // 回転回数の検出
    void RemoveOneSpin();              // 検出した回転分だけ履歴から削除
    void Clear();                      // 履歴を初期化

private:
    std::vector<Input> directionHistory_;
    float GetAngle(Input dir) const;   // 方向→角度変換
};
