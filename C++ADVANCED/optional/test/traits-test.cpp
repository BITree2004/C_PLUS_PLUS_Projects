#include "optional.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace {

struct dummy_t {};

struct no_default_t {
  no_default_t() = delete;
};

struct throwing_default_t {
  throwing_default_t() {
    throw std::exception();
  }
};

struct throwing_move_operator_t {
  throwing_move_operator_t() = default;

  throwing_move_operator_t(throwing_move_operator_t&&) {
    throw std::exception();
  }

  throwing_move_operator_t& operator=(throwing_move_operator_t&&) = default;

  [[maybe_unused]] friend void swap(throwing_move_operator_t&, throwing_move_operator_t&) {
    throw std::exception();
  }
};

struct no_copy_t {
  no_copy_t(const no_copy_t&) = delete;
};

struct no_move_t {
  no_move_t(no_move_t&&) = delete;
};

struct non_trivial_copy_t {
  explicit non_trivial_copy_t(int x) noexcept : x{x} {}

  non_trivial_copy_t(const non_trivial_copy_t& other) noexcept : x{other.x + 1} {}

  int x;
};

struct non_trivial_copy_assignment_t {
  explicit non_trivial_copy_assignment_t(int x) noexcept : x{x} {}

  non_trivial_copy_assignment_t& operator=(const non_trivial_copy_assignment_t& other) {
    if (this != &other) {
      x = other.x + 5;
    }
    return *this;
  }

  int x;
};

struct no_move_assignment_t {
  no_move_assignment_t& operator=(no_move_assignment_t&&) = delete;
};

struct no_copy_assignment_t {
  no_copy_assignment_t& operator=(const no_copy_assignment_t&) = delete;
};

} // namespace

const nullopt_t* get_nullopt_ptr() noexcept;
const in_place_t* get_in_place_ptr() noexcept;

TEST(traits_test, nullopt_ptr) {
  EXPECT_EQ(&nullopt, get_nullopt_ptr());
}

TEST(traits_test, in_place_ptr) {
  EXPECT_EQ(&in_place, get_in_place_ptr());
}

TEST(traits_test, destructor) {
  using optional1 = optional<int>;
  using optional2 = optional<std::string>;
  EXPECT_TRUE(std::is_trivially_destructible_v<optional1>);
  EXPECT_FALSE(std::is_trivially_destructible_v<optional2>);
}

TEST(traits_test, default_constructor) {
  using optional1 = optional<std::vector<int>>;
  using optional2 = optional<no_default_t>;
  using optional3 = optional<throwing_default_t>;
  EXPECT_TRUE(std::is_default_constructible_v<optional1>);
  EXPECT_TRUE(std::is_default_constructible_v<optional2>);
  EXPECT_TRUE(std::is_default_constructible_v<optional3>);
}

TEST(traits_test, copy_constructor) {
  using optional1 = optional<no_copy_t>;
  using optional2 = optional<std::vector<std::string>>;
  using optional3 = optional<dummy_t>;
  using optional4 = optional<non_trivial_copy_t>;
  EXPECT_FALSE(std::is_copy_constructible_v<optional1>);
  EXPECT_TRUE(std::is_copy_constructible_v<optional2>);
  EXPECT_FALSE(std::is_trivially_copy_constructible_v<optional2>);
  EXPECT_TRUE(std::is_trivially_copy_constructible_v<optional3>);
  EXPECT_FALSE(std::is_trivially_copy_constructible_v<optional4>);
}

TEST(traits_test, move_constructor) {
  using optional1 = optional<no_move_t>;
  using optional2 = optional<std::string>;
  using optional3 = optional<dummy_t>;
  using optional4 = optional<throwing_move_operator_t>;
  EXPECT_FALSE(std::is_move_constructible_v<optional1>);
  EXPECT_TRUE(std::is_move_constructible_v<optional2>);
  EXPECT_TRUE(std::is_move_constructible_v<optional3>);
  EXPECT_FALSE(std::is_trivially_move_constructible_v<optional2>);
  EXPECT_TRUE(std::is_trivially_move_constructible_v<optional3>);
  EXPECT_TRUE(std::is_move_constructible_v<optional4>);
}

