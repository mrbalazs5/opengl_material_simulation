#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Application::Application(const char* windowTitle) {
    this->windowTitle = windowTitle;
}

void onWindowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  

void onMousePosChange(GLFWwindow* window, double xPos, double yPos) {
    Application *app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    app->mouseXPos = xPos;
    app->mouseYPos = yPos;
}

void onMouseClick(GLFWwindow* window, int button, int action, int mods){
    Application *app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if (state == GLFW_PRESS) {
        app->leftMouseButtonPressed = 1;
    } else {
        app->leftMouseButtonPressed = 0;
    }
}

void Application::handleKeyboard() {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, GL_TRUE);
    }

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        this->sandMode = 1;
        this->wallMode = 0;
        this->eraserMode = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        this->sandMode = 0;
        this->wallMode = 1;
        this->eraserMode = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        this->sandMode = 0;
        this->wallMode = 0;
        this->eraserMode = 1;
    }
}

Config* Application::loadConfig(const char* filePath) {
    xml_document<> doc;
	xml_node<>* rootNode;
    int windowHeight, windowWidth;

    ifstream configFile (filePath);
    vector<char> buffer((istreambuf_iterator<char>(configFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    rootNode = doc.first_node("config");

    xml_node<> * windowNode = rootNode->first_node("window");

    windowWidth = atoi(windowNode->first_attribute("width")->value());
    windowHeight = atoi(windowNode->first_attribute("height")->value());

    return new Config(windowWidth, windowHeight);
}

void Application::prepareScene() {}

void Application::run() {
    this->init();

    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    float vertices[] = {
        // position         //texture
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);

    glGenBuffers(1, &this->EBO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader("shaders/shader.vs", "shaders/shader.fs");

    unsigned int texture[2];
    glGenTextures(2, (GLuint*) &texture);
    float textureBorderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTexImage2D(
        GL_TEXTURE_2D,
        0, 
        GL_RGBA, 
        this->config->windowWidth, 
        this->config->windowHeight, 
        0, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        NULL
    );

    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RGBA, 
        this->config->windowWidth, 
        this->config->windowHeight, 
        0, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        NULL
    );

    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);

    glGenFramebuffers(1, &this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texture[1], 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(this->window)) {
        this->handleKeyboard();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        shader.use();
  
        shader.setVec2(
            "mousePosU",
            this->mouseXPos / this->config->windowWidth,
            1.0 - (this->mouseYPos / this->config->windowHeight)
        );
        shader.setInt("mouseLeftButtonU", this->leftMouseButtonPressed);
        shader.setInt("sandModeU", this->sandMode);
        shader.setInt("wallModeU", this->wallMode);
        shader.setInt("eraserModeU", this->eraserMode);

        this->src = this->activeBuffer;
        this->dest = 1 - this->activeBuffer;
        this->activeBuffer = 1 - this->activeBuffer;

        glBindVertexArray(this->VAO);

        glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

        GLuint buffs[] = { GL_COLOR_ATTACHMENT0 + (GLuint)dest };
        glDrawBuffers(1, buffs);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[src]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[src]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    this->close();
}

void Application::init() {
	this->config = this->loadConfig("config.xml");

    if (!glfwInit()) {
        throw runtime_error("Failed to init GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    this->window = glfwCreateWindow(
        this->config->windowWidth,
        this->config->windowHeight,
        windowTitle, NULL, NULL
    );
    
    if (!this->window) {
        glfwTerminate();

        throw runtime_error("Failed to create window.");
    }

    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw runtime_error("Failed to initialize GLAD.");
    }    

    glfwSetWindowUserPointer(this->window, this);

    glfwSetCursorPosCallback(this->window, onMousePosChange);
    glfwSetMouseButtonCallback(window, onMouseClick);
    glfwSetFramebufferSizeCallback(this->window, onWindowResize);  
}

void Application::close() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}