// @author: lx
// @time: 2022/10/11
// @brief: 

#ifndef GAME_APPLICATIONEVENT_H
#define GAME_APPLICATIONEVENT_H

#include "Event.h"


namespace Asura {
    // Application Tick Event
    class ASURA_API ApplicationTickEvent : public Event {
    public:
        ApplicationTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick);

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    // Application Update Event
    class ASURA_API ApplicationUpdateEvent : public Event {
    public:
        ApplicationUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    // Application Render Event
    class ASURA_API ApplicationRenderEvent : public Event {
    public:
        ApplicationRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    // Window Close Event
    class ASURA_API WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    // Window Resize Event
    class ASURA_API WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(int width, int height): m_width(width), m_height(height) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: (" << m_width << ", " << m_height << ")";
            return ss.str();
        }

        [[nodiscard]] inline int getWidth() const {
            return m_width;
        }

        [[nodiscard]] inline int getHeight() const {
            return m_height;
        }

        EVENT_CLASS_TYPE(WindowResize)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        int m_width, m_height;
    };

    // Window Focus Event
    class ASURA_API WindowFocusEvent : public Event {
    public:
        WindowFocusEvent() = default;

        EVENT_CLASS_TYPE(WindowFocus)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    // Window Lost Focus Event
    class ASURA_API WindowLostFocusEvent : public Event {
    public:
        WindowLostFocusEvent() = default;

        EVENT_CLASS_TYPE(WindowLostFocus)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    // Window Moved Event
    class ASURA_API WindowMovedEvent : public Event {
    public:
        WindowMovedEvent(int xPos, int yPos): m_xPos(xPos), m_yPos(yPos) {}

        EVENT_CLASS_TYPE(WindowMoved)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss <<  "WindowMovedEvent: " << "(" << m_xPos << ", " << m_yPos << ")";
            return ss.str();
        }

    private:
        int m_xPos, m_yPos;
    };
}

#endif //GAME_APPLICATIONEVENT_H
