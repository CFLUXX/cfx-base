/*
 * Copyright (c) 2026 oldmnj <oldmnj@163.com>
 * Copyright (c) 2026 CFLUXX
 * SPDX-License-Identifier: MIT
 */
/**
 * @file error.cppm
 * @brief provide the type for svaing message of errors
 * @anthor oldmnj
 * @date 2026-10-01
 */
module;

#include <fmt/format.h>
#include <source_location>

export module cfx.base.result.error;
export import :types;

namespace cfx {

export enum class ErrorCode {
  // 基础状态
  Ok,
  Unknown,
  Cancelled,
  InternalError,

  // 参数与状态校验
  InvalidArgument,
  InvalidState,
  Unsupported,

  // IO 与文件系统
  IOError,
  FileNotFound,
  FileTooLarge,
  FileAlreadyExists,
  DiskFull,
  PermissionDenied,

  // 网络
  NetworkError,
  Timeout,
  ConnectionFailed,
  ConnectionReset,
  RateLimited,

  // 解析与数据
  ParseError,
  InvalidFormat,
  DownloadFailed,
  ChecksumMismatch,
  JsonError,
  DataCorrupted,

  // JVM 与进程
  JvmNotFound,
  JvmIncompatible,
  ProcessCreateFailed,
  ProcessLaunchFailed,
  ProcessCrashed,
  OutOfMemory,

  // 版本与依赖
  VersionNotFound,
  VersionResolveFailed,
  AssetMissing,
  DependencyMissing,
  DependencyConflict,

  // 认证与安全
  AuthFailed,
  AuthExpired,
  TokenInvalid,

  // 插件与脚本
  PluginLoadFailed,
  PluginError,
  PluginDisabled,
  ScriptError,
  ScriptTimeout,

  // 配置与热重载
  ConfigLoadFailed,
  ConfigWatchFailed,
  ConfigParseError,
  HotReloadFailed
};

export enum class ErrorCategory {
  None,
  System,
  Parse,
  IO,
  Network,
  Security,
  Config,
  Runtime,
  Minecraft,
  Auth,
  Plugin,
  Script
};

export class Error {
public:
  Error(ErrorCategory category, ErrorCode code, String message,
        std::source_location location = std::source_location::current());

  template <typename... Args>
  [[nodiscard]]
  static Error
  Format(ErrorCategory category, ErrorCode code,
         fmt::format_string<Args...> fmt, Args &&...args,
         std::source_location location = std::source_location::current());


  Error(ErrorCategory category, ErrorCode code, String message,
        SharedPtr<const Error> cause,
        std::source_location location = std::source_location::current());

  [[nodiscard]] ErrorCode Code() const noexcept;
  [[nodiscard]]
  StringView Message() const noexcept;
  [[nodiscard]]
  const std::source_location &Location() const noexcept;

  [[nodiscard]]
  static constexpr StringView ToString(ErrorCode) noexcept;

  [[nodiscard]]
  static constexpr StringView ToString(ErrorCategory) noexcept;

  [[nodiscard]]
  String ToString() const;

  [[nodiscard]]
  ErrorCategory Category() const noexcept;

  auto WithCause(Error cause) -> Error;

  // 注意，仅在你不会使用原对象时使用此函数
  auto rWithCause(Error &&cause) -> Error;

  [[nodiscard]]
  auto HasCause() const -> bool;

  [[nodiscard]]
  auto Cause() const -> const Error *;

  [[nodiscard]]
  auto ChainDepth() const -> usize;

private:
  ErrorCode code_;
  ErrorCategory category_;
  String message_;
  std::source_location location_;
  SharedPtr<const Error> cause_;
};

}  // namespace cfx
