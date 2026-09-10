#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>

GLFWwindow *g_window = nullptr;
GLuint g_shaderProgram = 0;
GLuint g_vao = 0;

const char *vertexShaderSource = R"(#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 vColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    vColor = aColor;
})";

const char *fragmentShaderSource = R"(#version 300 es
precision mediump float;
in vec3 vColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vColor, 1.0);
})";

GLuint CompileShader(GLenum type, const char *source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);
  return shader;
}

void InitGraphics() {
  GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  g_shaderProgram = glCreateProgram();
  glAttachShader(g_shaderProgram, vs);
  glAttachShader(g_shaderProgram, fs);
  glLinkProgram(g_shaderProgram);

  float vertices[] = {0.0f, 0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -0.5f, -0.5f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  1.0f};

  GLuint vbo;
  glGenVertexArrays(1, &g_vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(g_vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void MainLoop() {
  glfwPollEvents();

  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(g_shaderProgram);
  glBindVertexArray(g_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glfwSwapBuffers(g_window);
}

int main() {
  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  g_window = glfwCreateWindow(800, 600, "C++ WebGL 2 (GLFW)", nullptr, nullptr);
  if (!g_window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(g_window);

  InitGraphics();

  emscripten_set_main_loop(MainLoop, 0, 1);

  glfwDestroyWindow(g_window);
  glfwTerminate();
  return 0;
}
