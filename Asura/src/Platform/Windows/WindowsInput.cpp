#include "Core/Input.h"
#include "Core/Application.h"


namespace Asura {
    bool Input::isKeyPressed(KeyCode keyCode) {
        auto window = (GLFWwindow*)Application::getApplication()->getWindow()->getNativeWindow();
        auto state = glfwGetKey(window, static_cast<int>(keyCode));
        return state == GLFW_PRESS;
    }

    bool Input::isMouseButtonPressed(MouseCode mouseCode) {
        auto window = (GLFWwindow*)Application::getApplication()->getWindow()->getNativeWindow();
        auto state = glfwGetMouseButton(window, static_cast<int>(mouseCode));
        return state == GLFW_PRESS;
    }

    float Input::getMouseX() {
        return getMousePosition().x;
    }

    float Input::getMouseY() {
        return getMousePosition().y;
    }

    glm::vec2 Input::getMousePosition() {
        auto window = (GLFWwindow*)Application::getApplication()->getWindow()->getNativeWindow();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return {(float)xPos, (float)yPos};
    }
}
