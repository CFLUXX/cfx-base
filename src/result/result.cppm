/*
 * Copyright (c) 2026 oldmnj <oldmnj@163.com>
 * Copyright (c) 2026 CFLUXX
 * SPDX-License-Identifier: MIT
 */
/**
 * @file result.cppm
 * @brief provide the type to deal with errors
 * @author oldmnj
 * @date 2026-10-02
 */
module;

#include <concepts>
#include <expected>
#include <memory>
#include <type_traits>
#include <utility>

export module cfx.base.result;
export import cfx.base.result.error;
import :exception;

namespace cfx {

export struct InPlaceValueTag {};
export struct InPlaceErrorTag {};

export template <typename T, typename E>
class Result;

namespace detail {
template <typename T>
using RemoveCvRef = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename>
struct IsResultImpl : std::false_type {};

template <typename T, typename E>
struct IsResultImpl<Result<T, E>> : std::true_type {};

template <typename T>
concept IsResult = IsResultImpl<RemoveCvRef<T>>::value;

template <typename R>
struct ResultTraits;

template <typename T, typename E>
struct ResultTraits<Result<T, E>> {
  using ValueType = T;
  using ErrorType = E;
};

template <typename R>
using ResultValueOf = typename ResultTraits<RemoveCvRef<R>>::ValueType;

template <typename R>
using ResultErrorOf = typename ResultTraits<RemoveCvRef<R>>::ErrorType;

template <typename T>
concept NothrowMove = std::is_nothrow_move_constructible_v<T>;
}  // namespace detail

/**
 * @brief A wrapper that contains either an expected value of type T or an
 * unexpected value of type E.
 *
 * @tparam T the type of expected value (maybe cv void).
 * @tparam E the type of unexpected value.
 *
 * @note T and E cannot be reference types.
 */
export template <typename T, typename E = Error>
class Result {
private:
  static_assert(!std::is_reference_v<T>, "Result: T cannot be reference");
  static_assert(!std::is_reference_v<E>, "Result: E cannot be reference");
  static_assert(
          !std::is_same_v<std::remove_cv_t<T>, InPlaceValueTag>,
          "Result: T cannot be InPlaceValueTag"
  );
  static_assert(
          !std::is_same_v<std::remove_cv_t<T>, InPlaceErrorTag>,
          "Result: T cannot be InPlaceErrorTag"
  );
  static_assert(
          !std::is_void_v<T>, "Result: use Result<void, E> for void value"
  );
  static_assert(!std::is_void_v<E>, "Result: E cannot be void");
  static_assert(!std::is_const_v<T>, "Result: T cannot be const");
  static_assert(
          !std::is_const_v<E>, "Result: E cannot be const"
  );  /// else we cannot assign values to E or T
  static_assert(
          std::is_nothrow_destructible_v<T>, "Result: destroy T must be nothrow"
  );
  static_assert(
          std::is_nothrow_destructible_v<E>, "Result: destroy E must be nothrow"
  );
  /*
   * 以下断言替代为构造函数，赋值函数等等的requires
  static_assert(
          detail::NothrowMove<T>, "Result: T must be nothrow-move/destructible"
  );
  static_assert(
          detail::NothrowMove<E>, "Result: E must be nothrow-move/destructible"
  );
  */

  union ResultStorage {
    [[no_unique_address]] T value;
    [[no_unique_address]] E error;
    constexpr ResultStorage() noexcept {}
    constexpr ~ResultStorage() noexcept {}

    /// The lifecycle of ResultStorage is controlled by Result.
    ResultStorage(const ResultStorage &)                   = delete;
    auto operator=(const ResultStorage &) -> ResultStorage = delete;
    ResultStorage(ResultStorage &&)                        = delete;
    auto operator=(ResultStorage &&) -> ResultStorage      = delete;
  };

  ResultStorage storage_;
  bool has_value_;

