
#pragma once

#include <vector>

struct Vec3 {
  float x, y, z;
};

struct Face {
  int v1, v2, v3;
};

class Mesh {
private:
  void loadObj(const std::string &filename);

public:
  std::vector<Vec3> vertices;
  std::vector<Face> faces;
  Mesh(const std::string &filename) { loadObj(filename); }
};
