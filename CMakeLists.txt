cmake_minimum_required(VERSION 3.17.2)

project(FishTracker)
set(PROJECT_NAME "FishTracker")
set(PROJECT_VERSION 1.0)

# On Macs, disable MACOSX_RPATH (cmake 3.x)
if(APPLE)
set(CMAKE_MACOSX_RPATH OFF)
set(CMAKE_CXX_STANDARD 11)
endif()

#Testing
enable_testing()
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Resources path
set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources")
# Images path
set(IMAGES_PATH "${RESOURCES_PATH}/Images")
# Videos path
set(VIDEOS_PATH "${RESOURCES_PATH}/Videos")

# Supported architectures
set(SUPPORTED_ARCHITECTURES x64)

# Check if architecture is supported
if(NOT ";${SUPPORTED_ARCHITECTURES};" MATCHES ${ARCHITECTURE})
  message(FATAL_ERROR "Architecture is not supported: " ${ARCHITECTURE})
endif()

# Set Product Name string
set(PRODUCT_NAME ${PROJECT_NAME} CACHE INTERNAL "Product name")

# Compiler-specific flags
# Visual Studio
if(MSVC)
  # Do not treat warnings as errors
  add_definitions("/W4")
  # Generate PDBs in Release build
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

# Clear lists
set(FT_LIBS "" CACHE INTERNAL "libraries")
set(FT_SOURCE_FILES "" CACHE INTERNAL "source files")

##########
# OpenGL #
##########
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  message(STATUS "OpenGL found.")
endif()

##########
# OpenMP #
##########
if(APPLE)
  #from https://iscinumpy.gitlab.io/post/omp-on-high-sierra/
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C "${CMAKE_C_COMPILER}")
        set(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
        set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
        set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
      set(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
      set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")
      set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
      set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
      set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    endif()
endif()

#find_package(OpenMP REQUIRED)

if (OPENMP_FOUND)
	message(STATUS "OpenMP found.")

    if(APPLE)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
    if(WIN32)
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
	    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
    endif()
endif()

########
# Qt 5 #
########
find_package(Qt5 5.14 REQUIRED COMPONENTS Core Widgets Quick OpenGL 3DCore PrintSupport Gui Xml Network Multimedia MultimediaWidgets Test)

# Save Qt paths
get_target_property(QtCore_LOCATION Qt5::Core LOCATION)
get_filename_component(QT_LIB_DIR ${QtCore_LOCATION} PATH)
set(QT_LIB_DIR ${QT_LIB_DIR} CACHE INTERNAL "Qt library dir")
set(QT_BIN_DIR ${QT_LIB_DIR}/../bin CACHE INTERNAL "Qt binary dir")
set(QT_SRC_DIR ${QT_LIB_DIR}/../../Src CACHE INTERNAL "Qt source dir")
set(QT_PLUGINS_DIR ${QT_LIB_DIR}/../plugins CACHE INTERNAL "Qt plugins dir")
message(STATUS "Qt library directory: " ${QT_LIB_DIR})
message(STATUS "Qt binary directory: " ${QT_BIN_DIR})
message(STATUS "Qt source directory: " ${QT_SRC_DIR})
message(STATUS "Qt plugins directory: " ${QT_PLUGINS_DIR})

##########
# OpenCV #
##########
set(OpenCV_STATIC OFF)
find_package(OpenCV 4.3.0)
include_directories(${OpenCV_INCLUDE_DIRS})

######################
# Application
######################
add_subdirectory(sources/application)
add_subdirectory(sources/libraries/imageprocessing)
add_subdirectory(sources/libraries/videoplayer)
add_subdirectory(sources/tests)

# Add platform-specific files
if (WIN32)
  add_subdirectory(platform/windows)
endif()
if (APPLE)
  add_subdirectory(platform/osx)
endif()

# Set startup project
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

# Target to run CMake
add_custom_target(CMAKE
  COMMAND ${CMAKE_COMMAND}
  ARGS
  .
)
