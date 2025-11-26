NAME = scop

SRC_DIR = ./srcs
OBJ_DIR = ./objs

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I includes -I srcs
RM = rm -rf

SRC = main.cpp

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEPS = $(OBJS:.o=.d)

LIBS = -Llib/glfw/build/src -lglfw3 \
        -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MF $(@:.o=.d) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re

