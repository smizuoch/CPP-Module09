NAME = PmergeMe

SRCS = main.cpp PmergeMe.cpp

HEADERS = PmergeMe.hpp

OBJS = $(SRCS:.cpp=.o)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

format:
	clang-format -style="{BasedOnStyle: Microsoft, IndentWidth: 4, UseTab: ForIndentation, TabWidth: 4}" -i $(SRCS) $(HEADERS)

re: fclean all

.PHONY: all clean fclean re

# ./PmergeMe `shuf -i 1-1000000 -n 30000 | tr "\n" " "`