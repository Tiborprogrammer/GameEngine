//
// Created by Tibor Varga on 03/09/2020.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include <memory>
#include "Window.h"
#include "events/Event.h"

class Application {
    Window* window;
    bool running = true;

public:
    Application();

    void OnWindowClose(Event& e);
    void receiveEvent(Event& event);

    float movementSpeed = 1;
    float xOffset = 0.0;
    float yOffset = 0.0;
    Vector2 mousePositionAbsolute = {0, 0};

    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    void run();
};


#endif //GAMEENGINE_APPLICATION_H
