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

#include <fmt/base.h>
#include <fmt/format.h>
#include <memory>
#include <source_location>
#include <utility>

export module cfx.base.result.error;
import :types;
export import :code;
export import :category;

namespace cfx {


export class Error {
public:
  Error(ErrorCategory category, ErrorCode code, String message,
        std::source_location location = std::source_location::current());

  Error(ErrorCode code, String message,
        std::source_location location = std::source_location::current());

  template <typename... Args>
  [[nodiscard]]
  static auto
  Format(ErrorCategory category, ErrorCode code, std::source_location location,
         fmt::format_string<Args...> fmt, Args &&...args) -> Error {
    return Error{
            category, code,
            std::move(fmt::format(fmt, std::forward<Args>(args)...)), location
    };
  }

  template <typename... Args>
  [[nodiscard]]
  static auto
  Format(ErrorCode code, std::source_location location,
         fmt::format_string<Args...> fmt, Args &&...args) -> Error {
    return Error{
            code, std::move(fmt::format(fmt, std::forward<Args>(args)...)),
            location
    };
  };


  Error(ErrorCategory category, ErrorCode code, String message,
        SharedPtr<const Error> cause,
        std::source_location location = std::source_location::current());

  [[nodiscard]] inline auto Code() const noexcept -> ErrorCode {
    return this->code_;
  }

  [[nodiscard]]
  inline auto Message() const noexcept -> StringView {
    return this->message_;
  };
  [[nodiscard]]
  inline auto Location() const noexcept -> const std::source_location & {
    return this->location_;
  };


