#pragma once  
#include <vector>  
#include <queue>  
#include <unordered_map>  
#include <string>  
#include <map>  
#include "Command.h"  
#include "InputManager.h"  
#include "InputBuffer.h"  
#include "SpinDetector.h"  
#include "ChargeManager.h"  
#include "CommandChecker.h"  

// コマンド入力検出クラス  
class CommandInput {  
public:  
    CommandInput();  
    ~CommandInput();  

    void Update(const char* keys, const char* preKeys);         // 毎フレームの入力処理  
    void ClearBuffer();                                         // バッファ初期化  
    std::string GetInputHistory() const;                        // 入力履歴の文字列を取得    
    bool CheckCommand(const Command& command);  

private:  
    InputBuffer inputBuffer_;  
    SpinDetector spinDetector_;  
    int lastSpinFrame_ = -9999;  
    int currentFrame;                                           // 現在のフレーム  
    Input lastInput;                                            // 前フレームの入力  

    InputManager inputManager_;  
    std::string InputToString(Input input) const;               // Input → 表示用文字列に変換  

    ChargeManager chargeManager_;  
    CommandChecker* commandChecker_ = nullptr;  
};
