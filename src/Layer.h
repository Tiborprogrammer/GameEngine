//
// Created by Tibor Varga on 05/11/2020.
//

#ifndef GAMEENGINE_LAYER_H
#define GAMEENGINE_LAYER_H

class Window;

class Layer {
public:
    Window& window;

    Layer(Window& window);
    virtual void Update() {};
    virtual void Render() {};
};


#endif //GAMEENGINE_LAYER_H
