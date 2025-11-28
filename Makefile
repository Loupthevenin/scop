NAME = scop

SRC_DIR = srcs
OBJ_DIR = objs

LIBFT = libft/libft.a

# GLFW (Homebrew)
GLFW_INC = /opt/homebrew/include
GLFW_LIB = -L /opt/homebrew/lib -lglfw

# GLAD
GLAD_DIR = glad
GLAD_INC = $(GLAD_DIR)/include
GLAD_SRC = $(GLAD_DIR)/src/glad.c

CXX = c++
CC = cc
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
CFLAGS= -Wall -Wextra -Werror

FRAMEWORKS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

INCLUDES = -I includes -I $(GLFW_INC) -I $(GLAD_INC)

SRC = main.cpp mesh.cpp
SRC_C = $(GLAD_SRC)

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o)) \
       $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_C:.c=.o)))

DEPS = $(OBJS:.o=.d)

LIBS = $(LIBFT) $(GLFW_LIB) $(FRAMEWORKS)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MF $(@:.o=.d) -c $< -o $@

$(OBJ_DIR)/%.o: $(GLAD_DIR)/src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MF $(@:.o=.d) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
