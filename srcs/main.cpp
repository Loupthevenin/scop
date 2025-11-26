#include "../includes/mesh.hpp"
#include "../includes/scop.hpp"

int main() {
  GLFWwindow *win = NULL;
  if (!glfwInit())
    return -1;

  win = glfwCreateWindow(800, 600, "Scop", NULL, NULL);
  if (!win)
    return -1;

  glfwMakeContextCurrent(win);

  Mesh mesh("logo42.obj");
  while (!glfwWindowShouldClose(win)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::cout << "Vertices: " << mesh.vertices.size() << "\n";
    std::cout << "Faces: " << mesh.faces.size() << "\n";

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();

  return (0);
}
