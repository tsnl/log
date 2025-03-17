#include <tsnl/log.hpp>

auto main() -> int {
    tsnl::log::set_min_level(tsnl::log::level::info);
    tsnl::log::info() << "This is an info message.";
    tsnl::log::warning() << "This is a warning message.";
    tsnl::log::error() << "This is an error message.";
    tsnl::log::fatal() << "This is a fatal message: terminating.";
    return 0;
}