# Main Page

This website is an information-oriented API reference document for the @libname
C++ library, a part of the NTIA/ITS Propagation Library. This site is primarily
useful for developers wishing to contribute to this library or take it as a dependency.

**For most users, the best place to start is the**
[**NTIA/ITS Propagation Library Wiki**](https://ntia.github.io/propagation-library-wiki).

On the wiki, you'll find installation instructions, usage guides, and code examples
for this and other software within the NTIA/ITS Propagation Library. Further, the
wiki includes instructions for using this library from other software languages,
including Python, MATLAB, and C#/.NET.

## Site Navigation

Please use the navigation menu and search functionality to explore this reference
documentation. The "Files" navigation menu includes the following notable options:

- [File List](files.html) provides a browsable overview of the source code directories.
- [File Members - All](globals.html) lists all documents file members alphabetically.

Additional pages listed under "File Members" allow for browsing based on member types,
e.g. classes, functions, etc.

## Generating this Documentation

This site is generated with [Doxygen](https://www.doxygen.nl/), which is configured
in the source project using [CMake](https://cmake.org/). The documentation is generated
by default when building the project in its debug configuration. Additionally, the
documentation can be generated without compiling the source project by using the
`DOCS_ONLY` CMake option.
