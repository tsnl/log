#pragma once

#include <exception>
#include <iostream>
#include <mutex>

//
// interface: log:
// e.g. log::info() << "Hello, World!" << 42;
// Note that one newline is automatically appended.
// Otherwise, you should use `\n` even on platforms expecting `\r\n`.
//

namespace tsnl::log {

class logger;

inline auto info() -> logger;
inline auto warning() -> logger;
inline auto error() -> logger;
inline auto fatal() -> logger;

enum class level {
    info,
    warning,
    error,
    fatal
};

inline void set_min_level(level l);
inline void set_locking_enabled(bool v);

class logger {
    friend void set_min_level(level l);
    friend void set_locking_enabled(bool v);

public:
    inline explicit logger(level l);
    inline ~logger();

    logger(logger const&) = delete;
    logger(logger&&) = delete;

    auto operator=(logger const&) -> logger& = delete;
    auto operator=(logger&&) -> logger& = delete;

    template <typename T>
    inline auto operator<<(T const& value) -> logger&;

private:
    bool active_;
    level level_;
    std::unique_lock<std::mutex> lock_;

    inline static level min_level_ = level::info;
    inline static bool locking_enabled_ = true;
    inline static std::mutex mutex_;
};

} // namespace tsnl::log

//
// implementation:
//

namespace tsnl::log::detail {

constexpr auto reset = "\033[0m";
constexpr auto bold = "\033[1m";
constexpr auto red = "\033[31m";
constexpr auto yellow = "\033[33m";
constexpr auto blue = "\033[34m";

inline auto level_to_color(level lvl) -> char const*;
inline auto level_to_char(level lvl) -> char;

inline auto level_to_color(level lvl) -> char const* {
    switch (lvl) {
    case level::info:
        return blue;
    case level::warning:
        return yellow;
    case level::error:
    case level::fatal:
        return red;
    }
    return "";
}

inline auto level_to_char(level lvl) -> char {
    switch (lvl) {
    case level::info:
        return 'I';
    case level::warning:
        return 'W';
    case level::error:
        return 'E';
    case level::fatal:
        return 'F';
    }
    return '?';
}

} // namespace tsnl::log::detail

namespace tsnl::log {

inline logger::logger(level l)
: active_(static_cast<int>(l) >= static_cast<int>(logger::min_level_)),
  level_(l),
  lock_(locking_enabled_ and active_ ? std::unique_lock(mutex_) : std::unique_lock(mutex_, std::defer_lock)) {
    if (active_) {
        std::cerr << detail::bold << detail::level_to_color(level_) << detail::level_to_char(level_) << detail::reset
                  << ' ';
    }
}

inline logger::~logger() {
    if (active_) {
        std::cerr << '\n';
    }
    if (level_ == level::fatal) {
        std::terminate();
    }
}

template <typename T>
inline auto logger::operator<<(T const& value) -> logger& {
    if (active_) {
        std::cerr << value;
    }
    return *this;
}

inline auto info() -> logger {
    return logger(level::info);
}

inline auto warning() -> logger {
    return logger(level::warning);
}

inline auto error() -> logger {
    return logger(level::error);
}

inline auto fatal() -> logger {
    return logger(level::fatal);
}

inline void set_min_level(level l) {
    logger::min_level_ = l;
}

inline void set_locking_enabled(bool v) {
    logger::locking_enabled_ = v;
}

} // namespace tsnl::log