TEST(traits_test, copy_assignment) {
  using optional1 = optional<no_copy_t>;
  using optional2 = optional<no_copy_assignment_t>;
  using optional3 = optional<non_trivial_copy_assignment_t>;
  using optional4 = optional<non_trivial_copy_t>;
  using optional5 = optional<dummy_t>;
  using optional6 = optional<no_copy_t>;
  EXPECT_FALSE(std::is_copy_assignable_v<optional1>);
  EXPECT_FALSE(std::is_copy_assignable_v<optional2>);
  EXPECT_TRUE(std::is_copy_assignable_v<optional3>);
  EXPECT_TRUE(std::is_copy_assignable_v<optional4>);
  EXPECT_TRUE(std::is_copy_assignable_v<optional5>);
  EXPECT_FALSE(std::is_trivially_copy_assignable_v<optional3>);
  EXPECT_FALSE(std::is_trivially_copy_assignable_v<optional4>);
  EXPECT_TRUE(std::is_trivially_copy_assignable_v<optional5>);
  EXPECT_FALSE(std::is_copy_assignable_v<optional6>);
}

TEST(traits_test, move_assignment) {
  using optional1 = optional<no_move_t>;
  using optional2 = optional<no_move_assignment_t>;
  using optional3 = optional<std::vector<double>>;
  using optional4 = optional<std::string>;
  using optional5 = optional<dummy_t>;
  using optional6 = optional<throwing_move_operator_t>;
  using optional7 = optional<no_move_t>;
  EXPECT_FALSE(std::is_move_assignable_v<optional1>);
  EXPECT_FALSE(std::is_move_assignable_v<optional2>);
  EXPECT_TRUE(std::is_move_assignable_v<optional3>);
  EXPECT_TRUE(std::is_move_assignable_v<optional4>);
  EXPECT_TRUE(std::is_move_assignable_v<optional5>);
  EXPECT_FALSE(std::is_trivially_move_assignable_v<optional3>);
  EXPECT_FALSE(std::is_trivially_move_assignable_v<optional4>);
  EXPECT_TRUE(std::is_trivially_move_assignable_v<optional5>);
  EXPECT_TRUE(std::is_move_assignable_v<optional6>);
  EXPECT_FALSE(std::is_move_assignable_v<optional7>);
}

