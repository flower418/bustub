//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// count_min_sketch.cpp
//
// Identification: src/primer/count_min_sketch.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/count_min_sketch.h"

#include <stdexcept>
#include <string>

namespace bustub {

/**
 * Constructor for the count-min sketch.
 *
 * @param width The width of the sketch matrix.
 * @param depth The depth of the sketch matrix.
 * @throws std::invalid_argument if width or depth are zero.
 */
template <typename KeyType>
CountMinSketch<KeyType>::CountMinSketch(uint32_t width, uint32_t depth) : width_(width), depth_(depth) {
  /** @TODO(student) Implement this function! */
  if (width == 0 || depth == 0) {
    throw std::invalid_argument("Width and depth must be positive");
  }

  table_.resize(depth_, std::vector<uint32_t>(width_, 0));
  /** @spring2026 PLEASE DO NOT MODIFY THE FOLLOWING */
  // Initialize seeded hash functions
  hash_functions_.reserve(depth_);
  for (size_t i = 0; i < depth_; i++) {
    hash_functions_.push_back(this->HashFunction(i));
  }
}

template <typename KeyType>
CountMinSketch<KeyType>::CountMinSketch(CountMinSketch &&other) noexcept : width_(other.width_), depth_(other.depth_) {
  // 移动构造，使用右值引用
  /** @TODO(student) Implement this function! */
  table_ = std::move(other.table_);
  hash_functions_ = std::move(other.hash_functions_);
}

template <typename KeyType>
// 对等号进行重载
auto CountMinSketch<KeyType>::operator=(CountMinSketch &&other) noexcept -> CountMinSketch & {
  /** @TODO(student) Implement this function! */
  // 不能复制自己
  // this 是一个指向对象本身的指针
  if (this == &other) {
    return *this;
  }

  // 移动资源
  this->width_ = other.width_;
  this->depth_ = other.depth_;

  // 移动需要拷贝的资源
  this->table_ = std::move(other.table_);
  this->hash_functions_ = std::move(other.hash_functions_);

  return *this;
}

template <typename KeyType>
void CountMinSketch<KeyType>::Insert(const KeyType &item) {
  /** @TODO(student) Implement this function! */
  std::lock_guard<std::mutex> lock(mutex_);
  for (uint32_t row = 0; row < depth_; row++) {
    uint32_t col = hash_functions_[row](item) % width_;
    table_[row][col]++;
  }
}

template <typename KeyType>
void CountMinSketch<KeyType>::Merge(const CountMinSketch<KeyType> &other) {
  if (width_ != other.width_ || depth_ != other.depth_) {
    throw std::invalid_argument("Incompatible CountMinSketch dimensions for merge.");
  }
  /** @TODO(student) Implement this function! */
  for (uint32_t row = 0; row < depth_; row++) {
    for (uint32_t col = 0; col < width_; col++) {
      table_[row][col] += other.table_[row][col];
    }
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::Count(const KeyType &item) const -> uint32_t {
  uint32_t min_value = INT_MAX;
  for (uint32_t row = 0; row < depth_; row++) {
    uint32_t col = hash_functions_[row](item) % width_;
    uint32_t freq = table_[row][col];

    if (freq < min_value) {
      min_value = freq;
    }
  }

  return min_value;
}

template <typename KeyType>
void CountMinSketch<KeyType>::Clear() {
  /** @TODO(student) Implement this function! */
  for (auto &row : table_) {
    std::fill(row.begin(), row.end(), 0);
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::TopK(uint16_t k, const std::vector<KeyType> &candidates)
    -> std::vector<std::pair<KeyType, uint32_t>> {
  /** @TODO(student) Implement this function! */
  for (auto const &candidate : candidates) {
  }
}

// Explicit instantiations for all types used in tests
template class CountMinSketch<std::string>;
template class CountMinSketch<int64_t>;  // For int64_t tests
template class CountMinSketch<int>;      // This covers both int and int32_t
}  // namespace bustub
