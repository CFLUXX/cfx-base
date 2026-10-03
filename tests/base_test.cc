#include <gtest/gtest.h>
#include <iostream>
import cfx.base.result;

TEST(ResultTest, HoldsValue) {
  cfx::Result<int> test_res = cfx::Ok(23);
  EXPECT_TRUE(test_res.HasValue());
  EXPECT_EQ(test_res.Value(), 23);
}

auto test_error_to_string() {
  return cfx::Err(
          {cfx::ErrorCategory::Auth, cfx::ErrorCode::AuthExpired,
           "Failed to login?"}
  );
}

TEST(ResultTest, HoldsError) {
  cfx::Error err{
          cfx::ErrorCategory::Config, cfx::ErrorCode::ParseError, "I don't know"
  };
  auto res = cfx::Err(err.rWithCause(
          {cfx::ErrorCategory::Parse, cfx::ErrorCode::ConfigParseError,
           "it may cause by Sun Xiaochuan"}
  ));
  EXPECT_FALSE(res.HasValue());
  EXPECT_TRUE(res.HasError());
  auto res_err = test_error_to_string();
  std::cout << res.Error().ToString() << std::endl
            << res_err.Error().ToString() << std::endl
            << res.Error().ToString().size();
}
