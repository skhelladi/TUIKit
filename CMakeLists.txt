cmake_minimum_required(VERSION 3.15)
project(TUIFramework VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3 -DNDEBUG)
    endif()
endif()

# =============================================================================
# DEPENDENCIES
# =============================================================================

# FTXUI
include(FetchContent)

FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v6.1.9
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# =============================================================================
# PROJECT STRUCTURE
# =============================================================================

# Headers
set(TUIKIT_HEADERS
    include/tuikit.h
    include/tuikit/core/TUIApp.h
    include/tuikit/core/TUIWidget.h
    include/tuikit/core/TUIStyle.h
    include/tuikit/layouts/TUILayout.h
    include/tuikit/layouts/TUIHBoxLayout.h
    include/tuikit/layouts/TUIVBoxLayout.h
    include/tuikit/widgets/TUIButton.h
    include/tuikit/widgets/TUITextField.h
    include/tuikit/widgets/TUILabel.h
    include/tuikit/widgets/TUIMenu.h
    include/tuikit/widgets/TUICheckBox.h
    include/tuikit/widgets/TUIRadioBox.h
    include/tuikit/widgets/TUIComboBox.h
    include/tuikit/widgets/TUISlider.h
    include/tuikit/widgets/TUIResizableSplit.h
    include/tuikit/widgets/TUICollapsible.h
    include/tuikit/widgets/TUITabWidget.h
    include/tuikit/widgets/TUIGroupBox.h
    include/tuikit/widgets/TUIForm.h
    include/tuikit/widgets/TUIStatusBar.h
    include/tuikit/widgets/TUIToolbar.h
    # Add other headers as they are created
)

set(TUIKIT_SOURCES
    src/core/TUIApp.cpp
    src/core/TUIWidget.cpp
    src/core/TUIStyle.cpp
    src/layouts/TUILayout.cpp
    src/layouts/TUIHBoxLayout.cpp
    src/layouts/TUIVBoxLayout.cpp
    src/widgets/TUIButton.cpp
    src/widgets/TUITextField.cpp
    src/widgets/TUILabel.cpp
    src/widgets/TUIMenu.cpp
    src/widgets/TUICheckBox.cpp
    src/widgets/TUIRadioBox.cpp
    src/widgets/TUIComboBox.cpp
    src/widgets/TUISlider.cpp
    src/widgets/TUIResizableSplit.cpp
    src/widgets/TUICollapsible.cpp
    src/widgets/TUITabWidget.cpp
    src/widgets/TUIGroupBox.cpp
    src/widgets/TUIForm.cpp
    src/widgets/TUIStatusBar.cpp
    src/widgets/TUIToolbar.cpp
    src/core/TUIIcons.cpp
    src/core/TUIScreen.cpp
    src/widgets/TUIProgressBar.cpp
    src/widgets/TUIScrollableContainer.cpp
    src/widgets/TUITextArea.cpp
    src/widgets/TUITreeView.cpp
    src/widgets/Notification.cpp
    # Add other source files as they are created
)

set(EXAMPLE_SOURCES
    examples/main.cpp
)

# =============================================================================
# TUI FRAMEWORK LIBRARY
# =============================================================================

add_library(tuikit ${TUIKIT_SOURCES})
target_include_directories(tuikit PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(tuikit PUBLIC ftxui::screen ftxui::dom ftxui::component)

# =============================================================================
# EXAMPLE EXECUTABLE
# =============================================================================

add_executable(example_usage ${EXAMPLE_SOURCES})
target_link_libraries(example_usage PRIVATE tuikit)


# =============================================================================
# INSTALLATION
# =============================================================================

# Installation of headers
install(FILES include/tuikit.h DESTINATION include)
install(DIRECTORY include/tuikit/core/ DESTINATION include/tuikit/core FILES_MATCHING PATTERN "*.h")
install(DIRECTORY include/tuikit/layouts/ DESTINATION include/tuikit/layouts FILES_MATCHING PATTERN "*.h")
install(DIRECTORY include/tuikit/widgets/ DESTINATION include/tuikit/widgets FILES_MATCHING PATTERN "*.h")

# Installation de la librairie
install(TARGETS tuikit
    EXPORT TUIFrameworkTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Configuration pour find_package
install(EXPORT TUIFrameworkTargets
    FILE TUIFrameworkTargets.cmake
    NAMESPACE TUIFramework::
    DESTINATION lib/cmake/TUIFramework
)

# # Config file
# include(CMakePackageConfigHelpers)

# configure_package_config_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TUIFrameworkConfig.cmake.in"
#     "${CMAKE_CURRENT_BINARY_ROOT}/TUIFrameworkConfig.cmake"
#     INSTALL_DESTINATION lib/cmake/TUIFramework
# )

# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_ROOT}/TUIFrameworkConfigVersion.cmake"
#     "${CMAKE_CURRENT_BINARY_ROOT}/TUIFrameworkConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY SameMajorVersion
# )

# install(FILES
#     "${CMAKE_CURRENT_BINARY_ROOT}/TUIFrameworkConfig.cmake"
#     "${CMAKE_CURRENT_BINARY_ROOT}/TUIFrameworkConfigVersion.cmake"
#     DESTINATION lib/cmake/TUIFramework
# )

# =============================================================================
# DOCUMENTATION (Doxygen)
# =============================================================================

# option(BUILD_DOCS "Build documentation" OFF)

# if(BUILD_DOCS)
#     find_package(Doxygen)
#     if(DOXYGEN_FOUND)
#         set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
#         set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_ROOT}/Doxyfile)

#         configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

#         add_custom_target(docs ALL
#             COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_ROOT}
#             COMMENT "Generating API documentation with Doxygen"
#             VERBATIM
#         )
#     else()
#         message(WARNING "Doxygen not found, documentation will not be built")
#     endif()
# endif()

# =============================================================================
# PACKAGING (CPack)
# =============================================================================

set(CPACK_PACKAGE_NAME "TUIKitFramework")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TUIKit - A Modern TUI Framework for C++")
set(CPACK_PACKAGE_VENDOR "Sofiane KHELLADI")
set(CPACK_PACKAGE_CONTACT "sofiane@khelladi.page")

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    "/build/"
    "/\.git/"
    "/\.vscode/"
    "/\.idea/"
    "\.gitignore"
    "\.DS_Store"
)

include(CPack)

# =============================================================================
# HELPER SCRIPTS
# =============================================================================

# Script de build rapide
# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/scripts/build.sh.in"
#     "${CMAKE_CURRENT_BINARY_ROOT}/build.sh"
#     @ONLY
# )

# Script d'installation
# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/scripts/install.sh.in"
#     "${CMAKE_CURRENT_BINARY_ROOT}/install.sh"
#     @ONLY
# )

# =============================================================================
# INFORMATIONS DE BUILD
# =============================================================================

message(STATUS "=================================")
message(STATUS "TUI Framework Configuration")
message(STATUS "=================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Build docs: ${BUILD_DOCS}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "=================================")