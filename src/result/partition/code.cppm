module;

#include <system_error>

export module cfx.base.result.error:code;
import :category;


namespace cfx {
export enum class ErrorCode {
  // ---------- General 0 ~ 99 ----------
  Success                    = 0,
  Unknown                    = 1,
  InvalidArgument            = 2,
  NotSupported               = 3,
  NotImplemented             = 4,
  OutOfRange                 = 5,
  AlreadyExists              = 6,
  NotFound                   = 7,
  TimedOut                   = 8,
  Canceled                   = 9,
  PermissionDenied           = 10,
  InsufficientMemory         = 11,
  BufferTooSmall             = 12,
  InvalidState               = 13,
  ResourceBusy               = 14,
  InternalError              = 15,

  // ---------- Render 100 ~ 199 ----------
  RendererNotInitialized     = 100,
  RendererAlreadyInitialized = 101,
  DeviceLost                 = 102,
  DeviceRemoved              = 103,
  DriverError                = 104,
  ShaderCompileFailed        = 105,
  ShaderLinkFailed           = 106,
  ProgramCreateFailed        = 107,
  TextureCreateFailed        = 108,
  TextureFormatUnsupported   = 109,
  BufferCreateFailed         = 110,
  FramebufferIncomplete      = 111,
  RenderTargetInvalid        = 112,
  VertexLayoutMismatch       = 113,
  DrawCallFailed             = 114,
  PipelineCreateFailed       = 115,
  SurfaceLost                = 116,
  OutOfVideoMemory           = 117,
  SwapChainCreateFailed      = 118,
  PresentFailed              = 119,

  // ---------- Engine 200 ~ 299 ----------
  EngineNotInitialized       = 200,
  EngineAlreadyRunning       = 201,
  SceneLoadFailed            = 202,
  SceneNotFound              = 203,
  EntityNotFound             = 204,
  ComponentNotFound          = 205,
  ComponentAlreadyAttached   = 206,
  ComponentTypeMismatch      = 207,
  PrefabInstantiateFailed    = 208,
  SystemUpdateFailed         = 209,
  EntityLimitReached         = 210,
  WorldCreateFailed          = 211,
  SerializationFailed        = 212,
  DeserializationFailed      = 213,
  TickOverrun                = 214,

  // ---------- Network 300 ~ 399 ----------
  SocketCreateFailed         = 300,
  SocketBindFailed           = 301,
  SocketListenFailed         = 302,
  SocketAcceptFailed         = 303,
  SocketConnectFailed        = 304,
  ConnectionRefused          = 305,
  ConnectionReset            = 306,
  ConnectionClosed           = 307,
  ConnectionTimedOut         = 308,
  HostNotFound               = 309,
  AddressInUse               = 310,
  NetworkUnreachable         = 311,
  SendFailed                 = 312,
  ReceiveFailed              = 313,
  PacketTooLarge             = 314,
  PacketCorrupted            = 315,
  ProtocolMismatch           = 316,
  HandshakeFailed            = 317,
  DnsResolveFailed           = 318,
  TooManyConnections         = 319,

  // ---------- Resource 400 ~ 499 ----------
  AssetLoadFailed            = 400,
  AssetNotFound              = 401,
  AssetCorrupted             = 402,
  AssetFormatUnsupported     = 403,
  AssetImportFailed          = 404,
  AssetCacheMiss             = 405,
  AssetDependencyMissing     = 406,
  AssetCircularDependency    = 407,
  AssetVersionMismatch       = 408,
  BundleMountFailed          = 409,

  // ---------- File 500 ~ 599 ----------
  FileOpenFailed             = 500,
  FileReadFailed             = 501,
  FileWriteFailed            = 502,
  FileNotFound               = 503,
  FileAlreadyExists          = 504,
  DirectoryNotFound          = 505,
  DirectoryNotEmpty          = 506,
  PathInvalid                = 507,
  PathTooLong                = 508,
  DiskFull                   = 509,
  ReadOnlyFileSystem         = 510,
  FileLocked                 = 511,
  EndOfFile                  = 512,

  // ---------- Audio 600 ~ 699 ----------
  AudioDeviceNotFound        = 600,
  AudioDeviceOpenFailed      = 601,
  AudioFormatUnsupported     = 602,
  AudioDecodeFailed          = 603,
  AudioEncodeFailed          = 604,
  AudioPlaybackFailed        = 605,
  AudioBufferUnderrun        = 606,
  AudioBufferOverrun         = 607,
  MixerInitFailed            = 608,

