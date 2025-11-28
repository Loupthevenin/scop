#include "../includes/mesh.hpp"
#include "../includes/scop.hpp"
#include <vector>

GLFWwindow *initWindow(int width, int height, const char *title) {
  if (!glfwInit()) {
    std::cerr << "Erreur : impossible d'initialiser GLFW\n";
    return nullptr;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // déjà présent

  GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window) {
    std::cerr << "Erreur : impossible de créer la fenêtre\n";
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Erreur : impossible d'initialiser GLAD\n";
    return nullptr;
  }

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  return window;
}

void createMeshBuffers(const Mesh &mesh, GLuint &vao, GLuint &vbo,
                       GLuint &ebo) {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  // VBO pour les vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vec3),
               &mesh.vertices[0], GL_STATIC_DRAW);

  // EBO pour les indices des faces
  std::vector<unsigned int> indices;
  for (std::vector<Face>::const_iterator it = mesh.faces.begin();
       it != mesh.faces.end(); ++it) {
    const Face &f = *it;
    indices.push_back(f.v1);
    indices.push_back(f.v2);
    indices.push_back(f.v3);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // Attribut position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

// Fonction pour compiler un shader
GLuint compileShader(GLenum type, const char *source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "Erreur compilation shader : " << infoLog << "\n";
  }
  return shader;
}

// Fonction pour créer un shader program minimal
GLuint createShaderProgram() {
  const char *vertexShaderSource = "#version 150 core\n"
                                   "layout(location = 0) in vec3 aPos;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    gl_Position = vec4(aPos, 1.0);\n"
                                   "}\n";

  const char *fragmentShaderSource =
      "#version 150 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "    FragColor = vec4(0.8, 0.3, 0.2, 1.0);\n"
      "}\n";

  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

int main() {
  GLFWwindow *win = initWindow(WIDTH, HEIGHT, "Scop");

  Mesh mesh("logo42.obj");

  GLuint vao, vbo, ebo;
  createMeshBuffers(mesh, vao, vbo, ebo);

  GLuint shaderProgram = createShaderProgram();

  while (!glfwWindowShouldClose(win)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();

  return (0);
}
