#pragma once
enum KeyType
{
    KEY_UP,
    KEY_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_QUIT,
    KEY_UNKNOWN
};
KeyType readKey();