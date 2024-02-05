# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/01/16 15:30:52 by wkonings      #+#    #+#                  #
#    Updated: 2024/02/05 14:59:07 by shoddy        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# ----------------------------------------- #
# --------------- VARIABLES --------------- #
# ----------------------------------------- #

NAME 	:= irc
FLAGS 	= -Wall -Wextra 
# FLAGS	+= -Werror
DEBUG 	:= -g -fsanitize=address
CC		:= c++

# ---------------------------------------- #
# --------------- DIRECTORIES ------------ #
# ---------------------------------------- #

#src and its subdirectories
SRC_DIR		:= src
S_CMD		:= commands

HDR_DIR		:= include
OBJ_DIR		:= obj

# ----------------------------------------- #
# --------------- FILES ------------------- #
# ----------------------------------------- #

HEADER_FILES := User Channel Server Command

MAIN_FILES	:= main User Channel Server Command parse
CMD_FILES := nick mode pass ping privmsg user join kick topic quit invite part

FILES	:=	$(MAIN_FILES:%=%.cpp) \
			$(addprefix $(S_CMD)/, $(CMD_FILES:%=%.cpp))

SRCS	:=	$(addprefix $(SRC_DIR)/, $(FILES))
# SRCS	:=	$(FILES)
OBJS	:=	$(addprefix $(OBJ_DIR)/, $(FILES:%.cpp=%.o))
# HEADERS	:=	$(HEADER_FILES:%=%.hpp)
HEADERS :=	$(addprefix $(HDR_DIR)/, $(HEADER_FILES:%=%.hpp))

# ----------------------------------------- #
# --------- COLOURS / FORMATTING ---------- #
# ----------------------------------------- #

#Formatting
RESET	:= \1\33[0m\2
END		:= \1\33[0m\2\3

#Colours
YELLOW	:= \1\33[38;5;220m\2
BLACK	:= \1\33[38;5;232m\2
ORANGE	:= \1\33[38;5;202m\2
RED		:= \1\33[38;5;196m\2
PINK	:= \1\33[38;5;198m\2
GREEN	:= \1\33[38;5;28m\2
BLUE	:= \1\33[38;5;33m\2
L_BLUE	:= \1\33[38;5;69m\2
D_BLUE	:= \1\33[38;5;21m\2
GREY	:= \1\33[38;5;242m\2
PURPLE	:= \1\33[38;5;92m\2
VIOLET	:= \1\33[38;5;183m\2

# ----------------------------------------- #
# --------------- RECIPES ----------------- #
# ----------------------------------------- #

$(NAME): $(HEADERS) $(OBJS)
	@printf "$(BLUE)Compiling $(YELLOW)$(NAME)\n$(END)"
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME) 
	@printf "$(YELLOW)$(NAME) Compiled!\n$(END)"

debug:
	@printf "$(BLUE)Compiling With Debug Flags $(YELLOW)$(NAME)\n$(END)"
	@$(CC) $(FLAGS) $(DEBUG) $(OBJS) -o $(NAME) 
	@printf "$(BLUE)The Executable $(YELLOW)$(NAME) $(BLUE)Compiled With $(RED)fsantize!\n$(END)"

all: $(NAME)

run: all
	./$(NAME)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@printf "$(BLUE)Compiling $(D_BLUE)$(notdir $@) $(RESET)From $(L_BLUE)$(notdir $<)$(END)\n"
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@printf "$(BLUE)Cleaning Up $(YELLOW)$(NAME)!\n$(END)"
	/bin/rm -rf $(OBJ_DIR)

fclean:	clean
	/bin/rm -f $(NAME)
	@printf "$(BLUE)Fully Cleaned $(YELLOW)$(NAME)\n$(END)"

re: fclean all

pog:
	@printf "\n$(RED)"
	@printf "⣿⣿⣿⣿⡿⠟⠛⠛⠛⠛⠉⠉⠙⠛⠛⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠟   \n"
	@printf "⣿⣿⣯⣥⣤⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿⣿⣏⣀⣀⣀⡀  \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠻⠿⠟⠉⠉⠉⢻⣿⣿⣿⡿⠟⠋⣡⣼⣿⣿⣿⡄ \n"
	@printf "⣿⣿⣿⣟⣭⣤⣶⣶⣿⣿⠃⠀⠀⢀⣀⣤⣶⣿⣿⣿⣿⡅⡀⢀⣩⣤⣤    \n"
	@printf "⣿⣿⣿⣿⣿⣿⣛⡛⠛⠛⠛⢋⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣛⠛⠛⠓⠠   \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣤⣤⣤⣦  \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇ \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⠿⢿⡿⢿⣿⣿⣿⠃ \n"
	@printf "⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣥⣄⣀⣀⠀⠀⠀⠀⠀⢰⣾⣿⣿⠏  \n"
	@printf "⠀⠀⠀⠀⠀⠀⠉⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣜⡻⠋    \n"
	@printf "⣰⣾⣷⣶⣿⣾⣖⣻⣿⣿⡿⣿⣿⣿⣿⠿⠿⠟⠛⠛⠛⠋⠉⠉⢉⡽⠃    \n"
	@printf "⣿⣿⣿⣿⣿⣿⡉⠉⠉⠛⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⡤⠚⠉      \n"
	@printf "⠛⠛⣿⣿⣿⣿⣿⣿⣿⠉⠛⢶⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⡇        \n"
	@printf "⠠⣾⣿⣿⣿⣿⣿⠿⠟⠃⠀⠀⠀⠈⠲⣴⣦⣤⣤⣤⣶⡾⠁        \n"
	@printf "⠄⠈⠉⠻⢿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉          \n"
	@printf "$(END)"

.PHONY: all clean fclean re pog debug
