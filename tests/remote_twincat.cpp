#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3_link_remote_twincat.hpp>

TEST(Link, Timeouts) {
  {
    const auto timeouts = autd3::link::Timeouts();
    const auto inner = static_cast<autd3::native_methods::Timeouts>(timeouts);
    EXPECT_FALSE(inner.connect.has_value);
    EXPECT_FALSE(inner.read.has_value);
    EXPECT_FALSE(inner.write.has_value);
  }

  {
    const auto timeouts = autd3::link::Timeouts{
        .connect = std::chrono::nanoseconds(0),
        .read = std::chrono::nanoseconds(1),
        .write = std::chrono::nanoseconds(2),
    };
    const auto inner = static_cast<autd3::native_methods::Timeouts>(timeouts);
    ASSERT_TRUE(inner.connect.has_value);
    EXPECT_EQ(inner.connect.value.nanos, 0);
    ASSERT_TRUE(inner.read.has_value);
    EXPECT_EQ(inner.read.value.nanos, 1);
    ASSERT_TRUE(inner.write.has_value);
    EXPECT_EQ(inner.write.value.nanos, 2);
  }
}

TEST(Link, Source) {
  const auto auto_ = autd3::link::Source::Auto;
  const auto addr = autd3::link::Source::Addr("0.0.0.0.0.0");
  const auto request = autd3::link::Source::Request;

  {
    const auto inner = static_cast<autd3::native_methods::Source>(auto_);
    EXPECT_EQ(inner.tag, autd3::native_methods::SourceTag::Auto);
  }

  {
    const auto inner = static_cast<autd3::native_methods::Source>(addr);
    EXPECT_EQ(inner.tag, autd3::native_methods::SourceTag::Addr);
    EXPECT_STREQ(inner.addr, "0.0.0.0.0.0");
  }

  {
    const auto inner = static_cast<autd3::native_methods::Source>(request);
    EXPECT_EQ(inner.tag, autd3::native_methods::SourceTag::Request);
  }
}

TEST(Link, RemoteTwinCATOption) {
  const auto option = autd3::link::RemoteTwinCATOption();
  const auto inner = static_cast<autd3::native_methods::RemoteTwinCATOption>(option);
  EXPECT_FALSE(inner.timeouts.connect.has_value);
  EXPECT_FALSE(inner.timeouts.read.has_value);
  EXPECT_FALSE(inner.timeouts.write.has_value);
  EXPECT_EQ(inner.source.tag, autd3::native_methods::SourceTag::Auto);
}

TEST(Link, RemoteTwinCAT) {
  auto link = autd3::link::RemoteTwinCAT("0.0.0.0", "1.1.1.1.1.1", autd3::link::RemoteTwinCATOption{});

#ifdef RUN_LINK_REMOTE_TWINCAT
  auto autd = autd3::controller::Controller::open({autd3::driver::AUTD3{}}, std::move(link));

  autd.close();
#else
  (void)link;
#endif
}
