//
// Created by Tibor Varga on 03/09/2020.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include "Window.h"
#include "events/Event.h"

class Application {
    std::unique_ptr<Window> window;
    bool running = true;

public:
    Application();

    void OnWindowClose(Event& e);
    void receiveEvent(Event& event);

    void run();
};


#endif //GAMEENGINE_APPLICATION_H
