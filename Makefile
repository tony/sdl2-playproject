WATCH_FILES= find . -type f -not -path '*/\.*' -and -not -path '*/build/*' | grep -i '.*[.][cc,cpp,c,h,hh]\|CMakeLists.txt$$' 2> /dev/null

entr_warn:
	@echo "----------------------------------------------------------"
	@echo "     ! File watching functionality non-operational !      "
	@echo "                                                          "
	@echo "Install entr(1) to automatically run tasks on file change."
	@echo "See http://entrproject.org/                               "
	@echo "----------------------------------------------------------"

all: build

mkdir_build:
	-test ! -d ./build && mkdir -p build

build: mkdir_build
	cd build; cmake ..
	make -C build

build_ninja: mkdir_build
	cd build; cmake -GNinja ..
	ninja -C build

clean:
	@if [ -d build ]; then \
		echo "Cleaning build directory..."; \
		if [ -f build/build.ninja ]; then \
			ninja -C build clean; \
		elif [ -f build/Makefile ]; then \
			make -C build clean; \
		fi; \
		echo "Removing CMake configuration..."; \
		rm -f build/CMakeCache.txt; \
		rm -rf build/CMakeFiles; \
	else \
		echo "Build directory doesn't exist. Nothing to clean."; \
	fi

debug: mkdir_build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
	make -C build

debug_ninja: mkdir_build
	cd build; cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
	ninja -C build

format:
	clang-format -style=Chromium src/* -i || clang-format38 -style=Chromium src/* -i

tidy:
	clang-tidy -format-style=google -header-filter=. -p build src/*.cpp -fix

cpplint:
	cpplint --filter=-build/include src/*

cppcheck:
	cppcheck --quiet --enable=all --inconclusive --std=c11 --language=c++ src/*

run:
	./build/game

watch_cpplint:
	if command -v entr > /dev/null; then ${WATCH_FILES} | entr -c $(MAKE) cpplint; else $(MAKE) cpplint entr_warn; fi

watch_cppcheck:
	if command -v entr > /dev/null; then ${WATCH_FILES} | entr -c $(MAKE) cppcheck; else $(MAKE) cppcheck entr_warn; fi

watch_debug:
	if command -v entr > /dev/null; then ${WATCH_FILES} | entr -c $(MAKE) debug_ninja; else $(MAKE) debug_ninja entr_warn; fi

watch_run:
	if command -v entr > /dev/null; then ${WATCH_FILES} | entr -c $(MAKE) debug run; else $(MAKE) debug run entr_warn; fi

install_osx_deps:
	brew install cmake ninja sdl2 sdl2_image sdl2_ttf entr

install_ubuntu_deps:
	sudo apt-get update
	sudo apt-get install -y \
		build-essential \
		cmake \
		ninja-build \
		libsdl2-dev \
		libsdl2-image-dev \
		libsdl2-ttf-dev \
		entr

install_conan:
	@echo "Installing Conan package manager..."
	@if ! command -v conan > /dev/null; then \
		pip3 install --upgrade pip; \
		pip3 install conan==2.2.0; \
		conan profile detect; \
		echo "Conan installed successfully. Run 'conan --version' to verify."; \
	else \
		echo "Conan is already installed. Run 'conan --version' to check version."; \
	fi

install_dev_deps:
	sudo apt-get update
	sudo apt-get install -y \
		clang \
		clang-tools \
		clang-format \
		cppcheck \
		iwyu \
		python3-pip
	$(MAKE) install_conan
	@echo "Installing vcpkg..."
	@if [ ! -d "vcpkg" ]; then \
		git clone https://github.com/Microsoft/vcpkg.git; \
		cd vcpkg && ./bootstrap-vcpkg.sh && cd ..; \
	fi
	@echo "Note: sonar-scanner must be installed manually from https://docs.sonarqube.org/latest/analyzing-source-code/scanners/sonarscanner/"
	@echo "Development dependencies installed. You may need to add vcpkg to your PATH."

install_all: install_ubuntu_deps install_dev_deps

.PHONY: mkdir_build clean
