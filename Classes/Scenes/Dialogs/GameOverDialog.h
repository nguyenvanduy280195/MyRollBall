#pragma once

#include "MyDialog.h"

class GameOverDialog : public MyDialog
{
public:
    bool init();
    void SetOnMainMenuButtonPressed(const std::function<void(cocos2d::Ref*)>&);
    void SetOnReplayButtonPressed(const std::function<void(cocos2d::Ref*)>&);
};