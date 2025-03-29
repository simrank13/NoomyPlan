cmake_minimum_required(VERSION 3.16)
project(NoomyPlan
  VERSION 0.1
  DESCRIPTION "Budget Management Application"
  LANGUAGES CXX
)
# Use absolute path but construct it dynamically 
set(OUTPUT_BASE_DIR "${CMAKE_SOURCE_DIR}/../../../work/work/test")
file(MAKE_DIRECTORY ${OUTPUT_BASE_DIR})
message("Output directory: ${OUTPUT_BASE_DIR}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH "C:/Qt_msvc/6.8.2/msvc2022_64")
# Override the default build output locations
set(CMAKE_PDB_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR})
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR})
# Find Qt components
find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)
set(SOURCES
    main.cpp
    user.cpp
    authenticatesystem.cpp
    loginpage.cpp
    financialreport.cpp
    cashflowtracking.cpp
    dashboard.cpp
    budgetpage.cpp
    budgetpagebudget.cpp
    budgetpageexpenses.cpp
    inventory.cpp
)
set(HEADERS
    user.h
    authenticatesystem.h
    loginpage.h
    financialreport.h
    cashflowtracking.h
    dashboard.h
    budgetpage.h
    budgetpagebudget.h
    budgetpageexpenses.h
    inventory.h
    mainwindow.h
)
add_executable(NoomyPlan ${SOURCES} ${HEADERS})
target_link_libraries(NoomyPlan PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
    Qt6::Charts
)
# Add MSVC-specific flags to enable exception handling
if(MSVC)
    target_compile_options(NoomyPlan PRIVATE /EHsc)
endif()
# Explicitly set output directory for the target, overriding everything
set_target_properties(NoomyPlan PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_BASE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_BASE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${OUTPUT_BASE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${OUTPUT_BASE_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_BASE_DIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_BASE_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_BASE_DIR}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_BASE_DIR}
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
# This is critical: Make sure the Qt DLLs are copied to the build directory
if(WIN32)
    # Get the path to the Qt binaries
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    
    # Add a custom command to run windeployqt after the build
    add_custom_command(TARGET NoomyPlan POST_BUILD
        COMMAND "${QT_BIN_DIR}/windeployqt.exe" 
                --release
                --no-compiler-runtime
                --no-translations
                --verbose 0
                "$<TARGET_FILE:NoomyPlan>"
        WORKING_DIRECTORY ${OUTPUT_BASE_DIR}
        COMMENT "Running windeployqt on NoomyPlan..."
    )
endif()