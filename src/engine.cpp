#include "engine.h"

#include <iostream>
#include <exception>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "mesh.h"
#include "arcballcamera.h"

static const char* glsl_version = "#version 330 core";
static bool mousePressed;
// static glm::vec3 lastPos;
static glm::vec3 cameraPosition{ 2.0f, 2.0f, 2.0f };

static double currentMouseX, currentMouseY;

static ArcballCamera camera{ 1024.0f, 768.0f };

static 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mousePressed = true;
        glfwGetCursorPos(window, &currentMouseX, &currentMouseY);
        camera.StartDrag(static_cast<float>(currentMouseX), static_cast<float>(currentMouseY));
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mousePressed = false;
        camera.StopDrag();
    }
}

static
void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        glfwGetCursorPos(window, &currentMouseX, &currentMouseY);
        camera.Drag(static_cast<float>(currentMouseX), static_cast<float>(currentMouseY));
    }
}

Engine::Engine(int width, int height) 
    : mWidth{ width }, mHeight{ height } {
    if (!glfwInit()) {
        std::cerr << "Error initializing glfw3\n";
        throw std::system_error(std::error_code(), "Error initializing glfw3");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    mWindow = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
    if (!mWindow) {
        std::cerr << "Error creating glfw3 window\n";
        glfwTerminate();
        throw std::system_error(std::error_code(), "Error creating glfw3 window");
    }

    glfwMakeContextCurrent(mWindow);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        throw std::system_error(std::error_code(), "Error initializing glfw3");
    }
}

Engine::~Engine() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Engine::Init() {
    // Make sure glfw callbacks are called before imgui inits
    glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
    glfwSetCursorPosCallback(mWindow, mouse_pos_callback);
    glfwSetKeyCallback(mWindow, key_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Engine::Run() {
    Init();

    auto mesh = Mesh();
    mesh.Load("assets/meshes/cube.obj");

    Shader shader{"assets/shaders/model.vs", "assets/shaders/model.fs"};
    shader.SetFloat("outColor", 1.0f);

    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float speed = 3.0f;
    float mouseSpeed = 0.005f;
    double lastTime = glfwGetTime();

    // glm::vec3 cameraPosition{ 2.0f, 2.0f, 2.0f };
    glm::vec3 objPosition{ 0.0f, 0.0f, 0.0f };
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 rotationAxis{ 0.0f, 1.0f, 0.0f };
    float rotationAngle{};
    glm::quat objRotation{};
    glm::vec3 cameraDir{};

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(mWindow)) {
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        glfwPollEvents();

        glClearColor(0.39f, 0.58f, 0.93f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        
         GUI();

        ImGui::Begin("Camera");;
        ImGui::DragFloat3("Obj pos", glm::value_ptr(objPosition), 0.01f);
        // ImGui::DragFloat3("Obj rot axis", glm::value_ptr(rotationAxis), 0.01f);
        // ImGui::DragFloat("Obj rot angle", &rotationAngle, 0.01f);
        ImGui::DragFloat4("Obj rotation", glm::value_ptr(objRotation), 0.01f);
        ImGui::End();

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), objPosition);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3{ 1.0f, 1.0f, 1.0f });
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4{1.0f}, rotationAngle, rotationAxis);
        // glm::mat4 rotationMatrix = glm::mat4_cast(objRotation);

        glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        // glm::vec3 cameraTarget{ 0.0f, 0.0f, 0.0f };
        // const glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, up);
        // glm::quatLookAt(cameraDir, up);
        const glm::mat4 viewMatrix = camera.GetViewMatrix();

        const float fieldOfView = 100.0f;
        const float nearClip = 0.1f;
        const float farClip = 100.0f;

        const auto winWidth = static_cast<float>(mWidth);
        const auto winHeight = static_cast<float>(mHeight);

        glm::mat4 projectionMatrix = glm::perspective(
            glm::radians(fieldOfView),  // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
            winWidth / winHeight,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar?
            nearClip,                   // Near clipping plane. Keep as big as possible, or you'll get precision issues.
            farClip                     // Far clipping plane. Keep as little as possible.
        );

        const auto modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;

        shader.Use();
        shader.SetMatrix("mvp", modelViewProjection);

        glBindVertexArray(mesh.GetVAO());
        
        glDrawElements(GL_TRIANGLES, mesh.GetIndicesCount(), GL_UNSIGNED_INT, nullptr);
        Render();
        glfwSwapBuffers(mWindow);
    }
}

void Engine::GUI() {
    // Imgui here
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Engine::Render() {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(mWindow, &display_w, &display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}