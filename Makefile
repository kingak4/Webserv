NAME = server

CXX = c++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98

SRCDIR = src
OBJDIR = obj

SOURCES = http/Parser.cpp config/ConfigParser.cpp core/EpollManager.cpp core/Server.cpp core/Client.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Removing object files...$(RESET)"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

