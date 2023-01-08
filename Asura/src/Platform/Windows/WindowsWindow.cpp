#include "WindowsWindow.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "GLFW/glfw3.h"
#include "Core/Input.h"


bool Asura::WindowsWindow::s_glfwInit = false;

Asura::WindowsWindow::WindowsWindow(const Asura::WindowProps &windowProps) {
    ASURA_PROFILE_FUNCTION();
    init(windowProps);
}

Asura::WindowsWindow::~WindowsWindow() {
    ASURA_PROFILE_FUNCTION();
    shutDown();
}


void Asura::WindowsWindow::onUpdate() {
    ASURA_PROFILE_FUNCTION();
    glfwPollEvents();
    m_context->swapBuffers();
}

void Asura::WindowsWindow::setVSync(bool enable) {
    ASURA_PROFILE_FUNCTION();
    // set vertical sync
    if(enable) {
        glfwSwapInterval(1);
    }
    else {
        glfwSwapInterval(0);
    }
    m_data.bVSync = enable;
}

void Asura::WindowsWindow::setEventCallback(const Asura::Window::EventCallbackFun &callback) {
    m_data.eventCallback = callback;
}

void Asura::WindowsWindow::init(const Asura::WindowProps &props) {
    ASURA_PROFILE_FUNCTION();
    m_data.Title = props.Title;
    m_data.Width = props.Width;
    m_data.Height = props.Height;

    // initialize glfw
    if(!s_glfwInit) {
        s_glfwInit = true;
        bool flag = glfwInit();
        ASURA_CORE_ASSERT(flag, "Unable to initialize GLFW");

        //set glfw error callback
        glfwSetErrorCallback([](int error_code, const char* description) {
            ASURA_CORE_ERROR("GLFW ERROR ({0}): {1}", error_code, description);
        });
    }

    m_window = glfwCreateWindow((int)m_data.Width, (int)m_data.Height, m_data.Title.c_str(), nullptr, nullptr);
    if(!m_window) {
        glfwTerminate();
    }

    // make context
    m_context = OpenGLContext::createGraphicsContext(m_window);
    m_context->init();
    // set user defined data structure
    glfwSetWindowUserPointer(m_window, &m_data);
    setVSync(true);

    ASURA_CORE_INFO("createWindow window \"{0}\": ({1}, {2})", m_data.Title, m_data.Width, m_data.Height);

    setGLFWWindowEventCallback();
    setGLFWMouseEventCallback();
    setGLFWKeyEventCallback();
    setGLFWApplicationEventCallback();
}

void Asura::WindowsWindow::shutDown() {
    ASURA_PROFILE_FUNCTION();
    glfwDestroyWindow(m_window);
}

void Asura::WindowsWindow::setGLFWWindowEventCallback() {
    // set glfw window resize callback
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Asura::WindowResizeEvent event(width, height);
        data.Width = width;
        data.Height = height;
        data.eventCallback(event);  // call WindowsWindow callback function
    });

    // set glfw window close callback
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Asura::WindowCloseEvent event;
        data.eventCallback(event);  // call WindowsWindow callback function
    });

    // set glfw window move callback
    glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int xPos, int yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Asura::WindowMovedEvent event(xPos, yPos);
        data.eventCallback(event);
    });


    glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (focused) {
            case GLFW_TRUE: {
                Asura::WindowFocusEvent event;
                data.eventCallback(event);
                break;
            }
            case GLFW_FALSE: {
                Asura::WindowLostFocusEvent event;
                data.eventCallback(event);
                break;
            }
        }
    });
}

void Asura::WindowsWindow::setGLFWMouseEventCallback() {
    // set glfw mouse button callback
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS: {
                Asura::MouseButtonPressedEvent event(button);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                Asura::MouseButtonReleasedEvent event(button);
                data.eventCallback(event);
                break;
            }
        }
    });

    // set glfw scroll event callback
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Asura::MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.eventCallback(event);
    });

    // set glfw mouse move event callback
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Asura::MouseMovedEvent event((float)xPos, (float)yPos);
        data.eventCallback(event);
    });
}

void Asura::WindowsWindow::setGLFWKeyEventCallback() {
    // set glfw key events callback
    glfwSetKeyCallback(m_window,[](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        // press release and repeat
        switch(action) {
            case GLFW_PRESS: {
                Asura::KeyPressedEvent event(key, false);
                data.eventCallback(event);
                break;
            }
                break;
            case GLFW_RELEASE: {
                Asura::KeyReleasedEvent event(key);  // call WindowsWindow callback
                data.eventCallback(event);  // call WindowsWindow callback function
                break;
            }
            case GLFW_REPEAT: {
                Asura::KeyPressedEvent event(key, true);
                data.eventCallback(event);
                break;
            }
        }
    });
}

void Asura::WindowsWindow::setGLFWApplicationEventCallback() {
    // set
//    glfwSetWindowRefreshCallback(m_window, [](GLFWwindow* window) {
//        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
//        Asura::ApplicationTickEvent event;
//        data.eventCallback(event);
//    });
}

void Asura::WindowsWindow::setGLFWCharEventCallback() {
    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codePoint) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Asura::KeyTypedEvent event(codePoint);
        data.eventCallback(event);
    });
}