  constexpr void destroy() noexcept {
    if (has_value_) {
      std::destroy_at(std::addressof(storage_.value));
    } else {
      std::destroy_at(std::addressof(storage_.error));
    }
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr void construct_value(
          Args &&...args
  ) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
    std::construct_at(
            std::addressof(storage_.value), std::forward<Args>(args)...
    );
    has_value_ = true;
  }

  template <typename... Args>
    requires std::constructible_from<E, Args...>
  constexpr void construct_error(
          Args &&...args
  ) noexcept(std::is_nothrow_constructible_v<E, Args...>) {
    std::construct_at(
            std::addressof(storage_.error), std::forward<Args>(args)...
    );
    has_value_ = false;
  }

public:
  using ValueType = T;
  using ErrorType = E;

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr explicit Result(
          InPlaceValueTag, Args &&...args
  ) noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : has_value_(true) {
    std::construct_at(
            std::addressof(storage_.value), std::forward<Args>(args)...
    );
  }

  template <typename... Args>
    requires std::constructible_from<E, Args...>
  constexpr explicit Result(
          InPlaceErrorTag, Args &&...args
  ) noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : has_value_(false) {
    std::construct_at(
            std::addressof(storage_.error), std::forward<Args>(args)...
    );
  }

  template <class U = T>
    requires(!std::is_same_v<T, E>) &&
            (!std::is_same_v<std::remove_cvref_t<U>, Result>) &&
            (!std::is_same_v<std::remove_cvref_t<U>, std::expected<T, E>>) &&
            (!std::is_same_v<std::remove_cvref_t<U>, std::unexpected<E>>) &&
            std::is_constructible_v<T, U &&> &&
            (!std::is_constructible_v<E, U &&>)
  constexpr Result(U &&value) : has_value_(true) {
    std::construct_at(std::addressof(storage_.value), std::forward<U>(value));
  }

  template <class G = E>
    requires(!std::is_same_v<T, E>) &&
            (!std::is_same_v<std::remove_cvref_t<G>, Result>) &&
            (!std::is_same_v<std::remove_cvref_t<G>, std::expected<T, E>>) &&
            (!std::is_same_v<std::remove_cvref_t<G>, std::unexpected<E>>) &&
            std::is_constructible_v<E, G &&> &&
            (!std::is_constructible_v<T, G &&>)
  constexpr Result(G &&error) : has_value_(false) {
    std::construct_at(std::addressof(storage_.error), std::forward<G>(error));
  }

  constexpr Result(const Result &other) noexcept(
          std::is_nothrow_copy_constructible_v<T> &&
          std::is_nothrow_copy_constructible_v<E>
  )
      : has_value_(other.has_value_) {
    if (has_value_) {
      std::construct_at(std::addressof(storage_.value), other.storage_.value);
    } else {
      std::construct_at(std::addressof(storage_.error), other.storage_.error);
    }
  }

  // static_assert保证
  constexpr Result(Result &&other) noexcept
    requires detail::NothrowMove<T> && detail::NothrowMove<E> &&
             std::is_move_constructible_v<T> && std::is_move_constructible_v<E>
      : has_value_(other.has_value_) {
    if (has_value_) {
      std::construct_at(
              std::addressof(storage_.value), std::move(other.storage_.value)
      );
    } else {
      std::construct_at(
              std::addressof(storage_.error), std::move(other.storage_.error)
      );
    }
  }

  template <typename U, typename G>
    requires std::is_constructible_v<T, const U &> &&
             std::is_constructible_v<E, const U &>
  constexpr explicit Result(const std::expected<U, G> &exp) noexcept(
          std::is_nothrow_copy_constructible_v<U> &&
          std::is_nothrow_copy_constructible_v<G>
  )
      : has_value_(exp.has_value()) {
    if (exp.has_value()) {
      std::construct_at(std::addressof(storage_.value), *exp);
    } else {
      std::construct_at(std::addressof(storage_.error), exp.error());
    }
  }

