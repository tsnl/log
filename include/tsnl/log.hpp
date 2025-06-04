#pragma once

#include <atomic>
#include <iostream>
#include <mutex>
#include <sstream>

//
// interface: log:
// e.g. log::info() << "Hello, World!" << 42;
// Note that one newline is automatically appended.
// Otherwise, you should use `\n` even on platforms expecting `\r\n`.
//

namespace tsnl::log {

class logger;

inline auto debug() -> logger;
inline auto info() -> logger;
inline auto warning() -> logger;
inline auto error() -> logger;

enum class level {
    debug,
    info,
    warning,
    error,
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
    std::stringstream stream_;

    inline static level min_level_ = level::info;
    inline static std::atomic_bool locking_enabled_ = true;
    inline static std::mutex mutex_;
};

} // namespace tsnl::log

//
// implementation:
//

namespace tsnl::log::detail {

constexpr auto reset = "\033[0m";
constexpr auto bold = "\033[1m";
constexpr auto bold_red = "\033[31;1m";
constexpr auto bold_yellow = "\033[33m";
constexpr auto bold_blue = "\033[34;1m";
constexpr auto faint_white = "\033[37;2m";
constexpr auto faint_magenta = "\033[35;2m";

inline auto level_to_color(level lvl) -> char const*;
inline auto level_to_char(level lvl) -> char;

inline auto level_to_color(level lvl) -> char const* {
    switch (lvl) {
    case level::debug:
        return faint_magenta;
    case level::info:
        return faint_white;
    case level::warning:
        return bold_yellow;
    case level::error:
        return bold_red;
    }
    return "";
}

inline auto level_to_char(level lvl) -> char {
    switch (lvl) {
    case level::debug:
        return 'D';
    case level::info:
        return '>';
    case level::warning:
        return 'W';
    case level::error:
        return 'E';
    }
    return '?';
}

} // namespace tsnl::log::detail

namespace tsnl::log {

inline logger::logger(level l)
: active_(static_cast<int>(l) >= static_cast<int>(logger::min_level_)),
  level_(l) {
    if (active_) {
        stream_ << detail::bold << detail::level_to_color(level_) << detail::level_to_char(level_) << detail::reset
                << ' ';
    }
}

inline logger::~logger() {
    if (active_) {
        auto const locking_enabled_for_this_transaction = locking_enabled_.load();

        if (locking_enabled_for_this_transaction) {
            mutex_.lock();
        }
        for (auto c : stream_.str()) {
            switch (c) {
            case '\0':
            case '\r': {
                // ignore null and carriage return
                break;
            }
            case '\n': {
                // append newline, add indentation for next line
                std::cerr << "\n  ";
                break;
            }
            default: {
                std::cerr << c; // output the character
            }
            }
        }
        std::cerr << "\n";
        if (locking_enabled_for_this_transaction) {
            mutex_.unlock();
        }
    }
}

template <typename T>
inline auto logger::operator<<(T const& value) -> logger& {
    if (active_) {
        stream_ << value;
    }
    return *this;
}

inline auto debug() -> logger {
    return logger(level::debug);
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

inline void set_min_level(level l) {
    logger::min_level_ = l;
}

inline void set_locking_enabled(bool v) {
    logger::locking_enabled_ = v;
}

} // namespace tsnl::log
