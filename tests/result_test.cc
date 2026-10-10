#include <gtest/gtest.h>
#include <iostream>
#include <print>
#include <source_location>
#include <vector>
import cfx.base.result;

TEST(ResultTest, HoldsValue) {
  cfx::Result<int> test_res = cfx::Ok(23);
  EXPECT_TRUE(test_res.HasValue());
  EXPECT_EQ(test_res.Value(), 23);
}

auto test_error_to_string() {
  return cfx::Err<cfx::Error>(
          {cfx::ErrorCode::ScriptRuntimeError, "Failed to login?"}
  );
}

TEST(ResultTest, HoldsError) {
  cfx::Error err{
          cfx::ErrorCategory::File, cfx::ErrorCode::FileNotFound, "I don't know"
  };
  auto res = cfx::Err(err.rWithCause(
          {cfx::ErrorCode::AbiMismatch, "it may cause by Sun Xiaochuan"}
  ));
  EXPECT_FALSE(res.HasValue());
  EXPECT_TRUE(res.HasError());
  auto res_err = test_error_to_string();
  std::cout << res.Error().ToString() << std::endl
            << res_err.Error().ToString() << std::endl
            << res.Error().ToString().size();
}

TEST(ResultTest, HoldsIfValue) {
  std::vector<int> arr{1, 2, 3, 4, 5, 6, 7, 8, 9};
  cfx::Result<std::vector<int>> res = arr;
  EXPECT_TRUE(res.HasValue());
  res.IfValue([](const std::vector<int> &arr) {
    for (auto &a : arr) {
      std::print("a: {}\n", a);
    }
  });

  cfx::Result<void> res_value =
          cfx::Error{cfx::ErrorCode::AudioDecodeFailed, "I DON'T KNOW"};

  res_value.IfError([](const cfx::Error &err) {
    std::print("{}", err.ToString());
  });
}