  // noexcept: see static_assert
  template <typename U, typename G>
    requires std::is_constructible_v<T, U &&> &&
             std::is_constructible_v<E, G &&> &&
             std::is_nothrow_move_constructible_v<U> &&
             std::is_nothrow_move_constructible_v<G>
  constexpr Result(std::expected<U, G> &&exp) noexcept
      : has_value_(exp.has_value()) {
    if (exp.has_value()) {
      std::construct_at(std::addressof(storage_.value), std::move(*exp));
    } else {
      std::construct_at(std::addressof(storage_.error), std::move(exp).error());
    }
  }

  constexpr ~Result() noexcept { destroy(); };

  constexpr auto operator=(const Result &other) noexcept(
          std::is_nothrow_copy_constructible_v<T> &&
          std::is_nothrow_copy_constructible_v<E> &&
          std::is_nothrow_copy_assignable_v<T> &&
          std::is_nothrow_copy_assignable_v<E>
  ) -> Result & {
    if (this == std::addressof(other)) {
      return *this;
    }
    if (has_value_ == other.has_value_) {
      if (has_value_) {
        storage_.value = other.storage_.value;
      } else {
        storage_.error = other.storage_.error;
      }
    } else {
      Result tmp{other};
      destroy();
      if (tmp.has_value_) {
        construct_value(std::move(tmp.storage_.value));
      } else {
        construct_error(std::move(tmp.storage_.error));
      }
    }
    return *this;
  }

  constexpr auto operator=(Result &&other) noexcept -> Result &
    requires detail::NothrowMove<T> && detail::NothrowMove<E> &&
             std::is_move_constructible_v<T> && std::is_move_constructible_v<E>
  {
    if (this == std::addressof(other)) {
      return *this;
    }

    if (has_value_ == other.has_value_) {
      if (has_value_) {
        storage_.value = std::move(other.storage_.value);
      } else {
        storage_.error = std::move(other.storage_.error);
      }
    } else {
      destroy();
      if (other.has_value_) {
        construct_value(std::move(other.storage_.value));
      } else {
        construct_error(std::move(other.storage_.error));
      }
    }
    return *this;
  }


  constexpr void Swap(Result &other) noexcept {
    if (this == std::addressof(other)) {
      return;
    }
    Result tmp{std::move(other)};
    other = std::move(*this);
    *this = std::move(tmp);
  }

  friend constexpr void swap(Result &a, Result &b) noexcept { a.Swap(b); }


  [[nodiscard]]
  constexpr auto HasValue() const noexcept -> bool {
    return has_value_;
  }
  [[nodiscard]]
  constexpr auto HasError() const noexcept -> bool {
    return !has_value_;
  }
  [[nodiscard]]
  constexpr explicit operator bool() const noexcept {
    return has_value_;
  }

  [[nodiscard]]
  constexpr auto Value(this auto &&self) -> decltype(auto) {
    if (!self.has_value_) {
      throw BadResultAccess<E>{
              std::forward<decltype(self)>(self).storage_.error
      };
    }
    return (std::forward<decltype(self)>(self).storage_.value);
  }

  [[nodiscard]]
  constexpr auto Error(this auto &&self) -> decltype(auto) {
    if (self.has_value_) {
      std::unreachable();
    }
    return (std::forward<decltype(self)>(self).storage_.error);
  }


  [[nodiscard]]
  constexpr auto ValueIf(this auto &&self) noexcept {
    using Ptr = std::conditional_t<
            std::is_const_v<std::remove_reference_t<decltype(self)>>, const T *,
            T *>;
    /// ValueIf and ErrorIf: 如果为右值版本,Ptr = T*,临时r销毁后,指针会悬垂
    return self.has_value_
                   ? static_cast<Ptr>(std::addressof(self.storage_.value))
                   : static_cast<Ptr>(nullptr);
  }

