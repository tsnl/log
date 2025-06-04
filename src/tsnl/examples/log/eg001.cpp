#include <tsnl/log.hpp>

auto main() -> int {
    tsnl::log::set_min_level(tsnl::log::level::debug);

    tsnl::log::debug() << "This is a debug message.";
    tsnl::log::info() << "This is an info message.";
    tsnl::log::warning() << "This is a warning message.";
    tsnl::log::error() << "This is an error message.";

    tsnl::log::info() << "We print multi-line messages correctly:\n"
                      << "- this is line 1\n"
                      << "- this is line 2\n"
                      << "We still terminate with a newline.";

    return 0;
}
