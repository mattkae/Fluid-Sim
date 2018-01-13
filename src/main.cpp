#include "gl_includes.h"
#include <iostream>
#include <string>
#include "boost/program_options.hpp"
#include "Logger.h"
#include "Window.h"
#include "Application.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

using namespace std;

// Window variables
const char* WINDOW_TITLE = "Fluid Simulator";
Window* mainWindow = nullptr;

void glfw_error_callback(int error, const char* message) {
  logger::log_error(message);
}

void initialize_glfw_environment(int argc, char** argv) {
  if (!glfwInit()) {
    logger::log_error("Failed to initialize glfw.");
    exit(EXIT_FAILURE);
  }
  
  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initialize_glew() {
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    logger::log_error((const char*)glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char** argv) {
  // Initialization
  initialize_glfw_environment(argc, argv);

  mainWindow = new Window(Application::screenWidth, Application::screenHeight, WINDOW_TITLE);
  mainWindow->set_context();

  initialize_glew();

  Camera camera;
  Shader shader("assets/shader.vert", "assets/shader.frag");
  Model model("assets/test.obj");

  // Loop variables
  double currentTime = 0, prevTime = 0, deltaTime;

  glEnable(GL_DEPTH_TEST);
  
  while (mainWindow->is_running()) {
    // Update timestep
    currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    
    // Get Input
    glfwPollEvents();

    // Update
    move_camera(deltaTime, &camera);
    camera.update(deltaTime);

    // Render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.Use();

    shader.SetUniform3f("ambient", 0.3, 0.3, 0.3);
    shader.SetUniform3f("directionalLight.direction", 0.0, 0.0, -1.0);
    shader.SetUniform3f("directionalLight.color", 1.0, 0.0, 1.0);

    shader.SetUniform3f("pointLight.position", 0.0, 5.0, 0.0);
    shader.SetUniform3f("pointLight.color", 0.0, 0.0, 1.0);
    shader.SetUniform1f("pointLight.constant", 0.55);
    shader.SetUniform1f("pointLight.linear", 0.25);
    shader.SetUniform1f("pointLight.quadratic", 0.1);
    
    camera.render(&shader);
    model.render(&shader);
    mainWindow->swap_buffers();
  }

  // Free window memory
  delete mainWindow;

  // Terminate glfw
  glfwTerminate();
  return 0;
}