namespace {

enum class variant {
  TRIVIAL,
  DELETED,
  USER_DEFINED,
};

template <variant...>
struct variants {};

template <variant V>
consteval std::string_view to_str() {
  if constexpr (V == variant::TRIVIAL) {
    return "trivial";
  } else if constexpr (V == variant::DELETED) {
    return "deleted";
  } else if constexpr (V == variant::USER_DEFINED) {
    return "user-defined";
  }
}

template <variant Dtor, variant CopyCtor, variant MoveCtor, variant CopyAssign, variant MoveAssign>
struct test_variants_error {
  friend std::ostream& operator<<(std::ostream& out, test_variants_error) {
    out << "Dtor = " << to_str<Dtor>() << ", ";
    out << "CopyCtor = " << to_str<CopyCtor>() << ", ";
    out << "MoveCtor = " << to_str<MoveCtor>() << ", ";
    out << "CopyAssign = " << to_str<CopyAssign>() << ", ";
    out << "MoveAssign = " << to_str<MoveAssign>();
    return out;
  }
};

// workaround for clang 16+
template <variant Dtor>
struct test_object_destructor_base;

template <>
struct test_object_destructor_base<variant::TRIVIAL> {};

template <>
struct test_object_destructor_base<variant::USER_DEFINED> {
  ~test_object_destructor_base() {}
};

template <variant Dtor, variant CopyCtor, variant MoveCtor, variant CopyAssign, variant MoveAssign>
void test_variants() {
  using enum variant;

  struct test_object : test_object_destructor_base<Dtor> {
    test_object(const test_object&)
      requires(CopyCtor == TRIVIAL)
    = default;

    test_object(const test_object&)
      requires(CopyCtor == DELETED)
    = delete;

    test_object(const test_object&)
      requires(CopyCtor == USER_DEFINED)
    {}

    test_object(test_object&&)
      requires(MoveCtor == TRIVIAL)
    = default;

    test_object(test_object&&)
      requires(MoveCtor == DELETED)
    = delete;

    test_object(test_object&&)
      requires(MoveCtor == USER_DEFINED)
    {}

    test_object& operator=(const test_object&)
      requires(CopyAssign == TRIVIAL)
    = default;

    test_object& operator=(const test_object&)
      requires(CopyAssign == DELETED)
    = delete;

    test_object& operator=(const test_object&)
      requires(CopyAssign == USER_DEFINED)
    {
      return *this;
    }

    test_object& operator=(test_object&&)
      requires(MoveAssign == TRIVIAL)
    = default;

    test_object& operator=(test_object&&)
      requires(MoveAssign == DELETED)
    = delete;

    test_object& operator=(test_object&&)
      requires(MoveAssign == USER_DEFINED)
    {
      return *this;
    }
  };

  test_variants_error<Dtor, CopyCtor, MoveCtor, CopyAssign, MoveAssign> err;

  {
    using opt = optional<test_object>;

    if constexpr (CopyCtor != DELETED) {
      EXPECT_TRUE(std::is_copy_constructible_v<opt>) << err;
    }
    if constexpr (MoveCtor != DELETED) {
      EXPECT_TRUE(std::is_move_constructible_v<opt>) << err;
    }
    if constexpr (CopyAssign != DELETED && CopyCtor != DELETED) {
      EXPECT_TRUE(std::is_copy_assignable_v<opt>) << err;
    }
    if constexpr (MoveAssign != DELETED && MoveCtor != DELETED) {
      EXPECT_TRUE(std::is_move_assignable_v<opt>) << err;
    }
  }

  {
    // inherit so that deleted moves still allow for calling copy ctor/assignment
    struct derived : test_object {
      using test_object::test_object;
    };

    using opt = optional<derived>;

    EXPECT_EQ(std::is_destructible_v<derived>, std::is_destructible_v<opt>) << err;
    EXPECT_EQ(std::is_trivially_destructible_v<derived>, std::is_trivially_destructible_v<opt>) << err;

    EXPECT_EQ(std::is_copy_constructible_v<derived>, std::is_copy_constructible_v<opt>) << err;
    EXPECT_EQ(std::is_trivially_copy_constructible_v<derived>, std::is_trivially_copy_constructible_v<opt>) << err;

    EXPECT_EQ(std::is_move_constructible_v<derived>, std::is_move_constructible_v<opt>) << err;
    EXPECT_EQ(std::is_trivially_move_constructible_v<derived>, std::is_trivially_move_constructible_v<opt>) << err;

    if constexpr (CopyCtor == DELETED) {
      EXPECT_FALSE(std::is_copy_assignable_v<opt>) << err;
    } else {
      EXPECT_EQ(std::is_copy_assignable_v<derived>, std::is_copy_assignable_v<opt>) << err;
    }

    if constexpr (CopyCtor == TRIVIAL && Dtor == TRIVIAL) {
      EXPECT_EQ(std::is_trivially_copy_assignable_v<derived>, std::is_trivially_copy_assignable_v<opt>) << err;
    } else {
      EXPECT_FALSE(std::is_trivially_copy_assignable_v<opt>) << err;
    }

    if constexpr (MoveCtor == DELETED && CopyCtor == DELETED) {
      EXPECT_FALSE(std::is_move_assignable_v<opt>) << err;
    } else {
      EXPECT_EQ(std::is_move_assignable_v<derived>, std::is_move_assignable_v<opt>) << err;
    }

    if constexpr ((MoveCtor == TRIVIAL || (MoveCtor == DELETED && CopyCtor == TRIVIAL)) && Dtor == TRIVIAL) {
      EXPECT_EQ(std::is_trivially_move_assignable_v<derived>, std::is_trivially_move_assignable_v<opt>) << err;
    } else {
      EXPECT_FALSE(std::is_trivially_move_assignable_v<opt>) << err;
    }
  }
}

template <variant... Vs, typename F>
void static_for_each(variants<Vs...>, F f) {
  (f.template operator()<Vs>(), ...);
}

} // namespace

TEST(traits_test, stress) {
  using dtor_variants = variants<variant::TRIVIAL, variant::USER_DEFINED>;
  using all_variants = variants<variant::TRIVIAL, variant::DELETED, variant::USER_DEFINED>;

  static_for_each(dtor_variants{}, []<variant Dtor>() {
    static_for_each(all_variants{}, []<variant CopyCtor>() {
      static_for_each(all_variants{}, []<variant MoveCtor>() {
        static_for_each(all_variants{}, []<variant CopyAssign>() {
          static_for_each(all_variants{}, []<variant MoveAssign>() {
            test_variants<Dtor, CopyCtor, MoveCtor, CopyAssign, MoveAssign>();
          });
        });
      });
    });
  });
}
