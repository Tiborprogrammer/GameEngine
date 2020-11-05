//
// Created by Tibor Varga on 05/11/2020.
//

#ifndef GAMEENGINE_LAYER_H
#define GAMEENGINE_LAYER_H

class Window;

class Layer {
    Window& window;

public:
    Layer(Window& window);
    virtual void Update() = 0;
    virtual void Render() = 0;

    virtual ~Layer() = 0;
};


#endif //GAMEENGINE_LAYER_H
