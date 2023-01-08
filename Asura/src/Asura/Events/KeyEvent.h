// @author: lx
// @time: 2022/10/11
// @brief: 

#ifndef GAME_KEYEVENT_H
#define GAME_KEYEVENT_H

#include "Event.h"
#include "Core/KeyCodes.h"


namespace Asura {
    // Key Event base class
    class ASURA_API KeyEvent : public Event {
    public:
        inline KeyCode getKeyCode() const {
            return m_keyCode;
        }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyCode m_keyCode;

        explicit KeyEvent(const KeyCode keyCode): m_keyCode(keyCode) {}
    };


    // Key Pressed Event
    class ASURA_API KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(const KeyCode keyCode, bool isRepeat = false): KeyEvent(keyCode), m_isRepeat(isRepeat) {}

        EVENT_CLASS_TYPE(KeyPressed)

        bool isRepeat() const {
            return m_isRepeat;
        }

    private:
        bool m_isRepeat = false;
    };


    // Key Released Event
    class ASURA_API KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(int keyCode): KeyEvent(keyCode) {}

        EVENT_CLASS_TYPE(KeyReleased)

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keyCode;
            return ss.str();
        }
    };

    //
    class ASURA_API KeyTypedEvent : public KeyEvent {
    public:
        explicit KeyTypedEvent(KeyCode keyCode): KeyEvent(keyCode) {}

        EVENT_CLASS_TYPE(KeyTyped)

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_keyCode;
            return ss.str();
        }
    };
}


#endif //GAME_KEYEVENT_H