  [[nodiscard]]
  static constexpr StringView ToString(ErrorCategory cat) noexcept {
    switch (cat) {
    case ErrorCategory::General:
      return "General";
    case ErrorCategory::Render:
      return "Render";
    case ErrorCategory::Engine:
      return "Engine";
    case ErrorCategory::Network:
      return "Network";
    case ErrorCategory::Resource:
      return "Resource";
    case ErrorCategory::File:
      return "File";
    case ErrorCategory::Audio:
      return "Audio";
    case ErrorCategory::Physics:
      return "Physics";
    case ErrorCategory::Script:
      return "Script";
    case ErrorCategory::Input:
      return "Input";
    case ErrorCategory::Build:
      return "Build";
    case ErrorCategory::Unknown:
      return "Unknown";
    default:
      return "Unknown";
    }
  }
  [[nodiscard]]
  static constexpr StringView ToString(ErrorCode code) noexcept {
    switch (code) {
    // ---------- General ----------
    case ErrorCode::Success:
      return "Success";
    case ErrorCode::Unknown:
      return "Unknown";
    case ErrorCode::InvalidArgument:
      return "InvalidArgument";
    case ErrorCode::NotSupported:
      return "NotSupported";
    case ErrorCode::NotImplemented:
      return "NotImplemented";
    case ErrorCode::OutOfRange:
      return "OutOfRange";
    case ErrorCode::AlreadyExists:
      return "AlreadyExists";
    case ErrorCode::NotFound:
      return "NotFound";
    case ErrorCode::TimedOut:
      return "TimedOut";
    case ErrorCode::Canceled:
      return "Canceled";
    case ErrorCode::PermissionDenied:
      return "PermissionDenied";
    case ErrorCode::InsufficientMemory:
      return "InsufficientMemory";
    case ErrorCode::BufferTooSmall:
      return "BufferTooSmall";
    case ErrorCode::InvalidState:
      return "InvalidState";
    case ErrorCode::ResourceBusy:
      return "ResourceBusy";
    case ErrorCode::InternalError:
      return "InternalError";

    // ---------- Render ----------
    case ErrorCode::RendererNotInitialized:
      return "RendererNotInitialized";
    case ErrorCode::RendererAlreadyInitialized:
      return "RendererAlreadyInitialized";
    case ErrorCode::DeviceLost:
      return "DeviceLost";
    case ErrorCode::DeviceRemoved:
      return "DeviceRemoved";
    case ErrorCode::DriverError:
      return "DriverError";
    case ErrorCode::ShaderCompileFailed:
      return "ShaderCompileFailed";
    case ErrorCode::ShaderLinkFailed:
      return "ShaderLinkFailed";
    case ErrorCode::ProgramCreateFailed:
      return "ProgramCreateFailed";
    case ErrorCode::TextureCreateFailed:
      return "TextureCreateFailed";
    case ErrorCode::TextureFormatUnsupported:
      return "TextureFormatUnsupported";
    case ErrorCode::BufferCreateFailed:
      return "BufferCreateFailed";
    case ErrorCode::FramebufferIncomplete:
      return "FramebufferIncomplete";
    case ErrorCode::RenderTargetInvalid:
      return "RenderTargetInvalid";
    case ErrorCode::VertexLayoutMismatch:
      return "VertexLayoutMismatch";
    case ErrorCode::DrawCallFailed:
      return "DrawCallFailed";
    case ErrorCode::PipelineCreateFailed:
      return "PipelineCreateFailed";
    case ErrorCode::SurfaceLost:
      return "SurfaceLost";
    case ErrorCode::OutOfVideoMemory:
      return "OutOfVideoMemory";
    case ErrorCode::SwapChainCreateFailed:
      return "SwapChainCreateFailed";
    case ErrorCode::PresentFailed:
      return "PresentFailed";

    // ---------- Engine ----------
    case ErrorCode::EngineNotInitialized:
      return "EngineNotInitialized";
    case ErrorCode::EngineAlreadyRunning:
      return "EngineAlreadyRunning";
    case ErrorCode::SceneLoadFailed:
      return "SceneLoadFailed";
    case ErrorCode::SceneNotFound:
      return "SceneNotFound";
    case ErrorCode::EntityNotFound:
      return "EntityNotFound";
    case ErrorCode::ComponentNotFound:
      return "ComponentNotFound";
    case ErrorCode::ComponentAlreadyAttached:
      return "ComponentAlreadyAttached";
    case ErrorCode::ComponentTypeMismatch:
      return "ComponentTypeMismatch";
    case ErrorCode::PrefabInstantiateFailed:
      return "PrefabInstantiateFailed";
    case ErrorCode::SystemUpdateFailed:
      return "SystemUpdateFailed";
    case ErrorCode::EntityLimitReached:
      return "EntityLimitReached";
    case ErrorCode::WorldCreateFailed:
      return "WorldCreateFailed";
    case ErrorCode::SerializationFailed:
      return "SerializationFailed";
    case ErrorCode::DeserializationFailed:
      return "DeserializationFailed";
    case ErrorCode::TickOverrun:
      return "TickOverrun";

    // ---------- Network ----------
    case ErrorCode::SocketCreateFailed:
      return "SocketCreateFailed";
    case ErrorCode::SocketBindFailed:
      return "SocketBindFailed";
    case ErrorCode::SocketListenFailed:
      return "SocketListenFailed";
    case ErrorCode::SocketAcceptFailed:
      return "SocketAcceptFailed";
    case ErrorCode::SocketConnectFailed:
      return "SocketConnectFailed";
    case ErrorCode::ConnectionRefused:
      return "ConnectionRefused";
    case ErrorCode::ConnectionReset:
      return "ConnectionReset";
    case ErrorCode::ConnectionClosed:
      return "ConnectionClosed";
    case ErrorCode::ConnectionTimedOut:
      return "ConnectionTimedOut";
    case ErrorCode::HostNotFound:
      return "HostNotFound";
    case ErrorCode::AddressInUse:
      return "AddressInUse";
    case ErrorCode::NetworkUnreachable:
      return "NetworkUnreachable";
    case ErrorCode::SendFailed:
      return "SendFailed";
    case ErrorCode::ReceiveFailed:
      return "ReceiveFailed";
    case ErrorCode::PacketTooLarge:
      return "PacketTooLarge";
    case ErrorCode::PacketCorrupted:
      return "PacketCorrupted";
    case ErrorCode::ProtocolMismatch:
      return "ProtocolMismatch";
    case ErrorCode::HandshakeFailed:
      return "HandshakeFailed";
    case ErrorCode::DnsResolveFailed:
      return "DnsResolveFailed";
    case ErrorCode::TooManyConnections:
      return "TooManyConnections";

    // ---------- Resource ----------
    case ErrorCode::AssetLoadFailed:
      return "AssetLoadFailed";
    case ErrorCode::AssetNotFound:
      return "AssetNotFound";
    case ErrorCode::AssetCorrupted:
      return "AssetCorrupted";
    case ErrorCode::AssetFormatUnsupported:
      return "AssetFormatUnsupported";
    case ErrorCode::AssetImportFailed:
      return "AssetImportFailed";
    case ErrorCode::AssetCacheMiss:
      return "AssetCacheMiss";
    case ErrorCode::AssetDependencyMissing:
      return "AssetDependencyMissing";
    case ErrorCode::AssetCircularDependency:
      return "AssetCircularDependency";
    case ErrorCode::AssetVersionMismatch:
      return "AssetVersionMismatch";
    case ErrorCode::BundleMountFailed:
      return "BundleMountFailed";

    // ---------- File ----------
    case ErrorCode::FileOpenFailed:
      return "FileOpenFailed";
    case ErrorCode::FileReadFailed:
      return "FileReadFailed";
    case ErrorCode::FileWriteFailed:
      return "FileWriteFailed";
    case ErrorCode::FileNotFound:
      return "FileNotFound";
    case ErrorCode::FileAlreadyExists:
      return "FileAlreadyExists";
    case ErrorCode::DirectoryNotFound:
      return "DirectoryNotFound";
    case ErrorCode::DirectoryNotEmpty:
      return "DirectoryNotEmpty";
    case ErrorCode::PathInvalid:
      return "PathInvalid";
    case ErrorCode::PathTooLong:
      return "PathTooLong";
    case ErrorCode::DiskFull:
      return "DiskFull";
    case ErrorCode::ReadOnlyFileSystem:
      return "ReadOnlyFileSystem";
    case ErrorCode::FileLocked:
      return "FileLocked";
    case ErrorCode::EndOfFile:
      return "EndOfFile";

    // ---------- Audio ----------
    case ErrorCode::AudioDeviceNotFound:
      return "AudioDeviceNotFound";
    case ErrorCode::AudioDeviceOpenFailed:
      return "AudioDeviceOpenFailed";
    case ErrorCode::AudioFormatUnsupported:
      return "AudioFormatUnsupported";
    case ErrorCode::AudioDecodeFailed:
      return "AudioDecodeFailed";
    case ErrorCode::AudioEncodeFailed:
      return "AudioEncodeFailed";
    case ErrorCode::AudioPlaybackFailed:
      return "AudioPlaybackFailed";
    case ErrorCode::AudioBufferUnderrun:
      return "AudioBufferUnderrun";
    case ErrorCode::AudioBufferOverrun:
      return "AudioBufferOverrun";
    case ErrorCode::MixerInitFailed:
      return "MixerInitFailed";

    // ---------- Physics ----------
    case ErrorCode::PhysicsWorldCreateFailed:
      return "PhysicsWorldCreateFailed";
    case ErrorCode::RigidBodyCreateFailed:
      return "RigidBodyCreateFailed";
    case ErrorCode::ColliderCreateFailed:
      return "ColliderCreateFailed";
    case ErrorCode::CollisionShapeInvalid:
      return "CollisionShapeInvalid";
    case ErrorCode::ConstraintSolveFailed:
      return "ConstraintSolveFailed";
    case ErrorCode::RaycastFailed:
      return "RaycastFailed";
    case ErrorCode::BroadphaseOverflow:
      return "BroadphaseOverflow";
    case ErrorCode::SimulationDiverged:
      return "SimulationDiverged";

    // ---------- Script ----------
    case ErrorCode::ScriptCompileFailed:
      return "ScriptCompileFailed";
    case ErrorCode::ScriptRuntimeError:
      return "ScriptRuntimeError";
    case ErrorCode::ScriptNotFound:
      return "ScriptNotFound";
    case ErrorCode::ScriptBindFailed:
      return "ScriptBindFailed";
    case ErrorCode::ScriptTypeMismatch:
      return "ScriptTypeMismatch";
    case ErrorCode::ScriptStackOverflow:
      return "ScriptStackOverflow";
    case ErrorCode::ScriptTimeout:
      return "ScriptTimeout";

    // ---------- Input ----------
    case ErrorCode::InputDeviceNotFound:
      return "InputDeviceNotFound";
    case ErrorCode::InputDeviceOpenFailed:
      return "InputDeviceOpenFailed";
    case ErrorCode::InputMappingConflict:
      return "InputMappingConflict";
    case ErrorCode::InputContextNotActive:
      return "InputContextNotActive";

    // ---------- Build ----------
    case ErrorCode::BuildConfigInvalid:
      return "BuildConfigInvalid";
    case ErrorCode::BuildConfigNotFound:
      return "BuildConfigNotFound";
    case ErrorCode::BuildConfigParseFailed:
      return "BuildConfigParseFailed";
    case ErrorCode::BuildTargetNotFound:
      return "BuildTargetNotFound";
    case ErrorCode::BuildTargetAmbiguous:
      return "BuildTargetAmbiguous";
    case ErrorCode::BuildRuleNotFound:
      return "BuildRuleNotFound";
    case ErrorCode::BuildRuleConflict:
      return "BuildRuleConflict";
    case ErrorCode::BuildToolchainNotFound:
      return "BuildToolchainNotFound";
    case ErrorCode::BuildToolchainMismatch:
      return "BuildToolchainMismatch";
    case ErrorCode::BuildGeneratorFailed:
      return "BuildGeneratorFailed";

    case ErrorCode::DependencyResolveFailed:
      return "DependencyResolveFailed";
    case ErrorCode::DependencyNotFound:
      return "DependencyNotFound";
    case ErrorCode::DependencyVersionConflict:
      return "DependencyVersionConflict";
    case ErrorCode::DependencyCycleDetected:
      return "DependencyCycleDetected";
    case ErrorCode::DependencyGraphInvalid:
      return "DependencyGraphInvalid";
    case ErrorCode::PackageFetchFailed:
      return "PackageFetchFailed";
    case ErrorCode::PackageIntegrityFailed:
      return "PackageIntegrityFailed";
    case ErrorCode::PackageVersionMismatch:
      return "PackageVersionMismatch";
    case ErrorCode::PackageNotInstalled:
      return "PackageNotInstalled";

    case ErrorCode::CompileFailed:
      return "CompileFailed";
    case ErrorCode::CompileTimeout:
      return "CompileTimeout";
    case ErrorCode::CompilerNotFound:
      return "CompilerNotFound";
    case ErrorCode::CompilerInvocationFailed:
      return "CompilerInvocationFailed";
    case ErrorCode::PreprocessFailed:
      return "PreprocessFailed";
    case ErrorCode::SyntaxError:
      return "SyntaxError";
    case ErrorCode::SemanticError:
      return "SemanticError";
    case ErrorCode::HeaderNotFound:
      return "HeaderNotFound";
    case ErrorCode::IncludeCycleDetected:
      return "IncludeCycleDetected";
    case ErrorCode::MacroRedefinition:
      return "MacroRedefinition";

    case ErrorCode::LinkFailed:
      return "LinkFailed";
    case ErrorCode::LinkerNotFound:
      return "LinkerNotFound";
    case ErrorCode::SymbolUndefined:
      return "SymbolUndefined";
    case ErrorCode::SymbolDuplicate:
      return "SymbolDuplicate";
    case ErrorCode::LibraryNotFound:
      return "LibraryNotFound";
    case ErrorCode::LibraryVersionMismatch:
      return "LibraryVersionMismatch";
    case ErrorCode::AbiMismatch:
      return "AbiMismatch";

    case ErrorCode::BuildFailed:
      return "BuildFailed";
    case ErrorCode::BuildCanceled:
      return "BuildCanceled";
    case ErrorCode::BuildTimeout:
      return "BuildTimeout";
    case ErrorCode::BuildStepFailed:
      return "BuildStepFailed";
    case ErrorCode::BuildStepSkipped:
      return "BuildStepSkipped";
    case ErrorCode::BuildGraphInvalid:
      return "BuildGraphInvalid";
    case ErrorCode::BuildLockAcquireFailed:
      return "BuildLockAcquireFailed";
    case ErrorCode::BuildLockHeld:
      return "BuildLockHeld";
    case ErrorCode::BuildOutputDirInvalid:
      return "BuildOutputDirInvalid";
    case ErrorCode::BuildOutputConflict:
      return "BuildOutputConflict";

    case ErrorCode::IncrementalStateCorrupted:
      return "IncrementalStateCorrupted";
    case ErrorCode::TimestampOutOfOrder:
      return "TimestampOutOfOrder";
    case ErrorCode::CacheMiss:
      return "CacheMiss";
    case ErrorCode::CacheCorrupted:
      return "CacheCorrupted";
    case ErrorCode::CacheWriteFailed:
      return "CacheWriteFailed";
    case ErrorCode::CacheEvictionFailed:
      return "CacheEvictionFailed";
    case ErrorCode::ArtifactStale:
      return "ArtifactStale";

    case ErrorCode::PackageArchiveFailed:
      return "PackageArchiveFailed";
    case ErrorCode::PackageSignFailed:
      return "PackageSignFailed";
    case ErrorCode::PackageVerifyFailed:
      return "PackageVerifyFailed";
    case ErrorCode::ArtifactNotFound:
      return "ArtifactNotFound";
    case ErrorCode::ArtifactChecksumMismatch:
      return "ArtifactChecksumMismatch";
    case ErrorCode::ArtifactUploadFailed:
      return "ArtifactUploadFailed";
    case ErrorCode::ArtifactDownloadFailed:
      return "ArtifactDownloadFailed";
    case ErrorCode::InstallFailed:
      return "InstallFailed";
    case ErrorCode::UninstallFailed:
      return "UninstallFailed";

    default:
      return "Unknown";
    }
  }

