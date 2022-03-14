cmake_minimum_required(VERSION 3.19.0)

project(QtzCore VERSION 0.4.1 
    DESCRIPTION "Qtz Rapid Application Development Framework, Core Module"
    HOMEPAGE_URL "https://github.com/soroush/qtz-core"
    LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Check for dependencies
find_package(Qt5 COMPONENTS Core REQUIRED)

# OS-dependent sources
set(OS_SOURCES )
if(WIN32)
    list(APPEND OS_SOURCES ${PROJECT_NAME}.rc)
endif()

# Define the library
add_library (${PROJECT_NAME} SHARED
    src/settings.cpp
    src/run-guard.cpp
    src/qio.cpp
    src/number-formatter.cpp
    src/jalali-date.cpp
    # Headers
    include/settings.hpp
    include/run-guard.hpp
    include/qtz-core.hpp
    include/qio.hpp
    include/number-formatter.hpp
    include/jalali-date.hpp
    ${OS_SOURCES}
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES 
    VERSION 0.4.1
    SOVERSION 0
    DEBUG_POSTFIX d
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED TRUE
    DEFINE_SYMBOL QTZ_CORE_LIBRARY
    PUBLIC_HEADER "include/qtz-core.hpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link against dependencies
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)

# Copy headers for local builds
if(QTZ_LOCAL_BUILD)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND 
    ${CMAKE_COMMAND} -P 
        "${CMAKE_CURRENT_LIST_DIR}/cmake/copy_local_includes.cmake" 
        "${CMAKE_CURRENT_SOURCE_DIR}/include/" "core" "${QTZ_LOCAL_INCLUDES_DIR}"
)
endif()

# Install and export
include(GNUInstallDirs)

export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_NAME}Targets.cmake")

install(TARGETS ${PROJECT_NAME}
    EXPORT "${PROJECT_NAME}Targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/qtz/core"
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/qtz/core")

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT "${PROJECT_NAME}Targets"
    FILE "${PROJECT_NAME}Targets.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/
)

install(FILES "cmake/${PROJECT_NAME}Config.cmake" 
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/
)

# pkg-config
configure_file("${PROJECT_NAME}.pc.in" "${PROJECT_NAME}.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)