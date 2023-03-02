cmake_minimum_required(VERSION 3.20.0)
# give a name to the project
project(ProjectRoot)

#compile_options(-O3)   # specify some compilation flags


# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/dependencies/cmake_modules)
  

# specify the system
message(VERBOSE "CMAKE_VERSION           : " ${CMAKE_VERSION})
message(VERBOSE "CMAKE_SYSTEM_NAME       : " ${CMAKE_SYSTEM_NAME})
message(VERBOSE "CMAKE_SYSTEM            : " ${CMAKE_SYSTEM})
message(VERBOSE "CMAKE_SYSTEM_PROCESSOR  : " ${CMAKE_SYSTEM_PROCESSOR} "\n")

# specify the directories
message(VERBOSE "CMAKE_BINARY_DIR        : " ${CMAKE_BINARY_DIR})
message(VERBOSE "CMAKE_SOURCE_DIR        : " ${CMAKE_SOURCE_DIR})
message(VERBOSE "CMAKE_HOME_DIRECTORY    : " ${CMAKE_HOME_DIRECTORY})
message(VERBOSE "PROJECT_BINARY_DIR      : " ${PROJECT_BINARY_DIR})
message(VERBOSE "PROJECT_SOURCE_DIR      : " ${PROJECT_SOURCE_DIR} "\n")

# compile flags
message(VERBOSE "CMAKE_CXX_FLAGS         : " ${CMAKE_CXX_FLAG})
message(VERBOSE "CMAKE_CXX_FLAGS_DEBUG   : " ${CMAKE_CXX_FLAGS_DEBUG})
message(VERBOSE "CMAKE_CXX_FLAGS_RELEASE : " ${CMAKE_CXX_FLAGS_RELEASE})
message(VERBOSE "CMAKE_CXX_COMPILER      : " ${CMAKE_CXX_COMPILER} "\n")


# (optional) debug or release
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

# (optional) put the binary files in this directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)



get_filename_component(PROJ_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
message("Creating target '${PROJ_NAME}'")
project(${PROJ_NAME} LANGUAGES C CXX)


file(GLOB_RECURSE src_files_list src/*.cpp)

# for each sample file, make an exe
foreach(src_file ${src_files_list})

  get_filename_component(file_exe ${src_file} NAME_WE)    # define te name of the app (filename Without Extension)
  set(file_exe TEST-${file_exe})
  add_executable(${file_exe} ${src_file})                 # lib dependency


  target_compile_features(${file_exe} PRIVATE cxx_std_20) # use at least c++ 11
  target_compile_options(${file_exe} PRIVATE -Wall -Werror -Wextra -pedantic -O3)   # specify some compilation flags

  message(STATUS "src file  " ${src_file})
  message(STATUS "exe file  " ${file_exe})

endforeach()

