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

### Git Submodules

Software in the ITS Propagation Library is implemented primarily in C++. Each
piece of software has a primary repository which contains the base C++ implementation,
test data and resources, and common files used by the multi-language wrappers.
Interfaces for additional programming languages are provided in separate repositories,
which are linked to the primary repository as [Git submodules](https://gist.github.com/gitaarik/8735255).
When cloning the primary repository, the submodules are not additionally cloned
by default. This can be done with the `git submodule init` command. Initializing
the submodule as part of the parent repository will let you use the build
configuration from the primary repository to compile the C++ source and place it
appropriately for use by the wrapper code. If you choose to independently clone
the wrapper repository, you will likely need to separately download the compiled
library (for example, a DLL from a GitHub release).

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

1. When you're done making changes, create a pull request, also known as a PR.
In your PR, please include a meaningful description of the changes you've made.
If your PR solves an issue,
[link to it](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue)!
Once you submit your PR, a maintainer will review your changes. We may ask questions
or request additional changes which must be addressed before the PR can be merged.

When your PR is approved and merged, your changes will be a part of the main
branch of the repository. A new release may or may not be immediately created,
depending on the changes made. If a new release is not immediately made, your
changes will be packaged into the next release.

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
tool, which apply automated formatting to files when they are commited to Git.
It is recommended to use this tool to autoformat Python code when checked in.

## Project Structure and CMake

Software in the ITS Propagation Library is primarily implemented in C++, then
wrapped with interfaces exposing the C++ library to users of other languages. The
primary repository for each software package uses [CMake](https://cmake.org/) to
handle cross-platform C++ build configuration, C++ unit tests, and generation of
API documentation (with [Doxygen](https://www.doxygen.nl/)). Many IDEs support CMake
integration in some form or fashion, and it is recommended that you familiarize yourself
with any such functionality of your chosen IDE.

This section shows a typical project structure for a primary (i.e., non-wrapper)
repository. For details about wrapper repositories, refer to their own README files.

```bash
docs/                        
  CMakeLists.txt             # Doxygen configuration
extern/
  ...                        # External Git submodules/dependencies
include/                     
  <PackageNamespace>/        # Include namespace folder, e.g. "ITS.Propagation.ITM"
    <HeaderFiles>.h          # Library header files go here, e.g. "ITM.h" and "ErrorCodes.h"
src/
  <SourceFiles>.cpp          # Source files go here, e.g. "LongleyRice.cpp" and "FreeSpaceLoss.cpp"
  CMakeLists.txt             # Configures cross-platform build
tests/
  data/
    <TestDataFiles>.csv      # Testing data goes here. Does not have to be CSV.
  <TestFiles>.cpp            # Unit tests, usually one test file per source file.
  <TestFiles>.h              # Any headers used by tests go here as well.
  CMakeLists.txt             # CTest+GTest config. Must add names of test files here.
wrap/
  dotnet/                    # C#/.NET wrapper submodule. Should contain CMakeLists.txt
  matlab/                    # MATLAB wrapper submodule. Should contain CMakeLists.txt
  python/                    # Python wrapper submodule. Should contain CMakeLists.txt
CMakeLists.txt               # Top-level CMakeLists.txt: project metadata and options
CMakePresets.json            # Presets for CMake, e.g. "release64", "debug64", etc.
...
```

As you can see, multiple `CMakeLists.txt` files exist within the project. Each
one contains configurations relevant to the directory where it is stored. For
example, the `tests/CMakeLists.txt` file configures unit tests using CMake.

When modifying or extending this software, ensure that unit tests are added to
cover your new code. In general, each C++ file in `src/` has a corresponding C++
file in `tests/` which implements unit tests. If you've added a new file in `tests/`,
make sure to add that file to the executable in `tests/CMakeLists.txt`.

To compile the software, from the cloned repository, run:

```bash
cmake -S . -B build
cmake --build build
```

After compiling the library, you can run unit tests as follows. First, change your
working directory to the `build` directory, then run:

```bash
ctest
```

The included `CMakePresets.json` provides presets for common CMake configurations.
The "Release" configurations will compile the software with optimizations, build
documentation, and configure unit tests. The "Debug" configurations will skip building
the documentation, which is useful for rapid development and testing. Additionally,
the Debug configuration will attempt to pass debug flags to the compiler.

### Supported Platforms and Build Options

The provided `CMakeLists.txt` and `CMakePresets.json` files aim to be flexible
for development from the platform of your choosing. The approach taken is to make
few assumptions about your toolchain to implicitly enable cross-platform and
multi-environment development as much as possible. However, we cannnot guarantee
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
and the GitHub action succesfully generates the documentation site. Below is an
example showing the expected documentation formats.

```cpp
#define PI 3.1415 /**< Inline doxygen format, e.g. for macros or struct members */

/*******************************************************************************
 * This is a brief description of the function.
 * 
 * This is an optional, longer description of the function. It can include
 * LaTeX formatting, for example: this function doubles its input @f$ x @f$ and
 * returns a value @f$ y @f$ with @f$ y = 2x @f$.
 *
 * @param[in] x  The input and its expected units
 * @return       The result @f$ y = 2x @f$
 ******************************************************************************/
double doubleTheInput(double x)
{
    return 2 * x;
}
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

### C#/.NET Wrappers

In C#/.NET, documentation comments are written in XML format and are used to
generate documentation through tools like Visual Studio. Use `<summary>` tags to
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
