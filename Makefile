CXX=c++
CXXFLAGS=-std=c++98
NAME=ircserv
SRC_DIR=src
OBJ_DIR=obj
SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
HEADERS=$(wildcard include/*.hpp)
INCLUDE=-Iinclude

ifndef LENIENT
	CXXFLAGS += -Wall -Wextra -Werror -Wpedantic
endif

ifdef DEBUG
	CXXFLAGS += -g
endif

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $^

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

test: all
	./$(NAME)

re: fclean all

.PHONY:
	all clean fclean re test
