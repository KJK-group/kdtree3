
#include "kdtree3/kdtree3.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

using Eigen::Vector3f;

namespace kdtree {

kdtree3::kdtree3(const std::vector<Vector3f> &points)
    : tree_(points.size() * 2) {
  auto start = points.cbegin();
  tree_[1] = *start;

  // todo: figure out how insert should handle duplicates.
  std::for_each(std::next(start), points.cend(),
                [this](const points::value_type &point) { insert(point); });

  //   for (auto itr = std::next(start); itr != points.end(); ++itr) {
  //     insert()
  //   }
}

kdtree3::insert(const Vector3f &point)->bool {
  auto index = 1;
  return insert_(point, index);
  //   if (root_ == nullptr) {
  //     *root_ = kdtree3_node{point, 0, nullptr, nullptr};
  //   }

  //   return true;
}

kdtree3::get_parent_idx_(unsigned int child_index) const->unsigned int {
  auto is_even = child_index % 2 == 0;
  auto child_is_left_of_parent = is_even;
  return child_index - (child_is_left_of_parent ? 0 : 1) / 2;
}

kdtree3::depth_(unsigned int index)->float {
  return std::floor(std::log2(index));
}

kdtree3::resize_tree_to_double_its_size_() {
  tree_.resize(tree_.size() * 2, ::empty);
}

kdtree3::insert_(const Vector3f &point, unsigned int index)->bool {
  assert(0 < index && index < tree_.size());

  const auto &node_at_index = tree_.at(index);
  auto index_points_at_leaf = node_at_index.index == 0;
  if (index_points_at_leaf) {
    if (index == tree_.size() - 1) {
      resize_tree_to_double_its_size_();
    }
    tree_.[index] = point;
    ++n_nodes_;
    return true;
  }

  auto depth = depth_(index);
  auto ordering = compare_nodes_based_on_depth(point, point_at_index, index);
  switch (ordering) {
  case partial_ordering::less_than: // go left
    auto left_child_index = index * 2;
    insert_(point, left_child_index); // recurse left
    break;
  case partial_ordering::greater_than: // go right
    auto right_child_index = index * 2 + 1;
    insert_(point, right_child_index); // recurse right
    break;
  case partial_ordering::equal: // dunno
    break;
  }

  return false;
}

} // namespace kdtree