# NTIA/ITS Propagation Library Contribution Guide

Thank you for your interest in contributing to this open source software. On this
page you will get an overview of the contribution workflow from opening an issue,
creating a PR, reviewing,
and merging the PR. This page also includes some information about the project
structures, development workflows, and code styles which are used throughout the
ITS Propagation Library.

If you are instead interested in usage documentation, please refer to the
[Propagation Library Wiki](https://ntia.github.io/propagation-library-wiki).

## Contents

- [Found a Bug?](#found-a-bug)
- [Background for New Contributors](#background-for-new-contributors)
- [Notes on Code Style](#notes-on-code-style)
- [Project Structure and CMake](#project-structure-and-cmake)
- [Documenting Code](#documenting-code)
- [Testing Code](#testing-code)

## Found a Bug?

If you spot a problem with this software,
[search if an issue already exists](https://docs.github.com/en/github/searching-for-information-on-github/searching-on-github/searching-issues-and-pull-requests#search-by-the-title-body-or-comments).
If a related issue doesn't exist, we encourage you to open one (even if you
don't plan to contribute a resolution yourself). Issues may be opened for bugs,
documentation errors, or feature requests.

## Background for new contributors

The workflow we recommend and describe here follows from best and common
practices in the Git and GitHub ecosystems. We aim to leverage this workflow,
especially the elements of code review and approval, to enable open source
development of robust, trustworthy radio propagation software. Here are some
resources to help you get started with open source contributions:

- [Set up Git](https://docs.github.com/en/get-started/getting-started-with-git/set-up-git)
- [GitHub flow](https://docs.github.com/en/get-started/using-github/github-flow)
- [Collaborating with pull requests](https://docs.github.com/en/github/collaborating-with-pull-requests)
- [Basic explanation of Git submodules](https://gist.github.com/gitaarik/8735255)
by [**@gitaarik**](https://github.com/gitaarik)

### Git Branches

Our repositories use the following approach to organize and keep track of branches.
The `main` branch typically represents the most recently released version of the software.
The `dev` branch stages changes before they are merged into `main` and a new release is created.
New features or bug fixes should be developed on individual "feature branches" with descriptive names.
When complete, features branches should merge into `dev`.

### Git Submodules

PropLib C++ repositories make use of Git submodules to reference certain development
dependencies, e.g. GoogleTest. Depending on the CMake preset or options used, submodules
may be required to successfully build and/or test the software. When cloning a repository,
submodules are not additionally cloned by default. Use the following commands to initialize
and clone any submodules in a repository:

```cmd
git submodule init
git submodule update
```

### Contributing on GitHub

If you'd like to solve an existing issue, add a new feature, or modify this software,
follow these steps when making your changes.

1. Fork the repository. This allows you to make your changes without affecting the
original project until you're ready to merge them. You can create a fork
[with GitHub Desktop](https://docs.github.com/en/desktop/contributing-and-collaborating-using-github-desktop/cloning-and-forking-repositories-from-github-desktop)
or [using the command line](https://docs.github.com/en/github/getting-started-with-github/fork-a-repo#fork-an-example-repository)

1. Create a working branch and start with your changes! Commit changes
incrementally to your fork. See the sections below for details about unit tests,
code style, and documentation.

1. When you're done making changes, create a pull request (PR). In your PR, please include
a meaningful description of the changes you've made. If your PR solves an issue,
[link to it](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue)!

Once you submit your PR, a maintainer will review your changes to determine
whether or not they should be merged. We may ask questions or request additional
changes which must be addressed. For example, we may request changes so that the code
meets structure, formatting, accuracy, or testing requirements.

If your PR is approved and merged, your changes will be a part of the `dev`
branch of the repository, where they will stay until a new release is made. At that
point, `dev` will merge into `main` and a new release will be created. The maintainers
of a repository hold the authority on when a new release should be created. For example,
important bug fixes may take higher priority, while small improvements may stay on `dev`
for a while. Rest assured, even if a new release is not immediately made, your approved
changes will be always packaged into the next release.

## Notes on Code Style

- In general, variables follow the naming convention in which a single underscore
denotes a subscript (pseudo-LaTeX format), where a double underscore is followed
by the units, i.e. `h_1__meter`.
- Variables are named to match their corresponding mathematical variables in the
underlying text, when applicable.
- Wherever possible, equation numbers are provided. It is assumed that a user
reviewing this source code would have a copy of the relevant text available
as a primary reference.
- _For base/C++ repositories_, a `.clang-format` file is included in the root directory.
Most IDEs support this type of file, which can and should be used to apply uniform
code styling to C++ source and header files.
- _For Python wrapper repositories_, a `.pre-commit-config.yaml` file is included
in the root directory. This file implements multiple hooks for the [pre-commit](https://pre-commit.com/)
tool, which apply automated formatting to files when they are committed to Git.
It is recommended to use this tool to autoformat Python code when checked in.

## Project Structure and CMake

Software in the ITS Propagation Library is primarily implemented in C++, then
wrapped with interfaces exposing the C++ library to users of other languages. The
primary repository for each software package uses [CMake](https://cmake.org/) to
handle cross-platform C++ build configuration, C++ unit tests (with
[GoogleTest](https://github.com/google/googletest) and
[CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html)), and generation of
API documentation (with [Doxygen](https://www.doxygen.nl/)). Many IDEs support CMake
integration in some form or fashion, and it is recommended that you familiarize yourself
with any such functionality of your chosen IDE.

This section shows a typical project structure for a primary (i.e., non-wrapper)
repository. For details about wrapper repositories, refer to their own README files.

```bash
app/                         # The command-line driver which can run the library
  include/                   # Headers used by the command-line driver
  src/                       # Source code for the command-line driver
  tests/                     # Header and source files for testing the command-line driver
  CMakeLists.txt             # Configuration for the command-line driver and its tests
docs/
  CMakeLists.txt             # Doxygen configuration
  ...                        # Static files (images, HTML, CS, Markdown) used by Doxygen
extern/
  test-data/                 # Git submodule containing test data files shared with wrappers
  ...                        # Other external Git submodules/dependencies
include/
  <HeaderFile>.h             # Library interface header file goes here, e.g. "ITM.h"
src/
  <SourceFiles>.cpp          # Source files go here, e.g. "LongleyRice.cpp" and "FreeSpaceLoss.cpp"
  CMakeLists.txt             # Configures cross-platform build
tests/
  <TestFiles>.cpp            # Unit tests, usually one test file per source file.
  <TestFiles>.h              # Any headers used by tests go here as well.
  CMakeLists.txt             # CTest+GTest config. Files containing tests must be included here.
CMakeLists.txt               # Top-level CMakeLists.txt: project metadata and options
CMakePresets.json            # Presets for CMake, e.g. "release64", "debug32", etc.
...
```

### CMake Options and CMake Presets

As you can see, multiple `CMakeLists.txt` files exist within the project. Each
one contains configurations relevant to the directory where it is stored. For
example, the `tests/CMakeLists.txt` file configures unit tests using CMake. The
top-level `CMakeLists.txt` stores the primary project configuration and includes
the lower-level configurations based on the preset or specified CMake options.

The following CMake options are used for top-level project configuration:

| Option             | Default | Definition                               |
|--------------------|---------|------------------------------------------|
| `BUILD_DOCS`       | `ON`    | Generate documentation site with Doxygen |
| `BUILD_DRIVER`     | `ON`    | Build the command-line driver executable |
| `RUN_DRIVER_TESTS` | `ON`    | Test the command-line driver executable  |
| `DOCS_ONLY`        | `OFF`   | Skip all steps _except_ generating the documentation site |
| `RUN_TESTS`        | `ON`    | Run unit tests for the main library      |

[CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) are
provided to support common build configurations. These are specified in the
`CMakePresets.json` file. The `release` preset will compile the library and driver
with optimizations, build the documentation site, and run all unit tests. The `debug` preset
will skip building the documentation site, driver, and driver tests, which can be useful for
rapid development and testing. Additionally, the Debug configuration will attempt to pass
debug flags to the compiler. Finally, the "docsOnly" preset skips all steps except for
generating the Doxygen documentation site.

| Option             | `release` preset | `debug` preset | `docsOnly` preset |
|--------------------|------------------|----------------|-------------------|
| `DOCS_ONLY`        | `OFF`            | `OFF`          | `ON`              |
| `RUN_TESTS`        | `ON`             | `ON`           | `OFF`             |
| `CMAKE_BUILD_TYPE` | `Release`        | `Debug`        | not set           |
| `BUILD_DOCS`       | `ON`             | `OFF`          | `ON`              |
| `BUILD_DRIVER`     | `ON`             | `OFF`          | `OFF`             |
| `RUN_DRIVER_TESTS` | `ON`             | `OFF`          | `OFF`             |

Below are some examples of how CMake can be called to compile this software.

```bash
# Configure and compile in 64-bit release configuration
cmake --preset release64
cmake --build --preset release64

# Use the 64-bit release configuration but don't build Doxygen docs
cmake --preset release64 -DBUILD_DOCS=OFF
cmake --build --preset release64

# Configure and compile in 32-bit debug configuration
cmake --preset debug32
cmake --build --preset debug32

# Use the 64-bit release configuration but don't run driver tests
cmake --preset release64 -DRUN_DRIVER_TESTS=OFF
cmake --build --preset release64
```

### Supported Platforms and Build Options

The provided `CMakeLists.txt` and `CMakePresets.json` files aim to be flexible
for development from the platform of your choosing. The approach taken is to make
few assumptions about your toolchain to implicitly enable cross-platform and
multi-environment development as much as possible. However, we cannot guarantee
that all compilers, tools, and platforms will work without requiring some additional
configuration which is not documented here. If you find an issue or would like to
see a change to support your chosen platform or tools, open an issue or create a
pull request!

## Documenting Code

### C++ Base Libraries

The C++ source code is documented with Doxygen. A GitHub Action is configured to
build and deploy the documentation using GitHub Pages. This action will ensure
that any new code has been accompanied by Doxygen-formatted documentation. Code
will not be merged until and unless it is completely documented using Doxygen,
and the GitHub action successfully generates the documentation site. Below is an
example showing the expected documentation formats. Except for inline documentation,
use the JavaDoc banner style [described by Doxygen](https://www.doxygen.nl/manual/docblocks.html)

```cpp
constexpr double = PI 3.1415; /**< Inline format, e.g. for constants */

/*******************************************************************************
 * This is a brief description of the function.
 *
 * This is an optional, longer description of the function. It can include
 * LaTeX formatting, for example: this function doubles its input @f$ x @f$ and
 * returns a value @f$ y @f$ with @f$ y = 2x @f$. This whole documentation block
 * is using the JavaDoc banner style!
 *
 * @param[in] x  The input and its expected units
 * @return       The result @f$ y = 2x @f$
 ******************************************************************************/
double doubleTheInput(double x)
{
    return 2 * x;
}
```

### Doxygen for C++ Libraries

The base C++ libraries include Doxygen configurations which generate static
websites from code comments. These documentation sites are published as developer
reference documentation using GitHub Pages. When building the Doxygen site locally,
The site is generated in `docs/html/` and the main page can be accessed at `docs/html/index.html`.
When new releases are made, GitHub Actions workflows are triggered which build and deploy
the Doxygen site to GitHub Pages.

### MATLAB Wrappers

MATLAB® wrappers are implemented as toolboxes which interface with the shared library
compiled from C++ source code. The project structure is informed by the best practices
provided by MathWorks® in their [`toolboxdesign` repository](https://github.com/mathworks/toolboxdesign).
Here is an example of how a function may be documented in a MATLAB wrapper. Note the
documentation with code, where input and output arguments are provided for autocompletion.

```matlab
function y = DoubleTheInput(x)
% DoubleTheInput - produces an output which is twice its input.
%
% Syntax:
%   y = DoubleTheInput(x)
%
% Input Arguments:
%   x   (double) - A number which needs doubling
%
% Output Arguments:
%   y   (double) - The result, 2*x
%
% Description:
%   Functions more complex than this one may warrant an additional,
%   longer description.
arguments (Input)
    x double
end
arguments (Output)
    y double
end
...
```

### Python Wrappers

The Python wrapper code is documented in the [Sphinx](https://sphinx-rtd-tutorial.readthedocs.io/en/latest/docstrings.html)
format. It is recommended to include docstrings for all primary functions, classes,
or structures provided by the Python wrapper. Further, function signatures should
include [type annotation](https://docs.python.org/3/library/typing.html) for inputs
and returned values. Inline or other comments should be included to explain other
variables or functionalities of the code. Below is an example showing the recommended
documentation format.

```python

CONSTANT_EXPOSED_BY_MODULE = 42  # A brief comment could explain what this is

def double_the_input(x: float) -> float:
  """This is a brief description of the function.

  This is an optional, longer description of the function.
  It can span multiple lines.

  :param x: The input value, and its expected units.
  :return: The result y = 2*x
  """
  return 2 * x
```

### .NET Wrappers

PropLib .NET wrappers are written in C# and documentation comments are written in
[XML format](https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/documentation-comments)
and are used to generate documentation through tools like Visual Studio. Use `<summary>` tags to
provide brief descriptions of classes, constants, functions, etc. Functions should
include `<param>` and `<returns>` elements for all inputs and outputs. An example
of this documentation style is shown below.

```csharp
/// <summary>
/// Represents a class that contains constants and methods related to calculations.
/// </summary>
public class CalculationUtils
{
    /// <summary>
    /// A constant value exposed by the module.
    /// </summary>
    public const int CONSTANT_EXPOSED_BY_MODULE = 42;

    /// <summary>
    /// Doubles the input value.
    /// </summary>
    /// <param name="x">The input value to be doubled.</param>
    /// <returns>The doubled value of the input.</returns>
    public double DoubleTheInput(double x)
    {
        // Brief comment explaining what this function does.
        return 2 * x;
    }
}
```

## Testing Code

When modifying or extending this software, ensure that unit tests are added to
cover your new code. In general, each C++ file in `src/` has a corresponding C++
file in `tests/` which implements unit tests. If you've added a new file in `tests/`,
make sure to add that file to the executable in `tests/CMakeLists.txt`.

After compiling the library, you can run unit tests as follows. First, change your
working directory to the `build` directory, then run:

```bash
ctest
```
