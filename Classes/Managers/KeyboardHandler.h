#pragma once
#include "base/CCRef.h"
#include "base/CCEventKeyboard.h"
#include <unordered_map>

struct StringHasher {
    size_t operator()(const cocos2d::EventKeyboard::KeyCode& t) const {
        return (size_t) t;
    }
};

namespace cocos2d
{
class Node;
}

class KeyboardHandler : public cocos2d::Ref
{
	using KeyCode = cocos2d::EventKeyboard::KeyCode;
public:
	KeyboardHandler(cocos2d::Node*);

	std::unordered_map<KeyCode, bool, StringHasher> pressingKeys;
};
