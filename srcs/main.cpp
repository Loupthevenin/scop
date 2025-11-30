#include "../includes/mesh.hpp"
#include "../includes/scop.hpp"
#include <vector>

GLFWwindow *initWindow(int width, int height, const char *title) {
  if (!glfwInit()) {
    std::cerr << "Erreur : impossible d'initialiser GLFW\n";
    return nullptr;
  }

#ifdef __APPLE__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // Obligatoire !
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
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
  // Afficher version réelle
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  return window;
}

void createMeshBuffers(const Mesh &mesh, GLuint &vao, GLuint &vbo, GLuint &ebo,
                       GLuint shaderProgram) {

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  // --- VBO
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vec3),
               &mesh.vertices[0], GL_STATIC_DRAW);

  // --- EBO
  std::vector<unsigned int> indices;
  indices.reserve(mesh.faces.size() * 3);

  for (size_t i = 0; i < mesh.faces.size(); ++i) {
    const Face &f = mesh.faces[i];
    indices.push_back(f.v1);
    indices.push_back(f.v2);
    indices.push_back(f.v3);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // --- Attribut aPos
  GLint posLoc = glGetAttribLocation(shaderProgram, "aPos");
  if (posLoc == -1) {
    std::cerr << "Erreur : aPos introuvable dans le shader !" << std::endl;
  }

  glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void *)0);
  glEnableVertexAttribArray(posLoc);

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
  const char *vertexShaderSource = "#version 330 core\n"
                                   "layout(location = 0) in vec3 aPos;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    gl_Position = vec4(aPos, 1.0);\n"
                                   "}\n";

  const char *fragmentShaderSource =
      "#version 330 core\n"
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
  if (!win)
    return 1;

  Mesh mesh("logo42.obj");

  GLuint vao, vbo, ebo;

  GLuint shaderProgram = createShaderProgram();
  glUseProgram(shaderProgram);

  createMeshBuffers(mesh, vao, vbo, ebo, shaderProgram);

  while (!glfwWindowShouldClose(win)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  return 0;
}
