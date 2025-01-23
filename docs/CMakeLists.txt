###########################################
## FIND DOXYGEN AND DOXYGEN-AWESOME-CSS
###########################################
# Doxygen >=1.11.0 is required to properly render the header
set(MINIMUM_DOXYGEN_VERSION "1.11")
find_package(Doxygen ${MINIMUM_DOXYGEN_VERSION} REQUIRED doxygen)
# find_package will cause an error and exit if package is not found

# Ensure doxygen-awesome-css submodule has been initialized
set(EXTRA_STYLESHEET "${PROJECT_SOURCE_DIR}/extern/doxygen-awesome-css/doxygen-awesome.css")
if (NOT EXISTS ${EXTRA_STYLESHEET})
    message(FATAL_ERROR
        "External Doxygen stylesheet is missing! "
        "Run `git submodule init extern/doxygen-awesome-css`, then "
        "`git submodule update` and try again."
    )
endif ()

###########################################
## CONFIGURE DOXYGEN
###########################################
set(DOCS_DIR "${PROJECT_SOURCE_DIR}/docs")
set(DOXYGEN_ALIASES "libname=${LIB_NAME}")  # Used to populate library name on main page
set(DOXYGEN_PROJECT_NAME "${CMAKE_PROJECT_NAME}")
set(DOXYGEN_BUILTIN_STL_SUPPORT "YES")
set(DOXYGEN_DISABLE_INDEX "NO")
set(DOXYGEN_EXCLUDE "${PROJECT_SOURCE_DIR}/tests/*")
set(DOXYGEN_FULL_SIDEBAR "NO")
set(DOXYGEN_GENERATE_LATEX "NO")
set(DOXYGEN_GENERATE_TREEVIEW "NO")
set(DOXYGEN_HTML_COLORSTYLE "LIGHT")  # Required for doxygen-awesome-css
set(DOXYGEN_HTML_EXTRA_FILES
    "${DOCS_DIR}/images/ITSlogoOnly400.png"
    "${DOCS_DIR}/images/favicon-16x16.png"
    "${DOCS_DIR}/images/favicon-32x32.png"
    "${DOCS_DIR}/images/apple-touch-icon.png")
set(DOXYGEN_HTML_EXTRA_STYLESHEET "${EXTRA_STYLESHEET}" "${DOCS_DIR}/doxy_custom.css")
set(DOXYGEN_HTML_FOOTER "${DOCS_DIR}/doxy_footer.html")
set(DOXYGEN_HTML_HEADER "${DOCS_DIR}/doxy_header.html")
set(DOXYGEN_JAVADOC_AUTOBRIEF "YES")
set(DOXYGEN_JAVADOC_BANNER "YES")
set(DOXYGEN_OUTPUT_DIRECTORY "${DOCS_DIR}")
set(DOXYGEN_PREDEFINED "DOXYGEN_SHOULD_SKIP")
set(DOXYGEN_PROJECT_BRIEF "Part of the NTIA/ITS Propagation Library")
set(DOXYGEN_PROJECT_LOGO "${DOCS_DIR}/images/ntia-logo-400px.png")
set(DOXYGEN_REPEAT_BRIEF "YES")
set(DOXYGEN_SHOW_INCLUDE_FILES "NO")
set(DOXYGEN_USE_MATHJAX "YES")
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${DOCS_DIR}/doxy_mainpage.md")    
set(DOXYGEN_WARN_AS_ERROR "YES")
set(DOXYGEN_WARN_IF_UNDOC_ENUM_VAL "YES")
set(DOXYGEN_WARN_NO_PARAMDOC "YES")

# Doxygen docs are a developer, not user, reference.
# Therefore, document private and internal code
set(DOXYGEN_EXTRACT_PRIVATE "YES")
set(DOXYGEN_INTERNAL_DOCS "YES")

doxygen_add_docs(
    "${LIB_NAME}Docs"
    "${PROJECT_SOURCE_DIR}/app/src"
    "${PROJECT_SOURCE_DIR}/app/include"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/include"
    "${DOCS_DIR}/doxy_mainpage.md"
    ALL
    COMMENT "Generate HTML documentation with Doxygen"
)