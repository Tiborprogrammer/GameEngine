/*#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

int readImage() {
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);


    // Step 1: Generate texture buffer
    unsigned int texture;
    glGenTextures(1, &texture);

    // Step 2: say we want to operate on the newly created texture buffer
    glBindTexture(GL_TEXTURE_2D, texture);

    // Step 2a: step params on texture (interpolation and filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Step 3: fill the texture buffer with the "wood" image we loaded
    if (data) {
        glTexImage2D(
                GL_TEXTURE_2D, // Type of texture
                0,  // Mipmap level (don't need to care for now)
                GL_RGB,  // Store data in buffer in RGB format
                width, height,
                0, // Legacy
                GL_RGB, // Image data is also in RGB format
                GL_UNSIGNED_BYTE, // image data is stored as unsigned bytes (e.g. chars)
                data // pointer to the image data
        );
        glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmap (for efficiency, will explain later)
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Free the image
    stbi_image_free(data);






    // USING THE TEXTURE
    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    //
    glVertexAttribPointer(
            1,  // Location
            2,  // Number of values in the texture (2: float U and float V)
            GL_FLOAT, // Each value is a float
            GL_FALSE, // No normalization
            5 * sizeof(float),  // to go from one UV coordinate to another, advance by 5 floats
            (void*)(3 * sizeof(float))  // the first UV coordinate starts after 3 floats
    );
    glEnableVertexAttribArray(1); // Activate location 1 (



    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    return 0;
}*/