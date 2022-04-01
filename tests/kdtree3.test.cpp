
#include "eigen3/Eigen/Dense"
#include <iostream>
#include <vector>

#include "kdtree3/kdtree3.hpp"

using namespace std;

// int main(int argc, char **argv) { return 0; }

int main(int argc, char const *argv[]) {

  vector<Eigen::Vector3f> points = {
      {0.0, 0.0, 0.0}, {1.0, 6.0, 0.0},  {2.0, -5., 2.0},
      {8.5, 1.0, 9.0}, {-1.0, 40, -1.0}, {2.0, -5., 6.0},
  };

  std::cout << "calling constructor" << std::endl;
  auto kdtree3 = kdtree::kdtree3(points);

  std ::cout << "can construct kdtree3" << std::endl;

  kdtree3.inorder_traversal([](const Eigen::Vector3f &point) {
    std::cout << point.x() << " " << point.y() << " " << point.z() << std::endl;
  });
  return 0;
}
