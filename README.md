# `tsnl/log`

A single-header C++20 logging library. See [`include/tsnl/log.hpp`](include/tsnl/log.hpp).

```cpp
#include <tsnl/log.hpp>

auto main() -> int {
    tsnl::log::set_min_level(tsnl::log::level::debug);  // default min level is 'info'
    tsnl::log::debug() << "This is a debug message.";
    tsnl::log::info() << "This is an info message.";
    tsnl::log::warning() << "This is a warning message.";
    tsnl::log::error() << "This is an error message.";
    return 0;
}
```

## Getting Started

### Bazel

```starlark
# in MODULE.bazel

bazel_dep(name = "tsnl.log")
git_override(
    module_name = "tsnl.log",
    remote = "https://github.com/tsnl/log",
    branch = "main",
    # commit = "..."	# -> you can set a fixed commit if you'd like.
)
```

```starlark
# in BUILD.bazel

cc_library(
    name = "my_awesome_cpp_library",
    deps = [
        # ...
        "@tsnl.log"
    ]
)
```

### Other build systems

You can copy the contents of `/include` into your project.
You only need to ensure that `include/` is in the include path.
