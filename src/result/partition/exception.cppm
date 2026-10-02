module;

#include <exception>
#include <type_traits>
#include <utility>

export module cfx.base.result:exception;

namespace cfx {

export template <typename E>
class BadResultAccess : public std::exception {
  static_assert(
          !std::is_reference_v<E>, "BadResultAccess: E cannot be reference"
  );
  static_assert(!std::is_void_v<E>, "BadResultAccess: E cannot be void");

  E error_;

public:
  explicit BadResultAccess(const E &e) : error_(e) {}
  explicit BadResultAccess(E &&e) noexcept(std::is_move_constructible_v<E>)
      : error_(std::move(e)) {}

  [[nodiscard]] auto error() const & noexcept -> const E & { return error_; }

  [[nodiscard]] auto error() & noexcept -> E & { return error_; }

  [[nodiscard]] auto error() && noexcept -> E && { return std::move(error_); }

  [[nodiscard]] const char *what() const noexcept override {
    return "bad Result access";
  }
};

class BadResultAccessVoid : public std::exception {
public:
  [[nodiscard]] const char *what() const noexcept override {
    return "bad Result access (void)";
  }
};
}  // namespace cfx