  [[nodiscard]]
  constexpr auto ErrorIf(this auto &&self) noexcept {
    using Ptr = std::conditional_t<
            std::is_const_v<std::remove_reference_t<decltype(self)>>, const E *,
            E *>;
    return !self.has_value_
                   ? static_cast<Ptr>(std::addressof(self.storage_.error))
                   : static_cast<Ptr>(nullptr);
  }

  template <typename U>
    requires std::constructible_from<T, U &&>
  [[nodiscard]]
  constexpr auto ValueOr(this auto &&self, U &&default_value) -> T {
    if (self.has_value_) {
      return static_cast<T>(std::forward<decltype(self)>(self).storage_.value);
    }
    return static_cast<T>(std::forward<U>(default_value));
  }

  /**
   * @brief get reference of  Value
   *
   * @return reference of value(Deduce reference type from object state)
   *
   * @note the version of && will return a dangling pointer.
   */
  [[nodiscard]]
  constexpr auto operator*(this auto &&self) noexcept -> decltype(auto) {
    return std::forward<decltype(self)>(self).storage_.value;
  }

  [[nodiscard]]
  constexpr auto operator->(this auto &&self) noexcept {
    return std::addressof(std::forward<decltype(self)>(self).storage_.value);
  }


  template <typename F>
  [[nodiscard]]
  constexpr auto Map(this auto &&self, F &&f)
    requires std::invocable<
            F, decltype(std::forward<decltype(self)>(self).Value())>
  {
    using Self = decltype(self);
    using U =
            std::invoke_result_t<F, decltype(std::forward<Self>(self).Value())>;

    if (self.has_value_) {
      if constexpr (std::is_void_v<U>) {
        std::forward<F>(f)(std::forward<Self>(self).Value());
        return Result<void, E>{InPlaceValueTag{}};
      } else {
        return Result<U, E>{
                InPlaceValueTag{},
                std::forward<F>(f)(std::forward<Self>(self).Value())
        };
      }
    } else {
      return Result<U, E>{InPlaceErrorTag{}, std::forward<Self>(self).Error()};
    }
  }

  template <typename F>
  [[nodiscard]]
  constexpr auto MapErr(this auto &&self, F &&f)
    requires(!std::is_void_v<std::invoke_result_t<
                     F, decltype(std::forward<decltype(self)>(self).Error())>>) &&
            std::invocable<
                    F, decltype(std::forward<decltype(self)>(self).Error())>
  {
    using Self = decltype(self);
    using F_ =
            std::invoke_result_t<F, decltype(std::forward<Self>(self).Error())>;

    if (self.has_value_) {
      return Result<T, F_>{InPlaceValueTag{}, std::forward<Self>(self).Value()};
    }
    return Result<T, F_>{
            InPlaceErrorTag{},
            std::forward<F>(f)(std::forward<Self>(self).Error())
    };
  }

  template <typename F>
  [[nodiscard]] constexpr auto AndThen(this auto &&self, F &&f)
    requires detail::IsResult<std::invoke_result_t<
                     F, decltype(std::forward<decltype(self)>(self).Value())>> &&
             std::is_constructible_v<
                     typename detail::ResultTraits<
                             detail::RemoveCvRef<std::invoke_result_t<
                                     F, decltype(std::forward<decltype(self)>(self)
                                                         .Value())>>>::ErrorType,
                     E> &&
             std::invocable<
                     F, decltype(std::forward<decltype(self)>(self).Value())>

  {
    using Self = decltype(self);
    using U =
            std::invoke_result_t<F, decltype(std::forward<Self>(self).Value())>;
    using Traits = detail::ResultTraits<detail::RemoveCvRef<U>>;
    using Res    = detail::RemoveCvRef<U>;

    if (self.has_value_) {
      return std::forward<F>(f)(std::forward<Self>(self).Value());
    }

    return Res{InPlaceErrorTag{}, std::forward<Self>(self).Error()};
  }

