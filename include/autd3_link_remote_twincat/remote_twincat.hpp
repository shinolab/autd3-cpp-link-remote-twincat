#pragma once

#include <chrono>
#include <optional>
#include <string>

#include "autd3/native_methods/utils.hpp"
#include "autd3_link_remote_twincat/native_methods.hpp"

namespace autd3::link {

struct Timeouts {
  std::optional<std::chrono::nanoseconds> connect = std::nullopt;
  std::optional<std::chrono::nanoseconds> read = std::nullopt;
  std::optional<std::chrono::nanoseconds> write = std::nullopt;

  operator native_methods::Timeouts() const {
    return native_methods::Timeouts{
        .connect = native_methods::to_option_duration(connect),
        .read = native_methods::to_option_duration(read),
        .write = native_methods::to_option_duration(write),
    };
  }
};

struct Source {
  static const Source Auto;
  static Source Addr(std::string addr) { return Source(native_methods::SourceTag::Addr, std::move(addr)); }
  static const Source Request;

  operator native_methods::Source() const { return native_methods::Source{.tag = tag, .addr = addr.c_str()}; }

 private:
  explicit Source(const native_methods::SourceTag tag, std::string addr = "") : tag(tag), addr(std::move(addr)) {}

  native_methods::SourceTag tag;
  std::string addr;
};

const Source Source::Auto = Source(native_methods::SourceTag::Auto);
const Source Source::Request = Source(native_methods::SourceTag::Request);

struct RemoteTwinCATOption {
  Timeouts timeouts;
  Source source = Source::Auto;

  operator native_methods::RemoteTwinCATOption() const {
    return native_methods::RemoteTwinCATOption{
        .timeouts = static_cast<native_methods::Timeouts>(timeouts),
        .source = static_cast<native_methods::Source>(source),
    };
  }
};

struct RemoteTwinCAT final {
  explicit RemoteTwinCAT(std::string addr, std::string ams_net_id, RemoteTwinCATOption option)
      : addr(std::move(addr)), ams_net_id(std::move(ams_net_id)), option(std::move(option)) {}

  std::string addr;
  std::string ams_net_id;
  RemoteTwinCATOption option;

  [[nodiscard]] native_methods::LinkPtr resolve() const {
    return validate(
        native_methods::AUTDLinkRemoteTwinCAT(addr.c_str(), ams_net_id.c_str(), static_cast<native_methods::RemoteTwinCATOption>(option)));
  }
};

}  // namespace autd3::link