  // ---------- Physics 700 ~ 799 ----------
  PhysicsWorldCreateFailed   = 700,
  RigidBodyCreateFailed      = 701,
  ColliderCreateFailed       = 702,
  CollisionShapeInvalid      = 703,
  ConstraintSolveFailed      = 704,
  RaycastFailed              = 705,
  BroadphaseOverflow         = 706,
  SimulationDiverged         = 707,

  // ---------- Script 800 ~ 899 ----------
  ScriptCompileFailed        = 800,
  ScriptRuntimeError         = 801,
  ScriptNotFound             = 802,
  ScriptBindFailed           = 803,
  ScriptTypeMismatch         = 804,
  ScriptStackOverflow        = 805,
  ScriptTimeout              = 806,

  // ---------- Input 900 ~ 999 ----------
  InputDeviceNotFound        = 900,
  InputDeviceOpenFailed      = 901,
  InputMappingConflict       = 902,
  InputContextNotActive      = 903,

  // ---------- Build 1000 ~ 1099 ----------
  // 构建系统 / 配置
  BuildConfigInvalid         = 1000,
  BuildConfigNotFound        = 1001,
  BuildConfigParseFailed     = 1002,
  BuildTargetNotFound        = 1003,
  BuildTargetAmbiguous       = 1004,
  BuildRuleNotFound          = 1005,
  BuildRuleConflict          = 1006,
  BuildToolchainNotFound     = 1007,
  BuildToolchainMismatch     = 1008,
  BuildGeneratorFailed       = 1009,

  // 依赖解析
  DependencyResolveFailed    = 1010,
  DependencyNotFound         = 1011,
  DependencyVersionConflict  = 1012,
  DependencyCycleDetected    = 1013,
  DependencyGraphInvalid     = 1014,
  PackageFetchFailed         = 1015,
  PackageIntegrityFailed     = 1016,
  PackageVersionMismatch     = 1017,
  PackageNotInstalled        = 1018,

  // 编译
  CompileFailed              = 1020,
  CompileTimeout             = 1021,
  CompilerNotFound           = 1022,
  CompilerInvocationFailed   = 1023,
  PreprocessFailed           = 1024,
  SyntaxError                = 1025,
  SemanticError              = 1026,
  HeaderNotFound             = 1027,
  IncludeCycleDetected       = 1028,
  MacroRedefinition          = 1029,

  // 链接
  LinkFailed                 = 1030,
  LinkerNotFound             = 1031,
  SymbolUndefined            = 1032,
  SymbolDuplicate            = 1033,
  LibraryNotFound            = 1034,
  LibraryVersionMismatch     = 1035,
  AbiMismatch                = 1036,

  // 构建执行
  BuildFailed                = 1040,
  BuildCanceled              = 1041,
  BuildTimeout               = 1042,
  BuildStepFailed            = 1043,
  BuildStepSkipped           = 1044,
  BuildGraphInvalid          = 1045,
  BuildLockAcquireFailed     = 1046,
  BuildLockHeld              = 1047,
  BuildOutputDirInvalid      = 1048,
  BuildOutputConflict        = 1049,

  // 增量 / 缓存
  IncrementalStateCorrupted  = 1050,
  TimestampOutOfOrder        = 1051,
  CacheMiss                  = 1052,
  CacheCorrupted             = 1053,
  CacheWriteFailed           = 1054,
  CacheEvictionFailed        = 1055,
  ArtifactStale              = 1056,

