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

# Include directories
include_directories("include")

# OS-dependent sources
set(OS_SOURCES )
if(WIN32)
    list(APPEND OS_SOURCES QtzCore.rc)
endif()

# Define the library
add_library (QtzCore SHARED
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
set_target_properties(QtzCore PROPERTIES 
    VERSION 0.4.1
    SOVERSION 0
    DEBUG_POSTFIX d
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED TRUE
    DEFINE_SYMBOL QTZ_CORE_LIBRARY
)

# pkg-config
configure_file(QtzCore.pc.in QtzCore.pc @ONLY)

# Copy headers for local builds
if(QTZ_LOCAL_BUILD)
add_custom_command(TARGET QtzCore POST_BUILD COMMAND 
    ${CMAKE_COMMAND} -P 
        "${CMAKE_CURRENT_LIST_DIR}/../cmake/copy_local_includes.cmake" 
        "${CMAKE_CURRENT_SOURCE_DIR}/include/" "core" "${QTZ_LOCAL_INCLUDES_DIR}")
        file(COPY ${header_files} DESTINATION "${QTZ_LOCAL_INCLUDES_DIR}/qtz/${component}/" FOLLOW_SYMLINK_CHAIN)
endif()

# Install and export
include(GNUInstallDirs)
target_include_directories(QtzCore PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>  
    $<INSTALL_INTERFACE:include/>
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qtz/core)

install(TARGETS QtzCore
    EXPORT QtzCoreConfig
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

export(TARGETS QtzCore FILE QtzCoreConfig.cmake)

install(EXPORT QtzCoreConfig
    FILE QtzCoreConfig.cmake
    NAMESPACE Qtz::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QtzCore.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# Link againsg dependencies
target_link_libraries(QtzCore Qt5::Core)
