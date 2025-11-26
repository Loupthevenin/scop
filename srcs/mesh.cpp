#include "../includes/mesh.hpp"
#include "fstream"
#include <fstream>
#include <iostream>

void Mesh::loadObj(const std::string &filename) {
  std::ifstream file(filename);
  if (file.is_open()) {
    std::cerr << "Erreur: impossible d'ouvrir " << filename << "\n";
    return;
  }
  std::string line;
  while (std::getline(file, line)) {
    if (line.substr(0, 2) == "v ") {
      Vec3 v;
      sscanf(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
      vertices.push_back(v);
    } else if (line.substr(0, 2) == "f ") {
      int a, b, c;
      sscanf(line.c_str(), "f %d %d %d", &a, &b, &c);
      Face f;
      f.v1 = a - 1;
      f.v2 = b - 1;
      f.v3 = c - 1;
      faces.push_back(f);
    }
  }
}
