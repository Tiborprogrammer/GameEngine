//
// Created by Tibor Varga on 03/09/2020.
//

#include "Application.h"

#include <memory>

void Application::receiveEvent(Event& event) {

    switch (event.eventType) {
        case EventType::WindowClose :
            OnWindowClose(event);
            break;

        case EventType::WindowResize :
            std::cout << "Resize: " << event.eventData.resizeData.width << "  " << event.eventData.resizeData.height << "\n";
            break;

        case EventType::MouseMoved :
            std::cout << "Mouse moved: " << event.eventData.mouseMovedData.x << "  " << event.eventData.mouseMovedData.y << "\n";
            break;

        case EventType::MouseButtonReleased:
            std::cout << "Mouse button released" << "\n";
            break;

        case EventType::MouseButtonPressed:
            std::cout << "Mouse button pressed" << "\n";
            break;

        case EventType::KeyPressed:
            std::cout << "Key pressed: " << event.eventData.keyPressed.key << "  " << event.eventData.keyPressed.isFirstTime << "\n";
            break;

        case EventType::KeyReleased:
            std::cout << "Key released: " << event.eventData.keyReleased << "\n";
            break;

        default:
            break;


    }
}

Application::Application() {
    window = std::make_unique<Window>();
    window->setEventProcessingFn([this](Event& event) {
        this->receiveEvent(event);
    });
}

void Application::run() {
    while (running) {
        window->update();
    }
}

void Application::OnWindowClose(Event &e) {
    window->shutDown();
    running = false;
}
