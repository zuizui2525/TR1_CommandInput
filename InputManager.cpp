#include "InputManager.h"
#include <Novice.h>

Input InputManager::GetInput(const char* keys, const char* preKeys) {
    if (keys[DIK_U] && !preKeys[DIK_U]) return Input::Punch;
    if (keys[DIK_J] && !preKeys[DIK_J]) return Input::Kick;

    bool up = keys[DIK_SPACE] || keys[DIK_W];
    bool down = keys[DIK_S];
    bool left = keys[DIK_A];
    bool right = keys[DIK_D];

    if (up && right) return Input::UpRight;
    if (up && left) return Input::UpLeft;
    if (down && right) return Input::DownRight;
    if (down && left) return Input::DownLeft;
    if (up) return Input::Up;
    if (down) return Input::Down;
    if (left) return Input::Left;
    if (right) return Input::Right;

    return Input::None;
}
