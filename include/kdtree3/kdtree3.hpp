#ifndef _KDTREE3_KDTREE3_HPP_
#define _KDTREE3_KDTREE3_HPP_

#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>

#include <execution>
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

class kdtree3 {
public:
  kdtree3(const std::vector<Eigen::Vector3f> &points);
  //   ~kdtree3();

  auto insert(const Eigen::Vector3f &node) -> bool;
  auto remove(const Eigen::Vector3f &node) -> bool;
  auto contains(const Eigen::Vector3f &node) -> bool;

  auto knn_search(unsigned int k, const Eigen::Vector3f &node)
      -> std::vector<Eigen::Vector3f>;
  auto nn_search(const Eigen::Vector3f &node) -> decltype(knn_search(1, node)) {
    return knn_search(1, node);
  }

  auto inorder_traversal(std::function<void(const Eigen::Vector3f &)> fn) const
      -> void;

private:
  // version 1
  //   kdtree3_node *root_;
  //   std::priority_queue<kdtree3_node> binary_tree;
  unsigned int n_nodes_ = 0;
  // version 2
  // root_ at idx 0
  // left child at idx parent_idx
  //   using kdtree3_node = Eigen::Vector3f;

  //   using empty = std::monostate;
  //   using kdtree3_node_state = std::variant<empty, kdtree3_node>;
  using kdtree3_node = std::variant<std::monostate, Eigen::Vector3f>;
  std::vector<kdtree3_node> tree_;
  //   using kdtree3_comparator =
  //       std::function<bool(const kdtree3_node &, const kdtree3_node &)> &;

  //   std::vector<std::optional<kdtree3_node>> tree_;

  //   auto comparator_x(const kdtree3_node &n1, const kdtree3_node &n2)
  //       -> partial_ordering {
  //     // return n1.x() < n2.x();
  //     return n1.x() < n2.x() ? partial_ordering::less_than
  //                            : n1.x() > n2.x() ?
  //                            partial_ordering::greater_than
  //                                              : partial_ordering::equal;
  //   }

  //   static auto comparator_y(const kdtree3_node &n1, const kdtree3_node &n2)
  //       -> partial_ordering {

  //     return n1.y() < n2.y() ? partial_ordering::less_than
  //                            : n1.y() > n2.y() ?
  //                            partial_ordering::greater_than
  //                                              : partial_ordering::equal;
  //   }

  //   auto comparator_z(const kdtree3_node &n1, const kdtree3_node &n2)
  //       -> partial_ordering {
  //     // return n1.z() < n2.z();
  //     return n1.z() < n2.z() ? partial_ordering::less_than
  //                            : n1.z() > n2.z() ?
  //                            partial_ordering::greater_than
  //                                              : partial_ordering::equal;
  //   }

  //   auto comparator_x(float n1, float n2) -> partial_ordering {
  //     // return n1.x() < n2.x();
  //     return n1.x() < n2.x() ? partial_ordering::less_than
  //                            : n1.x() > n2.x() ?
  //                            partial_ordering::greater_than
  //                                              : partial_ordering::equal;
  //   }

  //   static auto comparator_y(float n1, float n2) -> partial_ordering {

  //     return n1.y() < n2.y() ? partial_ordering::less_than
  //                            : n1.y() > n2.y() ?
  //                            partial_ordering::greater_than
  //                                              : partial_ordering::equal;
  //   }

  //   auto comparator_z(float n1, float n2) -> partial_ordering {
  //     // return n1.z() < n2.z();
  //     return n1.z() < n2.z() ? partial_ordering::less_than
  //                            : n1.z() > n2.z() ?
  //                            partial_ordering::greater_than
  //                                              : partial_ordering::equal;
  //   }

  auto compare_nodes_based_on_depth(const kdtree3_node &n1,
                                    const kdtree3_node &n2, unsigned int depth)
      //   -> decltype(comparator_z(n1, n2)) {
      -> partial_ordering {
    auto comparison = [](float a, float b) -> partial_ordering {
      return a < b ? partial_ordering::less_than
                   : a > b ? partial_ordering::greater_than
                           : partial_ordering::equal;
    };
    // switch (depth % 3) {
    // case 0:
    //   return comparator_x(n1, n2);
    // case 1:
    //   return comparator_y(n1, n2);
    // case 2:
    //   return comparator_z(n1, n2);
    // }

    try {
      auto p1 = std::get<Eigen::Vector3f>(n1);
      auto p2 = std::get<Eigen::Vector3f>(n2);
      switch (depth % 3) {
      case 0:
        return comparison(p1.x(), p2.x());
      case 1:
        return comparison(p1.y(), p2.y());
      case 2:
        return comparison(p1.z(), p2.z());
      }
    } catch (const std::bad_variant_access &ex) {
      std::cout << ex.what() << '\n';
    }

    std::cerr << "[ERROR]" << __FILE__ << ": " << __LINE__
              << "execution should NOT be able to reach here: " << std::endl;
    std::exit(EXIT_FAILURE);
  }
  auto insert_(const Eigen::Vector3f &, unsigned int depth) -> bool;
  auto is_leaf_(unsigned int idx) const -> bool;
  auto has_children_(unsigned int idx) const -> bool;
  auto get_parent_idx_(unsigned int child_idx) const -> unsigned int;
  auto depth_(unsigned int idx) const -> unsigned int;

  auto resize_tree_to_double_its_size_() -> void;
  auto rebalance_() -> void;
  inline auto calculate_left_child_idx_(unsigned int parent_idx) const
      -> unsigned int;
  inline auto calculate_right_child_idx_(unsigned int parent_idx) const
      -> unsigned int;
  auto inorder_traversal_(std::function<void(const Eigen::Vector3f &)> fn,
                          unsigned int node_idx) const -> void;
};

// kdtree3::~kdtree3() {}

} // namespace kdtree

#endif // _KDTREE3_KDTREE3_HPP_
