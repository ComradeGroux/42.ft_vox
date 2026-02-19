CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Werror -Iheaders -Ibuild/glfw/include -Ibuild/glad/include/glad -Ibuild/stb -g
LDFLAGS := -lGL -lglfw -ldl -lpthread -lm -lX11

SRC_DIR := srcs
BUILD_DIR := build
OBJS_DIR := $(BUILD_DIR)/objs
DEPS_DIR := deps
DEPS_INSTALLED := $(DEPS_DIR)/.deps_installed

GLFW_BUILD_DIR := $(BUILD_DIR)/glfw
GLFW_LIB := $(GLFW_BUILD_DIR)/src/libglfw3.a

GLM_BUILD_DIR := $(BUILD_DIR)/glm
GLM_LIB := $(GLM_BUILD_DIR)/glm/libglm.a

ASSIMP_BUILD_DIR := $(BUILD_DIR)/assimp
ASSIMP_LIB := $(ASSIMP_BUILD_DIR)/lib/libassimp.a


TARGET := ft_vox

SRCS := $(wildcard $(SRC_DIR)/*.cpp)

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJS_DIR)/%.o)


all: $(TARGET)

$(TARGET): $(DEPS_INSTALLED) $(OBJS) 
	@echo "→ Edition de liens de $(TARGET)"
	$(CXX) $(OBJS) $(GLFW_LIB) $(GLM_LIB) $(ASSIMP_LIB) $(LDFLAGS) -o $@

$(OBJS_DIR)%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJS_DIR)
	@echo "→ Compilation de $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEPS_INSTALLED):
	@echo "→ Installation des dependances..."
	@./install.sh
	@touch $(DEPS_INSTALLED)

clean:
	@echo "→ Nettoyage..."
	rm -rf $(OBJS_DIR)

depsclean:
	@echo "→ Nettoyage complet"
	rm -rf $(DEPS_DIR)
	rm -rf $(BUILD_DIR)

fclean: depsclean
	rm -rf $(TARGET)

re: fclean all

.PHONY: all clean fullclean fclean re install