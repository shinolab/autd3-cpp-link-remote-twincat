#include "autd3.hpp"
#include "autd3_link_remote_twincat.hpp"
#include "util.hpp"

int main() try {
  auto autd =
      autd3::Controller::open(std::vector{autd3::AUTD3{}}, autd3::link::RemoteTwinCAT("0.0.0.0", "1.1.1.1.1.1.", autd3::link::RemoteTwinCATOption{}));

  autd.send((autd3::Sine(150 * autd3::Hz, autd3::SineOption{}), autd3::Focus(autd.center() + autd3::Vector3(0.0, 0.0, 150.0), autd3::FocusOption{})));

  std::string in;
  getline(std::cin, in);

  autd.close();

  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
