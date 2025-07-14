#include "CommandList.h"

Command hadouken = {
    {Input::Down, Input::DownRight, Input::Right, Input::Punch},
    7
};
Command shoryuken = {
    {Input::Right, Input::Down, Input::DownRight, Input::Punch},
    7
};
Command tatumakisenpukyaku = {
    {Input::Down, Input::DownLeft, Input::Left, Input::Kick},
    7
};
Command spinDriveSmasher = {
    {Input::Down, Input::DownRight, Input::Right, Input::Down, Input::DownRight, Input::Right, Input::Kick},
    7
};
Command sinkuhadouken = {
    {Input::Down, Input::DownRight, Input::Right, Input::Down, Input::DownRight, Input::Right, Input::Punch},
    7
};
Command nage = {
    {Input::Punch, Input::Kick},
    7
};
Command torigurahu = {
    {Input::Down, Input::Down, Input::Punch},
    9
};
Command BolshoiStormBuster = {
    {Input::Spin, Input::Spin, Input::Punch},
    14
};
Command ScrewPileDriver = {
    {Input::Spin, Input::Punch},
    7
};
Command Somersault = {
    {Input::Up, Input::Kick},
    7,
    {{Input::ChargeDown}}
};
Command SonicBoom = {
    {Input::Right, Input::Punch},
    7,
    {{Input::ChargeLeft}}
};
Command BakaWaza = {
    {Input::Punch, Input::Down, Input::DownRight, Input::Right, Input::Down, Input::DownRight, Input::Right, Input::Punch},
    7,
    {{Input::ChargeUp}}
};