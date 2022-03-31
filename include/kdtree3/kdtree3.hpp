#ifndef _KDTREE3_KDTREE3_HPP_
#define _KDTREE3_KDTREE3_HPP_

#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>

#include <variant>

#include "eigen3/Eigen/Dense"

namespace kdtree {

// * no cpp 20 :(
enum class partial_ordering : char { less_than, equal, greater_than };

// class kdtree {
// public:
//   virtual ~kdtree() {}
//   virtual auto insert() const && -> void = 0;
// };

// struct kdtree3_node {
//   Eigen::Vector3f position;
//   unsigned int depth;
//   kdtree3_node *left = nullptr;
//   kdtree3_node *right = nullptr;

//   inline auto x() const -> decltype(position.x()) { return position.x(); }
//   inline auto y() const -> decltype(position.y()) { return position.y(); }
//   inline auto z() const -> decltype(position.z()) { return position.z(); }
//   inline auto xyz() {
//     return std::make_tuple(position.x(), position.y(), position.z());
//   }
// };

using kdtree3_node = Eigen::Vector3f;
using kdtree3_comparator =
    std::function<bool(const kdtree3_node &, const kdtree3_node &)> &;

class kdtree3 {
public:
  kdtree3(const std::vector<Eigen::Vector3f> &points);
  ~kdtree3();

  auto insert(const Eigen::Vector3f &node) -> bool;
  auto remove(const Eigen::Vector3f &node) -> bool;
  auto contains(const Eigen::Vector3f &node) -> bool;

  auto knn_search(unsigned int k, const Eigen::Vector3f &node)
      -> std::vector<Eigen::Vector3f>;
  auto nn_search(const Eigen::Vector3f &node) -> decltype(knn_search(1, node)) {
    return knn_search(1, node);
  }

private:
  // version 1
  kdtree3_node *root_;
  std::priority_queue<kdtree3_node> binary_tree;

  // version 2
  // root_ at idx 0
  // left child at idx parent_idx
  using empty = std::monostate;
  using kdtree3_node_state = std::variant<empty, kdtree3_node>;
  std::vector<kdtree3_node_state> tree_;
  //   std::vector<std::optional<kdtree3_node>> tree_;

  auto comparator_x(const kdtree3_node &n1, const kdtree3_node &n2)
      -> partial_ordering {
    // return n1.x() < n2.x();
    return n1.x() < n2.x() ? partial_ordering::less_than
                           : n1.x() > n2.x() ? partial_ordering::greater_than
                                             : partial_ordering::equal;
  }

  static auto comparator_y(const kdtree3_node &n1, const kdtree3_node &n2)
      -> partial_ordering {

    return n1.y() < n2.y() ? partial_ordering::less_than
                           : n1.y() > n2.y() ? partial_ordering::greater_than
                                             : partial_ordering::equal;
  }

  auto comparator_z(const kdtree3_node &n1, const kdtree3_node &n2)
      -> partial_ordering {
    // return n1.z() < n2.z();
    return n1.z() < n2.z() ? partial_ordering::less_than
                           : n1.z() > n2.z() ? partial_ordering::greater_than
                                             : partial_ordering::equal;
  }

  auto compare_nodes_based_on_depth(const kdtree3_node &n1,
                                    const kdtree3_node &n2, unsigned int depth)
      -> decltype(comparator_z(n1, n2)) {
    switch (depth % 3) {
    case 0:
      return comparator_x(n1, n2);
    case 1:
      return comparator_y(n1, n2);
    case 2:
      return comparator_z(n1, n2);
    }

    std::cerr << "[ERROR]" << __FILE__ << ": " << __LINE__
              << "execution should NOT be able to reach here: " << std::endl;
    std::exit(EXIT_FAILURE);
  }
  auto insert_(const Eigen::Vector3f &, unsigned int depth) -> bool;

  auto rebalance_() -> void;
};

kdtree3::~kdtree3() {}

} // namespace kdtree

#endif // _KDTREE3_KDTREE3_HPP_
