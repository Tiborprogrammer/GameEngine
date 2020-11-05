//
// Created by Tibor Varga on 05/11/2020.
//

#ifndef GAMEENGINE_BACKGROUNDLAYER_H
#define GAMEENGINE_BACKGROUNDLAYER_H

#include "Layer.h"

class BackgroundLayer : public Layer {

public:
    void Render() override;

    BackgroundLayer(Window& window) : Layer(window) {};
};


#endif //GAMEENGINE_BACKGROUNDLAYER_H