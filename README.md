# Space Shooter Game

This is my first game written in C++ with SDL2 + CMake.

Branches exist where I experimented with entity patterns.

## Development Setup

### Prerequisites

- Ubuntu 24.04 (WSL recommended)
- Git
- Python 3.x
- CMake 3.16 or higher

### Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/sdl2-playproject.git
cd sdl2-playproject
```

2. Install all dependencies:
```bash
make install_all
```

This will install:
- Basic system dependencies (SDL2, build tools)
- Development tools (clang, clang-format, cppcheck, etc.)
- Package managers (Conan, vcpkg)

Alternatively, you can install dependencies separately:
```bash
# Install only system dependencies
make install_ubuntu_deps

# Install only development tools
make install_dev_deps
```

### Development Tools

The project includes several development tools for code quality and analysis:

- **Code Formatting**: `clang-format`
- **Static Analysis**: `cppcheck`, `clang-tidy`
- **Include Analysis**: `include-what-you-use`
- **Package Management**: 
  - Conan (v2.2.0)
  - vcpkg

### Building

```bash
# Standard build
make build

# Debug build with compile_commands.json
make debug

# Clean build
make clean
```

### Development Workflow

- `make format` - Format code using clang-format
- `make tidy` - Run clang-tidy
- `make cppcheck` - Run static analysis
- `make cpplint` - Run Google C++ style guide checks

### File Watching

The project includes file watching capabilities using `entr`:
- `make watch_debug` - Watch and rebuild on file changes
- `make watch_run` - Watch, rebuild, and run on file changes
- `make watch_cppcheck` - Watch and run static analysis on file changes

### Running

```bash
make run
```

## Notes

- SonarScanner is not included in the automatic installation. Install it manually from [SonarQube's website](https://docs.sonarqube.org/latest/analyzing-source-code/scanners/sonarscanner/)
- You may need to add vcpkg to your PATH after installation
- The project uses C++23 features, ensure your compiler supports it