  template <typename F>
  [[nodiscard]] constexpr auto OrElse(this auto &&self, F &&f)
    requires detail::IsResult<std::invoke_result_t<
                     F, decltype(std::forward<decltype(self)>(self).Error())>> &&
             std::invocable<
                     F, decltype(std::forward<decltype(self)>(self).Error())>

  {
    using Self = decltype(self);
    using U =
            std::invoke_result_t<F, decltype(std::forward<Self>(self).Error())>;
    using Traits = detail::ResultTraits<detail::RemoveCvRef<U>>;
    using UVal   = typename Traits::ValueType;
    using Res    = detail::RemoveCvRef<U>;

    if (!self.has_value_) {
      return std::forward<F>(f)(std::forward<Self>(self).Error());
    }
    if constexpr (std::is_void_v<UVal>) {
      return Res{InPlaceValueTag{}};
    }

    return Res{InPlaceValueTag{}, std::forward<Self>(self).Value()};
  }

  template <typename F>
    requires std::invocable<F, T &>
  constexpr auto IfValue(this Result &self, F &&f) -> Result & {
    if (self.has_value_) {
      std::forward<F>(f)(self.storage_.value);
    }
    return self;
  }

  template <typename F>
    requires std::invocable<F, const T &>
  constexpr auto IfValue(this const Result &self, F &&f) -> const Result & {
    if (self.has_value_) {
      std::forward<F>(f)(self.storage_.value);
    }
    return self;
  }

  template <typename F>
    requires std::invocable<F, E &>
  constexpr auto IfError(this Result &self, F &&f) -> Result & {
    if (!self.has_value_) {
      std::forward<F>(f)(self.storage_.error);
    }
    return self;
  }

  template <typename F>
    requires std::invocable<F, const E &>
  constexpr auto IfError(this const Result &self, F &&f) -> const Result & {
    if (!self.has_value_) {
      std::forward<F>(f)(self.storage_.error);
    }
    return self;
  }

  friend constexpr auto operator==(const Result &a, const Result &b) -> bool
    requires std::equality_comparable<T> && std::equality_comparable<E>
  {
    if (a.has_value_ != b.has_value_) {
      return false;
    }
    return a.has_value_ ? (a.storage_.value == b.storage_.value)
                        : (a.storage_.error == b.storage_.error);
  }
};

export template <typename E>
class Result<void, E> {
  static_assert(!std::is_void_v<E>, "Result: E cannot be void");
  static_assert(
          std::is_nothrow_destructible_v<E>,
          "Result: E must be nothrow-destructible"
  );
  static_assert(!std::is_const_v<E>, "Result: E cannot be const");


  union ResultStorage {
    [[no_unique_address]] E error;
    constexpr ResultStorage() noexcept {}
    constexpr ~ResultStorage() noexcept {}

    ResultStorage(const ResultStorage &)                   = delete;
    auto operator=(const ResultStorage &) -> ResultStorage = delete;
    ResultStorage(ResultStorage &&)                        = delete;
    auto operator=(ResultStorage &&) -> ResultStorage      = delete;
  };

  ResultStorage storage_;
  bool has_value_;

  constexpr void destroy() noexcept {
    if (!has_value_) {
      std::destroy_at(std::addressof(storage_.error));
    }
  }

public:
  using ValueType = void;
  using ErrorType = E;

  constexpr Result() noexcept : has_value_(true) {}

  constexpr explicit Result(InPlaceValueTag) noexcept : has_value_(true) {}

  template <typename... Args>
    requires std::constructible_from<E, Args...>
  constexpr explicit Result(
          InPlaceErrorTag, Args &&...args
  ) noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : has_value_(false) {
    std::construct_at(
            std::addressof(storage_.error), std::forward<Args>(args)...
    );
  }

