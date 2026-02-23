TARGET := ft_vox

CLASS_HEADERS := World Generator Render tmp

SRC_DIR := srcs
BUILD_DIR := build
OBJS_DIR := $(BUILD_DIR)/objs
DEPS_DIR := deps

GLFW_DEP_DIR := $(DEPS_DIR)/glfw
GLFW_BUILD_DIR := $(BUILD_DIR)/glfw
GLFW_LIB := $(GLFW_BUILD_DIR)/src/libglfw3.a

GLM_DEP_DIR := $(DEPS_DIR)/glm
GLM_BUILD_DIR := $(BUILD_DIR)/glm
GLM_LIB := $(GLM_BUILD_DIR)/glm/libglm.a

GLAD_BUILD_DIR := $(BUILD_DIR)/glad
GLAD_OBJ := $(OBJS_DIR)/glad.o

ASSIMP_DEP_DIR := $(DEPS_DIR)/assimp
ASSIMP_BUILD_DIR := $(BUILD_DIR)/assimp
ASSIMP_LIB := $(ASSIMP_BUILD_DIR)/libassimp.a

STB_BUILD_DIR := $(BUILD_DIR)/stb
STB_LIB := $(STB_BUILD_DIR)/stb_image.h


SRCS := $(wildcard $(SRC_DIR)/*.cpp) \
		$(wildcard $(SRC_DIR)/World/*.cpp) \
		$(wildcard $(SRC_DIR)/Generator/*.cpp) \
		$(wildcard $(SRC_DIR)/Render/*.cpp) \
		$(wildcard $(SRC_DIR)/tmp/*.cpp)

VPATH := $(dir $(SRCS))


OBJS := $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.cpp=.o)))

CLASS_DIRS := $(addprefix -I, $(addprefix $(SRC_DIR)/, $(CLASS_HEADERS)))

#########################
### COMPILATION RULES ###
#########################
CXX := g++
LDFLAGS := -lGL -ldl -lpthread -lm -lX11
CXXFLAGS := -std=c++17 -O2 -Wall -Werror \
			-Iheaders \
			-I$(GLFW_DEP_DIR)/include \
			-I$(GLAD_BUILD_DIR)/include/glad \
			-I$(GLM_DEP_DIR)/glm \
			-I$(DEPS_DIR)/assimp/include \
			-I$(STB_BUILD_DIR) \
			$(CLASS_DIRS)
DEBUG_FLAGS := -g -DDEBUG

all: $(TARGET)

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: all

$(TARGET): $(GLFW_LIB) $(GLM_LIB) $(GLAD_BUILD_DIR) $(ASSIMP_LIB) $(STB_LIB) $(OBJS) $(GLAD_OBJ)
	@echo "→ Linking of $(TARGET)"
	$(CXX) $(OBJS) $(GLAD_OBJ) $(GLFW_LIB) $(GLM_LIB) $(ASSIMP_LIB) $(LDFLAGS) -o $@

$(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(OBJS_DIR)
	@echo "→ Compilation of $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GLAD_OBJ): $(GLAD_BUILD_DIR)
	@mkdir -p $(OBJS_DIR)
	@echo "→ Compilation of GLAD"
	$(CC) -I$(GLAD_BUILD_DIR)/include -c $(GLAD_BUILD_DIR)/src/glad.c -o $@


########################
##### DEPENDANCIES #####
########################

$(GLFW_DEP_DIR):
	@echo "→ Download of GLFW"
	@mkdir -p $(DEPS_DIR)
	git clone --depth=1 --branch 3.4 git@github.com:glfw/glfw.git $(DEPS_DIR)/glfw
$(GLFW_LIB): $(GLFW_DEP_DIR)
	@echo "→ Compilation of GLFW"
	@mkdir -p $(GLFW_BUILD_DIR)
	@cd $(GLFW_BUILD_DIR) && cmake -DBUILD_SHARED_LIBS=OFF ../../$(DEPS_DIR)/glfw && make -s

$(GLM_DEP_DIR):
	@echo "→ Download of GLM"
	@mkdir -p $(DEPS_DIR)
	git clone --depth=1 --branch 1.0.3 git@github.com:g-truc/glm.git $(DEPS_DIR)/glm
$(GLM_LIB): $(GLM_DEP_DIR)
	@echo "→ Compilation of GLM"
	@mkdir -p $(GLM_BUILD_DIR)
	@cd $(GLM_BUILD_DIR) && cmake -DGLM_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF ../../$(DEPS_DIR)/glm && make -s

$(GLAD_BUILD_DIR):
	@echo "→ Download of GLAD generator"
	python3 -m venv .venv
	.venv/bin/pip install glad --quiet
	@echo "→ Generating GLAD files"
	.venv/bin/glad --profile core --api gl=4.6 --generator c --out-path $(GLAD_BUILD_DIR)

$(ASSIMP_DEP_DIR):
	@echo "→ Download of ASSIMP"
	@mkdir -p $(DEPS_DIR)
	git clone --depth=1 --branch v6.0.4 git@github.com:assimp/assimp.git $(DEPS_DIR)/assimp
$(ASSIMP_LIB): $(ASSIMP_DEP_DIR)
	@echo "→ Compilation of ASSIMP"
	@mkdir -p $(ASSIMP_BUILD_DIR)
	@cd $(DEPS_DIR)/assimp && cmake CMakeLists.txt -DBUILD_SHARED_LIBS=OFF -DASSIMP_BUILD_TESTS=OFF
	@cd $(ASSIMP_BUILD_DIR) && cmake --build ../../$(DEPS_DIR)/assimp
	cp $(DEPS_DIR)/assimp/lib/libassimp.a $(ASSIMP_BUILD_DIR)

$(STB_LIB):
	@echo "→ Download of STB_IMAGE"
	@mkdir -p $(BUILD_DIR)/stb
	curl -o $(BUILD_DIR)/stb/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h



########################
#### CLEANING RULES ####
########################

clean:
	@echo "→ Cleaning objects..."
	rm -rf $(OBJS_DIR)

fclean:
	@echo "→ Cleaning in depths"
	rm -rf .venv
	rm -rf $(DEPS_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf $(TARGET)

re: fclean all

.PHONY: all clean fclean re