export module cfx.base.result.error:category;


namespace cfx {
export enum class ErrorCategory {
  General,
  Render,
  Engine,
  Network,
  Resource,
  File,
  Audio,
  Physics,
  Script,
  Input,
  Build,
  Unknown,
};
}
