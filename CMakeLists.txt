cmake_minimum_required(VERSION 3.1.0)

project(QtzCore VERSION 0.4.1 
    DESCRIPTION "Qtz Rapid Application Development Framework, Core Module"
    HOMEPAGE_URL "https://github.com/soroush/qtz-core"
    LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

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
    PROPERTIES VERSION 0.4.1
    PROPERTIES SOVERSION 0
    CXX_STANDARD 11
    DEBUG_POSTFIX "d"
    DEFINE_SYMBOL "QTZ_CORE_LIBRARY"
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

# Install and export
include(GNUInstallDirs)

export(TARGETS ${PROJECT_NAME} FILE "${PROJECT_NAME}Targets.cmake")

install(TARGETS ${PROJECT_NAME}
    EXPORT "${PROJECT_NAME}Targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY "include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qtz/core)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT "${PROJECT_NAME}Targets"
    FILE "${PROJECT_NAME}Targets.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/
    NAMESPACE Qtz::
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