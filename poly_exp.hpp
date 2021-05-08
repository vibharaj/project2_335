///////////////////////////////////////////////////////////////////////////////
// poly_exp.hpp
//
// Definitions for two algorithms that solve the Maximum Subarray Problem,
// and one algorithm that solves the Subset Sum Problem.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <vector>
#include <math.h>

namespace subarray {

// A summed_span represents a non-empty range of indices inside of a vector of
// ints, stored in a begin iterator and end iterator. The class also stores
// the sum of the ints in that range.
//
// Just like in the rest of the C++ standard library, the range includes all
// elements in [begin, end), or in other words the range includes begin, and all
// elements up to BUT NOT INCLUDING end itself.
class summed_span {
public:
  using iterator = std::vector<int>::const_iterator;

private:
  iterator begin_, end_;
  int sum_;

public:

  // Constructor, given the begin iterator, end iterator, and sum of elements
  // in the range. begin must come before end. sum must be the total of all
  // elements in the range. O(1) time.
  summed_span(iterator begin, iterator end, int sum)
  : begin_(begin), end_(end), sum_(sum) {
    assert(begin < end);
  }

  // Constructor, given only the begin and end iterators. The sum is computed
  // in O(n) time.
  summed_span(iterator begin, iterator end)
  : summed_span(begin, end, std::accumulate(begin, end, 0)) {}

  // Equality tests, two spans are equal when each of their iterators are equal.
  bool operator== (const summed_span& rhs) const {
    return (begin_ == rhs.begin_) && (end_ == rhs.end_);
  }

  // Accessors.
  const iterator& begin() const { return begin_; }
  const iterator& end  () const { return end_  ; }
  int sum() const { return sum_; }

  // Compute the number of elements in the span.
  size_t size() const { return end_ - begin_; }

  // Stream insertion operator, so this class is printable.
  friend std::ostream& operator<<(std::ostream& stream, const summed_span& rhs) {
    stream << "summed_span, size=" << rhs.size() << ", sum=" << rhs.sum();
    return stream;
  }
};

// Compute the maximum subarray of input; i.e. the non-empty contiguous span of
// elements with the maximum sum. input must be nonempty. This function uses an
// exhaustive search algorithm that takes O(n^3) time.
int sum (const std::vector<int>& input, int a, int b){
  int summation = 0;
  for (int i = a; i < b + 1; i++){
    summation += input[i];
  }
  return summation;
}

summed_span max_subarray_exh(const std::vector<int>& input) {
  int b = 0;
  int e = 1;
  int best_sum = input[0];

  std::cout << std::endl;
  assert(!input.empty());


  if (input.size() == 1){
    return summed_span(input.begin(), input.end());
  }

    for (int i = 0; i < input.size() - 1; i++){
      for (int j = i + 1; j < input.size(); j++){
        if (sum(input, i, j) > best_sum){
          b = i;
          e = j;
          best_sum = sum(input, b, e);
        }
        if (input[j] > best_sum){
          best_sum = input[j];
          b = j;
          e = j;
        }
      }
  }
 // first element
  if(best_sum == input[0]){
    b = 0;
    e = 0;
  }
  return summed_span(input.begin() + b, input.begin() + e + 1);
}


// problem 2

// #2

summed_span max_subarray_crossing(const std::vector<int>& V, int low, int middle, int high) {
  // make sure to include c math.
  float left_sum = -(INFINITY);
  float right_sum = -(INFINITY);

  float sum = 0.0;

  int b = 0;
  int e = 0;

  for (int i = middle; i >= low; i--) {
    sum += V[i];

    if (sum > left_sum) {
      left_sum = sum;
      b = i;
    }
  }
  sum = 0.0;
  for (int i = middle + 1; i <= high; i++) {
    sum += V[i];

    if (sum > right_sum) {
      right_sum = sum;
      e = i;
    }
  }

  return summed_span(V.begin() + b, V.begin() + e + 1);
}


summed_span max_subarray_recurse(const std::vector<int>& V, int low, int high) {
  if (low == high) return summed_span(V.begin() + low, V.begin() + (low + 1));

  int middle = (low + high) / 2;

  summed_span entirely_left = max_subarray_recurse(V, low, middle);

  summed_span entirely_right = max_subarray_recurse(V, middle + 1, high);

  summed_span crossing = max_subarray_crossing(V, low, middle, high);

  if(entirely_left.sum() >= entirely_right.sum() && entirely_left.sum() >= crossing.sum()) return entirely_left;

  if(entirely_right.sum() >= entirely_left.sum() && entirely_right.sum() >= crossing.sum()) return entirely_right;

  if(crossing.sum() >= entirely_left.sum() && crossing.sum() >= entirely_right.sum()) return crossing;

  return crossing;
}

// Compute the maximum subarray using a decrease-by-half algorithm that takes
// O(n log n) time.
summed_span max_subarray_dbh(const std::vector<int>& input) {
  assert(!input.empty());

  return max_subarray_recurse(input, 0, input.size() - 1);
}


// Solve the subset sum problem: return a non-empty subset of input that adds
// up to exactly target. If no such subset exists, return an empty optional.
// input must not be empty, and must contain fewer than 64 elements.
// Note that the returned subset must never be empty, even if target == 0.
// This uses an exhaustive search algorithm that takes exponential O(n * 2^n)
// time.
// #3
std::optional<std::vector<int>>
subset_sum_exh(const std::vector<int>& input, int target) {

    assert(!input.empty());

    assert(input.size() < 64);

    int n = input.size();

    uint64_t bits = 0;

    int total_sub = 0;

    for(bits = 0; bits <= pow(2,n); bits++) {
        std::vector<int> cand;

        for (int j = 0 ; j <= n - 1 ; j++) {
            if (((bits>>j) & 1 ) == 1)
                cand.push_back(input[j]);

            total_sub = 0;

            for(int i = 0; i < cand.size(); i++)
                total_sub += cand[i];

            if ((cand.size() > 0) && (total_sub == target))
                return cand;
        }
    }

    return std::nullopt;
 }

}