  template <class G = E>
    requires(!std::is_same_v<void, E>) &&
            (!std::is_same_v<std::remove_cvref_t<G>, Result>) &&
            (!std::is_same_v<std::remove_cvref_t<G>, std::expected<void, E>>) &&
            (!std::is_same_v<std::remove_cvref_t<G>, std::unexpected<E>>) &&
            std::is_constructible_v<E, G &&> &&
            (!std::is_constructible_v<void, G &&>)
  constexpr Result(G &&error) : has_value_(false) {
    std::construct_at(std::addressof(storage_.error), std::forward<G>(error));
  }

  constexpr Result(
          const Result &other
  ) noexcept(std::is_nothrow_copy_constructible_v<E>)
      : has_value_(other.has_value_) {
    if (!has_value_) {
      std::construct_at(std::addressof(storage_.error), other.storage_.error);
    }
  }

  constexpr Result(Result &&other) noexcept
    requires detail::NothrowMove<E> && std::is_move_constructible_v<E>
      : has_value_(other.has_value_) {
    if (!has_value_) {
      std::construct_at(
              std::addressof(storage_.error), std::move(other.storage_.error)
      );
    }
  }

  template <typename G>
    requires std::is_constructible_v<G, const E &>
  constexpr Result(
          const std::expected<void, G> &exp
  ) noexcept(std::is_nothrow_copy_constructible_v<G>)
      : has_value_(exp.has_value()) {
    if (!exp.has_value()) {
      std::construct_at(std::addressof(storage_.error), exp.error());
    }
  }

  template <typename G>
    requires std::is_constructible_v<E, G &&> &&
             std::is_nothrow_move_constructible_v<G>
  constexpr Result(std::expected<void, G> &&exp) noexcept
      : has_value_(exp.has_value()) {
    if (!exp.has_value()) {
      std::construct_at(std::addressof(storage_.error), std::move(exp).error());
    }
  }

  constexpr ~Result() noexcept { destroy(); }

  constexpr auto operator=(const Result &other) noexcept(
          std::is_nothrow_copy_constructible_v<E> &&
          std::is_nothrow_copy_assignable_v<E>
  ) -> Result & {
    if (this == std::addressof(other)) {
      return *this;
    }
    if (has_value_ == other.has_value_) {
      if (!has_value_) {
        storage_.error = other.storage_.error;
      }
    } else {
      Result tmp(other);
      destroy();
      has_value_ = tmp.has_value_;
      if (!tmp.has_value_) {
        construct_error(std::move(tmp.storage_.error));
      }
    }
    return *this;
  }

  constexpr auto operator=(Result &&other) noexcept -> Result &
    requires detail::NothrowMove<E> && std::is_move_constructible_v<E>
  {
    if (this == std::addressof(other)) {
      return *this;
    }
    if (has_value_ == other.has_value_) {
      if (!has_value_) {
        storage_.error = std::move(other.storage_.error);
      }
    } else {
      destroy();
      has_value_ = other.has_value_;
      if (!has_value_) {
        construct_error(std::move(other.storage_.error));
      }
    }
    return *this;
  }

  constexpr void Swap(Result &other) noexcept {
    if (this == std::addressof(other)) {
      return;
    }
    Result tmp(std::move(other));
    other = std::move(*this);
    *this = std::move(tmp);
  }
  friend constexpr void swap(Result &a, Result &b) noexcept { a.Swap(b); }

  [[nodiscard]] constexpr auto HasValue() const noexcept -> bool {
    return has_value_;
  }
  [[nodiscard]] constexpr auto HasError() const noexcept -> bool {
    return !has_value_;
  }
  [[nodiscard]] constexpr explicit operator bool() const noexcept {
    return has_value_;
  }

  [[nodiscard]] constexpr auto Value(this auto &&self) -> void {
    if (!self.has_value_) {
      throw BadResultAccess{std::forward<decltype(self)>(self).storage_.error};
    }
  }

  [[nodiscard]] constexpr auto Error(this auto &&self) noexcept
          -> decltype(auto) {
    if (self.has_value_) {
      std::unreachable();
    }
    return (std::forward<decltype(self)>(self).storage_.error);
  }