  // 打包 / 产物
  PackageArchiveFailed       = 1060,
  PackageSignFailed          = 1061,
  PackageVerifyFailed        = 1062,
  ArtifactNotFound           = 1063,
  ArtifactChecksumMismatch   = 1064,
  ArtifactUploadFailed       = 1065,
  ArtifactDownloadFailed     = 1066,
  InstallFailed              = 1067,
  UninstallFailed            = 1068,
};

export constexpr auto
ErrorCodeFromStdErrorCode(const std::error_code &ec) noexcept -> ErrorCode {
  if (!ec) {
    return ErrorCode::Success;
  }

  switch (static_cast<std::errc>(ec.value())) {
  case std::errc::address_family_not_supported:
    return ErrorCode::NetworkUnreachable;
  case std::errc::address_in_use:
    return ErrorCode::AddressInUse;
  case std::errc::address_not_available:
    return ErrorCode::AddressInUse;
  case std::errc::already_connected:
    return ErrorCode::ConnectionRefused;
  case std::errc::argument_list_too_long:
    return ErrorCode::InvalidArgument;
  case std::errc::argument_out_of_domain:
    return ErrorCode::InvalidArgument;
  case std::errc::bad_address:
    return ErrorCode::PathInvalid;
  case std::errc::bad_file_descriptor:
    return ErrorCode::FileOpenFailed;
  case std::errc::bad_message:
    return ErrorCode::ProtocolMismatch;
  case std::errc::broken_pipe:
    return ErrorCode::ConnectionClosed;
  case std::errc::connection_aborted:
    return ErrorCode::ConnectionClosed;
  case std::errc::connection_already_in_progress:
    return ErrorCode::SocketConnectFailed;
  case std::errc::connection_refused:
    return ErrorCode::ConnectionRefused;
  case std::errc::connection_reset:
    return ErrorCode::ConnectionReset;
  case std::errc::cross_device_link:
    return ErrorCode::FileWriteFailed;
  case std::errc::destination_address_required:
    return ErrorCode::SocketConnectFailed;
  case std::errc::device_or_resource_busy:
    return ErrorCode::ResourceBusy;
  case std::errc::directory_not_empty:
    return ErrorCode::DirectoryNotEmpty;
  case std::errc::executable_format_error:
    return ErrorCode::FileReadFailed;
  case std::errc::file_exists:
    return ErrorCode::FileAlreadyExists;
  case std::errc::file_too_large:
    return ErrorCode::FileWriteFailed;
  case std::errc::filename_too_long:
    return ErrorCode::PathTooLong;
  case std::errc::function_not_supported:
    return ErrorCode::NotSupported;
  case std::errc::host_unreachable:
    return ErrorCode::HostNotFound;
  case std::errc::identifier_removed:
    return ErrorCode::Canceled;
  case std::errc::illegal_byte_sequence:
    return ErrorCode::InvalidArgument;
  case std::errc::inappropriate_io_control_operation:
    return ErrorCode::FileReadFailed;
  case std::errc::interrupted:
    return ErrorCode::Canceled;
  case std::errc::invalid_argument:
    return ErrorCode::InvalidArgument;
  case std::errc::invalid_seek:
    return ErrorCode::FileReadFailed;
  case std::errc::io_error:
    return ErrorCode::FileReadFailed;
  case std::errc::is_a_directory:
    return ErrorCode::FileOpenFailed;
  case std::errc::message_size:
    return ErrorCode::PacketTooLarge;
  case std::errc::network_down:
    return ErrorCode::NetworkUnreachable;
  case std::errc::network_reset:
    return ErrorCode::ConnectionReset;
  case std::errc::network_unreachable:
    return ErrorCode::NetworkUnreachable;
  case std::errc::no_buffer_space:
    return ErrorCode::SendFailed;
  case std::errc::no_child_process:
    return ErrorCode::NotFound;
  case std::errc::no_link:
    return ErrorCode::NotFound;
  case std::errc::no_lock_available:
    return ErrorCode::ResourceBusy;
  case std::errc::no_message:
    return ErrorCode::NotFound;
  case std::errc::no_protocol_option:
    return ErrorCode::ProtocolMismatch;
  case std::errc::no_space_on_device:
    return ErrorCode::DiskFull;
  case std::errc::no_such_device_or_address:
    return ErrorCode::FileNotFound;
  case std::errc::no_such_device:
    return ErrorCode::FileNotFound;
  case std::errc::no_such_file_or_directory:
    return ErrorCode::FileNotFound;
  case std::errc::no_such_process:
    return ErrorCode::NotFound;
  case std::errc::not_a_directory:
    return ErrorCode::DirectoryNotFound;
  case std::errc::not_a_socket:
    return ErrorCode::SocketCreateFailed;
  case std::errc::not_connected:
    return ErrorCode::ConnectionClosed;
  case std::errc::not_enough_memory:
    return ErrorCode::InsufficientMemory;
  case std::errc::not_supported:
    return ErrorCode::NotSupported;
  case std::errc::operation_canceled:
    return ErrorCode::Canceled;
  case std::errc::operation_in_progress:
    return ErrorCode::SocketConnectFailed;
  case std::errc::operation_not_permitted:
    return ErrorCode::PermissionDenied;
  case std::errc::operation_would_block:
    return ErrorCode::SendFailed;
  case std::errc::owner_dead:
    return ErrorCode::InvalidState;
  case std::errc::permission_denied:
    return ErrorCode::PermissionDenied;
  case std::errc::protocol_error:
    return ErrorCode::ProtocolMismatch;
  case std::errc::protocol_not_supported:
    return ErrorCode::ProtocolMismatch;
  case std::errc::read_only_file_system:
    return ErrorCode::ReadOnlyFileSystem;
  case std::errc::resource_deadlock_would_occur:
    return ErrorCode::ResourceBusy;
  case std::errc::result_out_of_range:
    return ErrorCode::OutOfRange;
  case std::errc::state_not_recoverable:
    return ErrorCode::InvalidState;
  case std::errc::text_file_busy:
    return ErrorCode::FileLocked;
  case std::errc::timed_out:
    return ErrorCode::TimedOut;
  case std::errc::too_many_files_open_in_system:
    return ErrorCode::FileOpenFailed;
  case std::errc::too_many_files_open:
    return ErrorCode::FileOpenFailed;
  case std::errc::too_many_links:
    return ErrorCode::FileOpenFailed;
  case std::errc::too_many_symbolic_link_levels:
    return ErrorCode::FileOpenFailed;
  case std::errc::value_too_large:
    return ErrorCode::OutOfRange;
  case std::errc::wrong_protocol_type:
    return ErrorCode::ProtocolMismatch;

  default:
    return ErrorCode::Unknown;
  }
}


export constexpr auto
CategoryFromStdErrorCode(const std::error_code &ec) noexcept -> ErrorCategory {
  if (!ec) {
    return ErrorCategory::General;
  }

  switch (static_cast<std::errc>(ec.value())) {
  case std::errc::address_family_not_supported:
  case std::errc::address_in_use:
  case std::errc::address_not_available:
  case std::errc::already_connected:
  case std::errc::connection_aborted:
  case std::errc::connection_already_in_progress:
  case std::errc::connection_refused:
  case std::errc::connection_reset:
  case std::errc::destination_address_required:
  case std::errc::host_unreachable:
  case std::errc::message_size:
  case std::errc::network_down:
  case std::errc::network_reset:
  case std::errc::network_unreachable:
  case std::errc::no_buffer_space:
  case std::errc::no_protocol_option:
  case std::errc::not_a_socket:
  case std::errc::not_connected:
  case std::errc::operation_in_progress:
  case std::errc::operation_would_block:
  case std::errc::protocol_error:
  case std::errc::protocol_not_supported:
  case std::errc::wrong_protocol_type:
  case std::errc::broken_pipe:
    return ErrorCategory::Network;


  case std::errc::bad_file_descriptor:
  case std::errc::bad_address:
  case std::errc::cross_device_link:
  case std::errc::directory_not_empty:
  case std::errc::executable_format_error:
  case std::errc::file_exists:
  case std::errc::file_too_large:
  case std::errc::filename_too_long:
  case std::errc::inappropriate_io_control_operation:
  case std::errc::invalid_seek:
  case std::errc::io_error:
  case std::errc::is_a_directory:
  case std::errc::no_space_on_device:
  case std::errc::no_such_device:
  case std::errc::no_such_device_or_address:
  case std::errc::no_such_file_or_directory:
  case std::errc::not_a_directory:
  case std::errc::read_only_file_system:
  case std::errc::text_file_busy:
  case std::errc::too_many_files_open:
  case std::errc::too_many_files_open_in_system:
  case std::errc::too_many_links:
  case std::errc::too_many_symbolic_link_levels:
    return ErrorCategory::File;


  case std::errc::device_or_resource_busy:
  case std::errc::no_lock_available:
  case std::errc::resource_deadlock_would_occur:
  case std::errc::not_enough_memory:

  default:
    return ErrorCategory::General;
  }
}

export constexpr ErrorCategory CategoryOf(ErrorCode code) noexcept {
  const int v = static_cast<int>(code);

  if (v >= 0 && v < 100)
    return ErrorCategory::General;
  if (v >= 100 && v < 200)
    return ErrorCategory::Render;
  if (v >= 200 && v < 300)
    return ErrorCategory::Engine;
  if (v >= 300 && v < 400)
    return ErrorCategory::Network;
  if (v >= 400 && v < 500)
    return ErrorCategory::Resource;
  if (v >= 500 && v < 600)
    return ErrorCategory::File;
  if (v >= 600 && v < 700)
    return ErrorCategory::Audio;
  if (v >= 700 && v < 800)
    return ErrorCategory::Physics;
  if (v >= 800 && v < 900)
    return ErrorCategory::Script;
  if (v >= 900 && v < 1000)
    return ErrorCategory::Input;
  if (v >= 1000 && v < 1100)
    return ErrorCategory::Build;

  return ErrorCategory::Unknown;
}
}  // namespace cfx