  [[nodiscard]]
  auto ToString() const -> String;

  [[nodiscard]]
  inline auto Category() const noexcept -> ErrorCategory {
    return this->category_;
  }

  [[nodiscard]]
  inline auto WithCause(Error cause) -> Error {
    return Error{
            this->category_, this->code_, this->message_,
            std::make_shared<const Error>(std::move(cause)), this->location_
    };
  }

  // 注意，仅在你不会使用原对象时使用此函数
  [[nodiscard]]
  inline auto rWithCause(Error &&cause) -> Error {
    Error out_err{std::move(*this)};
    out_err.cause_ = std::make_shared<const Error>(std::move(cause));
    return out_err;
  };

  [[nodiscard]]
  inline auto HasCause() const -> bool {
    return this->cause_.get() != nullptr;
  }

  [[nodiscard]]
  inline auto Cause() const -> const Error * {
    return this->cause_.get();
  }

  [[nodiscard]]
  auto ChainDepth() const -> usize {
    usize depth      = 0;
    const Error *cur = cause_.get();
    while (cur != nullptr) {
      ++depth;
      cur = cur->cause_.get();
    }
    return depth;
  }

private:
  ErrorCode code_;
  ErrorCategory category_;
  String message_;
  std::source_location location_;
  SharedPtr<const Error> cause_;
};

}  // namespace cfx
