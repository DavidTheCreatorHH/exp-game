TARGET = exp

# Default target to build the project
all:
	mkdir -p build
	cd build && cmake -Wno-dev .. && make && mv $(TARGET) ../

# Target to clean up build directory and generated files
clean:
	rm -rf build $(TARGET)
