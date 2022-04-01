
#include "kdtree3/kdtree3.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <variant>

using Eigen::Vector3f;

namespace kdtree {

kdtree3::kdtree3(const std::vector<Vector3f> &points)
    : tree_(points.size() * 2) {
  auto start = points.cbegin();
  tree_[1] = *start;

  // todo: figure out how insert should handle duplicates.
  std::for_each(std::next(start), points.cend(),
                [this](const Vector3f &point) { insert(point); });
  //   [this](const decltype(points::value_type) &point) { insert(point); });

  //   for (auto itr = std::next(start); itr != points.end(); ++itr) {
  //     insert()
  //   }
}

auto kdtree3::insert(const Vector3f &point) -> bool {
  auto index = 1;
  return insert_(point, index);
  //   if (root_ == nullptr) {
  //     *root_ = auto kdtree3_node{point, 0, nullptr, nullptr};
  //   }

  //   return true;
}

inline auto kdtree3::calculate_left_child_idx_(unsigned int parent_idx) const
    -> unsigned int {
  return parent_idx * 2;
}
inline auto kdtree3::calculate_right_child_idx_(unsigned int parent_idx) const
    -> unsigned int {
  return parent_idx * 2 + 1;
}
auto kdtree3::inorder_traversal_(
    std::function<void(const Eigen::Vector3f &)> fn,
    unsigned int node_idx) const -> void {
  auto node = tree_[node_idx];
  try {
    auto point = std::get<Vector3f>(node);
    fn(point);
    inorder_traversal_(fn, calculate_left_child_idx_(node_idx));
    inorder_traversal_(fn, calculate_right_child_idx_(node_idx));
  } catch (const std::bad_variant_access &ex) {
  }
}

auto kdtree3::inorder_traversal(
    std::function<void(const Eigen::Vector3f &)> fn) const -> void {
  inorder_traversal_(fn, 1);
}

auto kdtree3::is_leaf_(unsigned int idx) const -> bool {
  auto left_child_index = idx * 2;
  auto right_child_index = idx * 2 + 1;
  if (left_child_index > tree_.size() || right_child_index > tree_.size()) {
    return false;
  }
  auto right_child = tree_[right_child_index];
  auto left_child = tree_[left_child_index];
  // ! fix: i don't like this. too brittle
  auto left_child_empty = left_child.index() == 0;
  auto right_child_empty = right_child.index() == 0;
  return left_child_empty && right_child_empty;
}

auto kdtree3::has_children_(unsigned int idx) const -> bool {
  return !is_leaf_(idx);
}

auto kdtree3::get_parent_idx_(unsigned int child_index) const -> unsigned int {
  auto is_even = child_index % 2 == 0;
  auto child_is_left_of_parent = is_even;
  return child_index - (child_is_left_of_parent ? 0 : 1) / 2;
}

auto kdtree3::depth_(unsigned int index) const -> unsigned int {
  return std::floor(std::log2(index));
}

auto kdtree3::resize_tree_to_double_its_size_() -> void {
  //   tree_.resize(tree_.size() * 2, std::monostate);

  tree_.resize(tree_.size() * 2, std::monostate);
  std::cout << "resize tree to double its size. new size: " << tree_.size() * 2
            << std::endl;
}

auto kdtree3::insert_(const Vector3f &point, unsigned int index) -> bool {
  std::cout << "index is " << index << " and tree size is " << tree_.size()
            << std::endl;
  if (index > tree_.size() - 1) {
    resize_tree_to_double_its_size_();
  }
  assert(0 < index && index < tree_.size());

  const auto &node_at_index = tree_.at(index);
  auto index_points_at_leaf = node_at_index.index() == 0;
  if (index_points_at_leaf) {

    tree_[index] = point;
    ++n_nodes_;
    return true;
  }

  auto depth = depth_(index);
  auto ordering = compare_nodes_based_on_depth(point, node_at_index, depth);
  switch (ordering) {
  case partial_ordering::less_than: // go left
  {
    auto left_child_index = index * 2;
    insert_(point, left_child_index); // recurse left
  } break;
  case partial_ordering::greater_than: // go right
  {
    auto right_child_index = index * 2 + 1;
    insert_(point, right_child_index); // recurse right
  } break;
  case partial_ordering::equal: // dunno
    break;
  };

  return false;
}

} // namespace kdtree
