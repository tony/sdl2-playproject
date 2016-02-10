cmake_minimum_required(VERSION 2.6)
project(game)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${game_SOURCE_DIR}/cmake")
set(BIN_DIR ${game_SOURCE_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})


file (GLOB MY_PROJECT_GFX_RESOURCES resources/*.png)
file (COPY ${MY_PROJECT_GFX_RESOURCES} DESTINATION resources)
file (GLOB MY_PROJECT_GFX_ELLIOT_RESOURCES resources/elliot/*.png)
file (COPY ${MY_PROJECT_GFX_ELLIOT_RESOURCES} DESTINATION resources/elliot)
file (GLOB MY_PROJECT_FONT_RESOURCES resources/fonts/*)
file (COPY ${MY_PROJECT_FONT_RESOURCES} DESTINATION resources/fonts)

include_directories("./src")
file (GLOB game_SOURCES src/*.cpp)

find_program(CLANG_FORMAT NAMES clang-format
	clang-format39 clang-format-3.9
	clang-format38 clang-format-3.8
	clang-format37 clang-format-3.7
	clang-format36 clang-format-3.6
	clang-format35 clang-format-3.5
	clang-format34 clang-format-3.4
)

if (CLANG_FORMAT)
	message("ClangFormat found: ${CLANG_FORMAT}")
	add_custom_target(
		format
		COMMAND ${CLANG_FORMAT}
		-style=Chromium
		-i
		${game_SOURCES}
	)
endif()

add_executable(game ${game_SOURCES})
target_link_libraries(game ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})
install(TARGETS game RUNTIME DESTINATION ${BIN_DIR})
