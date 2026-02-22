make:
		cmake -S . -B build
		cmake --build build

run: all
		./build/qbdbg

clean:
		rm -rf build
