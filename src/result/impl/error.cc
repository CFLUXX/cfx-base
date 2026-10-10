module;

#include <fmt/base.h>
#include <fmt/format.h>
#include <iterator>
#include <source_location>
#include <utility>

module cfx.base.result.error;

namespace cfx {
Error::Error(
        ErrorCategory category, ErrorCode code, String message,
        std::source_location location
)
    : code_(code),
      category_(category),
      message_(std::move(message)),
      location_(location) {}

Error::Error(ErrorCode code, String message, std::source_location location)
    : code_(code),
      category_(CategoryOf(code)),
      message_(std::move(message)),
      location_(location) {}

Error::Error(
        ErrorCategory category, ErrorCode code, String message,
        SharedPtr<const Error> cause, std::source_location location
)
    : code_(code),
      category_(category),
      message_(std::move(message)),
      location_(location),
      cause_(std::move(cause)) {}


[[nodiscard]]
auto Error::ToString() const -> String {
  String out_str;
  out_str.reserve(
          (128 * (ChainDepth() > 8 ? 9 : (ChainDepth() + 1))) +
          ((ChainDepth() > 8) ? 72 : 0)
  );

  auto it = std::back_inserter(out_str);

  fmt::format_to(
          it, "Error[{}:{}] {}", ToString(category_), ToString(code_), message_
  );
  if (location_.file_name() != nullptr && location_.line() != 0) {
    fmt::format_to(
            it, "\n  at {}:{} in {}", location_.file_name(), location_.line(),
            location_.function_name()
    );
  }

  if (this->HasCause()) {
    constexpr usize kMaxChainDepth = 8;
    usize depth                    = 0;
    const Error *cur               = cause_.get();

    while (cur != nullptr) {
      if (depth >= kMaxChainDepth) {
        fmt::format_to(
                it,
                "\n  ...(Chain is too long(ChainDepth[{}] > 8), has "
                "been truncated)",
                ChainDepth()
        );
        break;
      }

      fmt::format_to(
              it, "\n  caused by: Error[{}:{}] {}", ToString(cur->category_),
              ToString(cur->code_), cur->message_
      );
      if (cur->location_.file_name() != nullptr && cur->location_.line() != 0) {
        fmt::format_to(
                it, "\n    at {}:{} in {}", cur->location_.file_name(),
                cur->location_.line(), cur->location_.function_name()
        );
      }
      cur = cur->cause_.get();
      ++depth;
    }
  }
  return out_str;
}

}  // namespace cfx
