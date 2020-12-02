//
// Created by Tibor Varga on 05/11/2020.
//

#include "BackgroundLayer.h"
#include "Window.h"

void BackgroundLayer::Render() {
    /*window.setColor(Vector3(0, 255, 0), 255);
    window.drawRect(Vector2(-1, -1), Vector2(2, 0.3));

    window.setColor(Vector3(0, 0, 255), 255);
    window.drawRect(Vector2(-1, -1+0.3), Vector2(2, 1.7));
    */
    Vector5 triangleVertexes[3] = {
            {-1, -1, 0, 0, 0},
            {1, -1, 0, 1, 0},
            {0, 1, 0, 0.5, 1}
            };

    window.drawTextureTriangle(triangleVertexes, "textures/wood.jpg");
}