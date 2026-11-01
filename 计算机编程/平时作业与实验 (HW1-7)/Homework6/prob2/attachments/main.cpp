#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <ostream>
/**
 * @brief Given a vector of strings @c strings, reduce this vector so that each
 * string appears only once. After the function call, @c strings contains the
 * same set of elements (possibly sorted) as the original vector, but each
 * element appears only once.
 *
 * @param strings The given vector of strings.
 */
void dropDuplicates(std::vector<std::string> &strings) {
  std::sort(strings.begin(), strings.end());
  strings.erase(std::unique(strings.begin(), strings.end()), strings.end());
}

/**
 * @brief Given a vector of strings @c strings and a nonnegative integer @c k,
 * partition the vector into two parts (with the elements rearranged) so that
 * the first part contains all the strings with length no greater than @c k and
 * the second part contains the rest of the strings. Return an iterator just
 * past the last element of the first part.
 *
 * @param strings The given vector of strings.
 * @return An iterator just past the last element of the first part. In
 * particular, if the second part is not empty, the returned iterator will refer
 * to the first element of the second part.
 */
auto partitionByLength(std::vector<std::string> &strings, std::size_t k) {
  return std::partition(strings.begin(), strings.end(), [k](const std::string &s) { return s.size() <= k; });
}

/**
 * @brief Generate all the permutations of {1, 2, ..., n} in lexicographical
 * order, and print them to @c os.
 *
 */

//Write a function generatePermutations(n, os), where n is a positive integer and os is an ostream, that generates all the permutations of {1,2,⋯.n} in lexicographical order and print them to os.

void generatePermutations(int n, std::ostream &os) {
  std::vector<int> numbers(n);
  // TODO: Fill `numbers` with {1, 2, ..., n}.
  std::iota(numbers.begin(), numbers.end(), 1);

  do {
    // TODO: Print the numbers, separated by a space.
    std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(os, " "));
    os << '\n';
  } while (std::next_permutation(numbers.begin(), numbers.end()));
}

int main() {
  std::vector<std::string> words{"fox", "jumps", "over", "quick", "red", "slow", "the", "turtle"};
  auto pos = partitionByLength(words, 4);
  for (auto it = words.begin(); it != pos; ++it)
    std::cout << *it << ' ';
  std::cout << std::endl;
  generatePermutations(3, std::cout);
  return 0;
}