  [[nodiscard]] constexpr auto ErrorIf(this auto &&self) noexcept {
    using Ptr = std::conditional_t<
            std::is_const_v<std::remove_reference_t<decltype(self)>>, const E *,
            E *>;
    return !self.has_value_
                   ? static_cast<Ptr>(std::addressof(self.storage_.error))
                   : static_cast<Ptr>(nullptr);
  }

  template <typename F>
    requires std::invocable<F>
  [[nodiscard]] constexpr auto Map(this auto &&self, F &&f) {
    using Self = decltype(self);
    using U    = std::invoke_result_t<F>;

    if (self.has_value_) {
      if constexpr (std::is_void_v<U>) {
        std::forward<F>(f)();
        return Result<void, E>{InPlaceValueTag{}};
      } else {
        return Result<U, E>{InPlaceValueTag{}, std::forward<F>(f)()};
      }
    } else {
      return Result<U, E>{InPlaceErrorTag{}, std::forward<Self>(self).Error()};
    }
  }

  template <typename F>
  [[nodiscard]] constexpr auto MapErr(this auto &&self, F &&f)
    requires(!std::is_void_v<std::invoke_result_t<
                     F, decltype(std::forward<decltype(self)>(self).Error())>>) &&
            std::invocable<
                    F, decltype(std::forward<decltype(self)>(self).Error())>
  {
    using Self = decltype(self);
    using F_ =
            std::invoke_result_t<F, decltype(std::forward<Self>(self).Error())>;

    if (self.has_value_) {
      return Result<void, F_>{InPlaceValueTag{}};
    }
    return Result<void, F_>{
            InPlaceErrorTag{},
            std::forward<F>(f)(std::forward<Self>(self).Error())
    };
  }

  template <typename F>
    requires detail::IsResult<std::invoke_result_t<F>> &&
             std::invocable<F>
             [[nodiscard]] constexpr auto AndThen(this auto &&self, F &&f)
               requires std::is_constructible_v<
                       typename detail::ResultTraits<detail::RemoveCvRef<
                               std::invoke_result_t<F>>>::ErrorType,
                       E>
  {
    using Self   = decltype(self);
    using U      = std::invoke_result_t<F>;
    using Traits = detail::ResultTraits<detail::RemoveCvRef<U>>;
    using Res    = detail::RemoveCvRef<U>;

    if (self.has_value_) {
      return std::forward<F>(f)();
    }

    return Res{InPlaceErrorTag{}, std::forward<Self>(self).Error()};
  }

  template <typename F>
  [[nodiscard]] constexpr auto OrElse(this auto &&self, F &&f)
    requires std::is_void_v<typename detail::ResultTraits<
                     detail::RemoveCvRef<std::invoke_result_t<
                             F, decltype(std::forward<decltype(self)>(self)
                                                 .Error())>>>::ValueType> &&
             std::invocable<
                     F, decltype(std::forward<decltype(self)>(self).Error())> &&
             detail::IsResult<std::invoke_result_t<
                     F, decltype(std::forward<decltype(self)>(self).Error())>>

  {
    using Self = decltype(self);
    using U =
            std::invoke_result_t<F, decltype(std::forward<Self>(self).Error())>;
    using Traits = detail::ResultTraits<detail::RemoveCvRef<U>>;
    using UVal   = typename Traits::ValueType;
    using Res    = detail::RemoveCvRef<U>;

    if (!self.has_value_) {
      return std::forward<F>(f)(std::forward<Self>(self).Error());
    }

    return Res{InPlaceValueTag{}};
  }

  template <typename F>
    requires std::invocable<F>
  constexpr auto IfValue(this Result &self, F &&f) -> Result & {
    if (self.has_value_) {
      std::forward<F>(f)();
    }
    return self;
  }

  template <typename F>
    requires std::invocable<F>
  constexpr auto IfValue(this const Result &self, F &&f) -> const Result & {
    if (self.has_value_) {
      std::forward<F>(f)();
    }
    return self;
  }

