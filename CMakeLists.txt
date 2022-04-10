cmake_minimum_required(VERSION 3.10)
project(BioMapper)

# Set the C++ Standard to C++20
set(CMAKE_CXX_STANDARD 20)
# Set the output directory to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################
# Message coloring convenience variables
# This is just for better looking output.
##########################################
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

# Convenience function to color output
function(DisplayPackage pkg)
    message("[${Yellow}${pkg}${ColourReset}]")
endfunction()


# Set subdirectory cmake options
if(CMAKE_BUILD_TYPE MATCHES Debug)
    DisplayPackage("BioMapper")
    message(STATUS "Building Debug Version of BioMapper")
    # Should pass to the compiler so we can have this defined in the code
    # Allows for conditional code for debug variant
    add_definitions(-DDEBUG_MESSAGES)
else()
    DisplayPackage("BioMapper")
    message(STATUS "Building Release Version of BioMapper")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

# Get a list of all files.  This will eventually be used for compiling in order to
# avoid having to add files by hand to the project.
file(GLOB SOURCE_FILES "src/*.c" "src/*.h" "src/*.cpp" "src/*.hpp")

# TODO: Add in command line parser library
# Get Command Line Parser
#DisplayPackage("CLParser")
#message(STATUS "Adding package...")
#add_subdirectory(CLParser)

# TODO: Add in spdlog library
# Add any cmake supported submodules
#DisplayPackage("spdlog")
#message(STATUS "Adding package...")
#add_subdirectory(spdlog)


# Actually add how to build the BioMapper project
add_executable(BioMapper
        src/main.cpp
        ${SOURCE_FILES}
        )

# Add in subdirectories that files are included from to the project
target_include_directories(BioMapper PRIVATE src)
target_include_directories(BioMapper PRIVATE ${CMAKE_BINARY_DIR})
#target_include_directories(BioMapper PRIVATE CLParser/include)


# Add any libraries being used by the BioMapper project (external, such as the spdlog library)
target_link_libraries(BioMapper PRIVATE stdc++)
#target_link_libraries(BioMapper PRIVATE CLParser)
#TARGET_LINK_LIBRARIES(BioMapper PRIVATE
#        pthread
#        dl
#        )

# Set G++ build info for this project (depending on debug vs release)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(BioMapper PRIVATE -g -pthread -Wno-deprecated-declarations)
else()
    target_compile_options(BioMapper PRIVATE -O3 -pthread -Wno-deprecated-declarations)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

file(COPY test DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#file(COPY conf_file.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

