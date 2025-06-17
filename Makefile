# Compiler and settings
CXX = g++
CXXFLAGS = -Wall -std=c++17 -DGLEW_STATIC \
	-IDependencies/GLEW/include \
	-IDependencies/GLFW/include \
	-IDependencies/

LDFLAGS = \
	-LDependencies/GLEW/lib/Release/x64 -lglew32s \
	-LDependencies/GLFW/lib-mingw-w64 -lglfw3 \
	-lopengl32 -lgdi32 -luser32 -lkernel32

SRC_DIR = ./RayTracingV2/src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

TARGET = $(BIN_DIR)/raytracer.exe
DLLS = Dependencies/GLFW/lib-mingw-w64/glfw3.dll

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	cp $(DLLS) $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all
