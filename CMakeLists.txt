cmake_minimum_required(VERSION 2.6)
project(game)

# for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${game_SOURCE_DIR}/cmake")

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++14 -Wno-unused-parameter")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra -Werror -Wno-missing-field-initializers")

	if(CMAKE_BUILD_TYPE MATCHES "Debug" AND NOT APPLE)
		# disable AddressSanitizer on Apple, https://github.com/alecthomas/entityx/issues/151
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
		if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
			# OR APPLE: in case using custom llvm/clang and on apple
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error")
		else()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
		endif()
	endif()
endif()

SET(SDL2PP_WITH_IMAGE TRUE)
SET(SDL2PP_WITH_TTF TRUE)
ADD_SUBDIRECTORY(vendor/libSDL2pp)

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Threads)

file (GLOB MY_PROJECT_GFX_RESOURCES resources/gfx/*)
file (COPY ${MY_PROJECT_GFX_RESOURCES} DESTINATION resources/gfx)
file (GLOB MY_PROJECT_FONTS_RESOURCES resources/fonts/*)
file (COPY ${MY_PROJECT_FONTS_RESOURCES} DESTINATION resources/fonts)
file (GLOB MY_PROJECT_MANIFESTS_RESOURCES resources/manifests/*)
file (COPY ${MY_PROJECT_MANIFESTS_RESOURCES} DESTINATION resources/manifests)

include_directories("./src")
include_directories("./vendor/spdlog/include")
include_directories("./vendor/json/src")

file (GLOB game_SOURCES src/*.cpp src/systems/*.cpp)
add_executable(game ${game_SOURCES})
include(ExternalProject)

set(entityx_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/external)
set(entityx_LIBRARY ${entityx_INSTALL_DIR}/lib/libentityx-d.a)
include_directories(${entityx_INSTALL_DIR}/include)
if(NOT EXISTS ${entityx_LIBRARY})
  ExternalProject_Add(entityx
    # GIT_REPOSITORY https://github.com/alecthomas/entityx.git
    # GIT_TAG 1.2.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entityx/
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/tmp/entityx
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/external
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DENTITYX_MAX_COMPONENTS=256
      -DENTITYX_BUILD_TESTING=false
  )

  add_custom_target(entityx_target
    cmake --build . --target entityx
    BYPRODUCTS ${entityx_LIBRARY}
    DEPENDS entityx
  )
  add_dependencies(game entityx)
endif()

# include plugin
include(cmake/clang-format.cmake)
include(cmake/clang-tidy.cmake)
if (CLANG_TIDY)  # CMake >= 3.6 runs clang-tidy on build
	set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
endif()

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2PP_INCLUDE_DIRS})

target_link_libraries(game ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2PP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${entityx_LIBRARY})
