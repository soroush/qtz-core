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

# Include directories
include_directories("include")

# Define the library
add_library (QtzCore SHARED
    src/settings.cpp
    src/qtz-single-core-application.cpp
    src/qio.cpp
    src/number-formatter.cpp
    src/jalali-date.cpp
    # Headers
    include/settings.hpp
    include/qtz-single-core-application.hpp
    include/qtz-single-core-application_p.hpp
    include/qtz-core.hpp
    include/qio.hpp
    include/number-formatter.hpp
    include/jalali-date.hpp
)

# Set version
set_target_properties(QtzCore PROPERTIES VERSION 0.4.1)
set_target_properties(QtzCore PROPERTIES SOVERSION 0)

# pkg-config
configure_file(QtzCore.pc.in QtzCore.pc @ONLY)

# Define symbol exporter
target_compile_definitions(QtzCore PRIVATE -DQTZ_CORE_LIBRARY)
target_compile_features(QtzCore PRIVATE cxx_std_11)

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

# OS-specific links
if(WIN32)
target_link_libraries(QtzCore "Advapi32.lib")
endif()