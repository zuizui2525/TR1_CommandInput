#pragma once
#include "Command.h"

class InputManager {
public:
    // 入力状態から Input を取得する
    Input GetInput(const char* keys, const char* preKeys);
};