  template <typename F>
    requires std::invocable<F, E &>
  constexpr auto IfError(this Result &self, F &&f) -> Result & {
    if (!self.has_value_) {
      std::forward<F>(f)(self.storage_.error);
    }
    return self;
  }

  template <typename F>
    requires std::invocable<F, const E &>
  constexpr auto IfError(this const Result &self, F &&f) -> const Result & {
    if (!self.has_value_) {
      std::forward<F>(f)(self.storage_.error);
    }
    return self;
  }

  friend constexpr auto operator==(const Result &a, const Result &b) -> bool
    requires std::equality_comparable<E>
  {
    if (a.has_value_ != b.has_value_) {
      return false;
    }
    return a.has_value_ ? (true) : (a.storage_.error == b.storage_.error);
  }
};

export template <typename T = void, typename E = Error>
[[nodiscard]]
constexpr auto Ok() -> Result<T, E>
  requires std::is_void_v<T> || std::is_default_constructible_v<T>
{
  if constexpr (std::is_void_v<T>) {
    return Result<T, E>{};
  } else {
    return Result<T, E>{InPlaceValueTag{}};
  }
}

export template <typename T>
[[nodiscard]]
constexpr auto Ok(T &&value)
        -> Result<std::remove_cv_t<std::remove_reference_t<T>>, Error> {
  using V = std::remove_cv_t<std::remove_reference_t<T>>;
  return Result<V, Error>{InPlaceValueTag{}, std::forward<T>(value)};
}

export template <typename T, typename E = Error, typename U>
  requires std::constructible_from<T, U &&>
[[nodiscard]]
constexpr auto Ok(U &&value) -> Result<T, E> {
  return Result<T, E>{InPlaceValueTag{}, std::forward<U>(value)};
}

export template <typename T = void, typename E = Error>
[[nodiscard]]
constexpr auto Err() -> Result<T, E>
  requires std::is_void_v<T>
{
  return Result<T, E>{InPlaceErrorTag{}};
}

export template <typename E>
[[nodiscard]]
constexpr auto Err(E &&error)
        -> Result<void, std::remove_cv_t<std::remove_reference_t<E>>> {
  using EE = std::remove_cv_t<std::remove_reference_t<E>>;
  return Result<void, EE>{InPlaceErrorTag{}, std::forward<E>(error)};
}

export template <typename T = void, typename E, typename G>
  requires std::constructible_from<E, G &&>
[[nodiscard]]
constexpr auto Err(G &&error) -> Result<T, E> {
  return Result<T, E>{InPlaceErrorTag{}, std::forward<G>(error)};
}

export template <typename T, typename E = Error, typename... Args>
  requires std::constructible_from<T, Args...>
[[nodiscard]]
constexpr auto OkEmplace(Args &&...args) -> Result<T, E> {
  return Result<T, E>{InPlaceValueTag{}, std::forward<Args>(args)...};
}

export template <typename T = void, typename E, typename... Args>
  requires std::constructible_from<E, Args...>
[[nodiscard]]
constexpr auto ErrEmplace(Args &&...args) -> Result<T, E> {
  return Result<T, E>{InPlaceErrorTag{}, std::forward<Args>(args)...};
}


export template <typename T, typename E>
[[nodiscard]]
constexpr auto ResultToExpected(const Result<T, E> &res)
        -> std::expected<T, E> {
  if (res.HasValue()) {
    return std::expected{std::in_place_t{}, *res};
  }
  return std::expected{std::unexpect_t{}, res.Error()};
}

export template <typename T, typename E>
[[nodiscard]]
constexpr auto ResultToExpected(Result<T, E> &&res) -> std::expected<T, E> {
  if (res.HasValue()) {
    return std::expected{std::in_place_t{}, std::move(*res)};
  }
  return std::expected{std::unexpect_t{}, std::move(res).Error()};
}
}  // namespace cfx
