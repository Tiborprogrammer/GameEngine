//
// Created by Tibor Varga on 03/09/2020.
//

#include "Application.h"

#include <memory>

void Application::receiveEvent(Event& event) {
    if (event.eventType == EventType::WindowClose)
        OnWindowClose(event);

    std::cout << event.GetName() << std::endl;
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
