// @author: lx
// @time: 2022/10/11
// @brief: 

#ifndef GAME_MOUSEEVENT_H
#define GAME_MOUSEEVENT_H

#include "Event.h"
#include "Core/MouseCodes.h"


namespace Asura {
    // MouseMoved Event
    class ASURA_API MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y): m_mouseX(x), m_mouseY(y) {}

        // get mouse x position
        inline float getX() const {
            return m_mouseX;
        }
        // get mouse y position
        inline float getY() const {
            return m_mouseY;
        }

        // override toString
        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: (" << m_mouseX << ", " << m_mouseY << ")";
            return ss.str();
        }

        // use marco defined in Event.h to add type and category functions
        EVENT_CLASS_TYPE(MouseButtonMoved);
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

    private:
        float m_mouseX, m_mouseY;  // mouse position
    };

    // MouseScroll Event
    class ASURA_API MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOffset, float yOffset): m_xOffset(xOffset), m_yOffset(yOffset) {}

        inline float getXOffset() const {
            return m_xOffset;
        }
        inline float getYOffset() const {
            return m_yOffset;
        }

        EVENT_CLASS_TYPE(MouseScrolled);

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_xOffset, m_yOffset;
    };

    // base class MouseButton Event
    class ASURA_API MouseButtonEvent : public Event {
    public:
        inline MouseCode getMouseButton() const {
            return m_button;
        }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

    protected:
        explicit MouseButtonEvent(int button): m_button(button) {}

        MouseCode m_button;
    };

    // MouseButtonPressed Event
    class ASURA_API MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonPressedEvent(int button): MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed);
    };

    // MouseButtonReleased Event
    class ASURA_API MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonReleasedEvent(int button): MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased);
    };
}


#endif //GAME_MOUSEEVENT_H
