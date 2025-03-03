cmake_minimum_required(VERSION 3.12)
project(hebi_human_human)

SET (CMAKE_CXX_STANDARD 20)
SET (CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fcoroutines")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
get_filename_component(ROOT_DIR "${ROOT_DIR}" REALPATH)

# `HEBI_CPP_DIR` can be used to point to an already downloaded C++ API.
# This is useful for environments that would prefer not to download files
# from the internet (or even don't have internet access).
if(NOT HEBI_CPP_DIR)
  set(HEBI_DIR ${ROOT_DIR}/hebi-cpp)
  include(DownloadHebiCpp.cmake)
else()
  set(HEBI_DIR ${HEBI_CPP_DIR})
endif()

set(hebi_cpp_build_dir hebi-cpp-${CMAKE_BUILD_TYPE})
string(TOLOWER ${hebi_cpp_build_dir} hebi_cpp_build_dir)
get_filename_component(hebi_cpp_build_dir "${CMAKE_CURRENT_BINARY_DIR}/${hebi_cpp_build_dir}" REALPATH)

add_subdirectory(${HEBI_DIR} ${hebi_cpp_build_dir})

find_package(Python 3.6 COMPONENTS Interpreter Development.Module Development.Embed NumPy)

# Due to an issue in Windows and mac with some versions of numpy/distutils/etc, need
# to manually add this here:
set(NUMPY_INCLUDE_DIR ${Python_SITELIB}/numpy/core/include)

# Make one metatarget for all examples
add_custom_target(examples)

# Build X_SERIES Examples
SET(X_SERIES_SOURCES
  ${ROOT_DIR}/src/main.cpp)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY exps)
foreach (EXAMPLE ${X_SERIES_SOURCES})

  # The target for the individual example is based on the filename
  get_filename_component(EX_NAME ${EXAMPLE} NAME_WE)
  
  if(WIN32)
    add_executable(${EX_NAME} ${EXAMPLE} $<TARGET_OBJECTS:_hebic++-obj>)
  else()
    add_executable(${EX_NAME} ${EXAMPLE})

  # Create logs directory in case the example logs data.
  add_custom_command(TARGET ${EX_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E make_directory
                         $<TARGET_FILE_DIR:${EX_NAME}>/logs)

  endif()
  add_dependencies(examples ${EX_NAME})
  target_include_directories(${EX_NAME} PRIVATE ${ROOT_DIR})

  target_include_directories(${EX_NAME} PRIVATE ${Python_INCLUDE_DIRS} ${NUMPY_INCLUDE_DIR})
  target_link_libraries(${EX_NAME} ${Python_LIBRARIES})
  if(WIN32)
    target_link_libraries(${EX_NAME} hebi kernel32)
    target_include_directories(${EX_NAME} PRIVATE ${HEBI_DIR}/src ${HEBI_DIR}/include ${HEBI_DIR}/Eigen)
    # For Windows, we copy the .dll file into the binary directory so that we
    # don't have to set the PATH variable.

    set(LIBHEBI_LOCATION "lib/win_${LIBHEBI_TARGET_ARCHITECTURE}")
    set(HEBI_CPP_LIB_DIRECTORY ${HEBI_DIR}/hebi/${LIBHEBI_LOCATION}/)

    add_custom_command(TARGET ${EX_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HEBI_CPP_LIB_DIRECTORY}/hebi.dll"
      $<TARGET_FILE_DIR:${EX_NAME}>)
  elseif(UNIX)
    target_link_libraries(${EX_NAME} hebi hebic++ m pthread)
  endif()

endforeach (EXAMPLE ${X_SERIES_SOURCES})












