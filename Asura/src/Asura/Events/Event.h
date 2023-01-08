// @author: lx
// @time: 2022/10/11
// @brief: 

#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "Asura/Core/Core.h"

#include <string>
#include <sstream>
#include <functional>


namespace Asura {
    /**
     * @brief event type
     */
    enum class EventType {
        None = 0x00,

        // Window events
        WindowClose = 0x01,
        WindowResize = 0x02,
        WindowFocus = 0x03,
        WindowLostFocus = 0x04,
        WindowMoved = 0x05,

        // Application events
        AppTick = 0x06,
        AppUpdate = 0x07,
        AppRender = 0x08,

        // Key events
        KeyPressed = 0x09,
        KeyReleased = 0x0a,
        KeyTyped = 0xf,

        // Mouse events
        MouseButtonPressed = 0x0b,
        MouseButtonReleased = 0x0c,
        MouseButtonMoved = 0x0d,
        MouseScrolled = 0x0e,
    };



    /**
     * @brief event category, use shift left to set the value of category,
     * so that we can use bitwise operations to get multiple category events.
     */
    enum EventCategory {
        None = 0,
        EventCategoryApplication = 0b1,
        EventCategoryInput = 0b1 << 1,
        EventCategoryKeyboard = 0x1 << 2,
        EventCategoryMouse = 0x1 << 3,
        EventCategoryMouseButton = 0x1 << 4,
    };
}


namespace Asura {
/*
 * ## 防止宏展开
 * # 转为字符串
 */

// use marco to simplify override of getEventType and getName, and also add definition of getStaticType
#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; } \
                                virtual EventType getEventType() const override { return getStaticType(); } \
                                virtual const char* getName() const override { return #type; }

// use marco to simplify override of getCategoryFlags
#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }


    /**
     * @brief Event Base class
     */
    class ASURA_API Event {
    public:
        friend class EventDispatcher;

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual int getCategoryFlags() const = 0;

        /**
         * @brief default returns getName, could be override in derived class
         * @return event message
         */
        virtual std::string toString() const {
            std::stringstream ss;
            ss << getName() << "Event";
            return ss.str();
        }

        [[nodiscard]] inline bool isInCategory(EventCategory category) const {
            // use bitwise operation
            return getCategoryFlags() & category;
        }

        /**
         *
         * @return if the event is handled
         */
        [[nodiscard]] inline bool handled() const {
            return m_handled;
        }
        inline void setHandled(bool flag) {
            m_handled = flag;
        }

    protected:
        bool m_handled = false;  // if the event is handled
    };


    /**
     * @brief event dispatcher
     */
    class ASURA_API EventDispatcher {
    public:
        template<typename T>
        using EventFun = std::function<bool(T&)>;

        explicit EventDispatcher(Event& event): m_event(event) {}

        /*!
         * @brief Application binds callback(onEvent) to events in platform specific window, in onEvent function,
         * dispatcher dispatches event to different functions depends on the category of event.
         * @tparam T derived classes of event
         * @param func handle function
         * @return if the event is handled
         */
        template<typename T>
        bool dispatch(EventFun<T> func) {
            if(m_event.m_handled) {
                return m_event.m_handled;
            }

            if(m_event.getEventType() == T::getStaticType()) {
                m_event.m_handled = func(*(T*)&m_event);
            }
            return m_event.handled();
        }

    private:
        Event& m_event;
    };

    inline std::ostream & operator<<(std::ostream& os, const Event& event) {
        return os << event.toString();
    }
}


#endif //GAME_EVENT_H
