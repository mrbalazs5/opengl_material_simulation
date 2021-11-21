#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <rapidxml/rapidxml.hpp>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <streambuf>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Config.h"
#include "Shader.h"

using namespace rapidxml;
using namespace std;

class Application {
        public:
            Application(const char* windowTitle);
            void run();

            double mouseXPos, mouseYPos;
            int leftMouseButtonPressed;
            int sandMode, wallMode, eraserMode;
            unsigned int VAO, VBO, EBO, FBO;
            int src, dest;
            int activeBuffer = 0;

        protected:
            const char* windowTitle;

        private:
            GLFWwindow* window;
            Config* config;

            Config* loadConfig(const char* filePath);
            void init();
            void handleKeyboard();
            void prepareScene();
            void close();
            void renderScene();
};

#endif //APPLICATION_H