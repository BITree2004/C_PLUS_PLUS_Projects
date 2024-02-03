#include "circular-buffer.h"
#include "element.h"
#include "test-utils.h"

#include <gtest/gtest.h>

#include <utility>

TEST(traits_test, iterator_constructor) {
  EXPECT_FALSE((std::is_constructible_v<container::iterator, std::nullptr_t>))
      << "iterator should not be constructible from nullptr\n";
  EXPECT_FALSE((std::is_constructible_v<container::const_iterator, std::nullptr_t>))
      << "const_iterator should not be constructible from nullptr\n";

  EXPECT_FALSE((std::is_constructible_v<container::reverse_iterator, std::nullptr_t>))
      << "reverse_iterator should not be constructible from nullptr\n";
  EXPECT_FALSE((std::is_constructible_v<container::reverse_iterator, std::nullptr_t>))
      << "reverse_iterator should not be constructible from nullptr\n";
}

TEST(traits_test, iterator_triviality) {
  EXPECT_TRUE(std::is_trivial_v<container::iterator>);
  EXPECT_TRUE(std::is_trivial_v<container::const_iterator>);
}

TEST(traits_test, iterator_member_types) {
  using iterator = container::iterator;
  using const_iterator = container::const_iterator;

  EXPECT_TRUE((std::is_same_v<element, iterator::value_type>));
  EXPECT_TRUE((std::is_same_v<element&, iterator::reference>));
  EXPECT_TRUE((std::is_same_v<element*, iterator::pointer>));
  EXPECT_TRUE((std::is_same_v<std::ptrdiff_t, iterator::difference_type>));
  EXPECT_TRUE((std::is_same_v<std::random_access_iterator_tag, iterator::iterator_category>));

  EXPECT_TRUE((std::is_same_v<element, const_iterator::value_type>));
  EXPECT_TRUE((std::is_same_v<const element&, const_iterator::reference>));
  EXPECT_TRUE((std::is_same_v<const element*, const_iterator::pointer>));
  EXPECT_TRUE((std::is_same_v<std::ptrdiff_t, const_iterator::difference_type>));
  EXPECT_TRUE((std::is_same_v<std::random_access_iterator_tag, const_iterator::iterator_category>));
}
