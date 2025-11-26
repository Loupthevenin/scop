#include "../includes/scop.hpp"

int main() {
  GLFWwindow *win = NULL;
  if (!glfwInit())
    return -1;

  win = glfwCreateWindow(800, 600, "Scop", NULL, NULL);
  if (!win)
    return -1;

  glfwMakeContextCurrent(win);

  while (!glfwWindowShouldClose(win)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw here;

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();

  return (0);
}
