NAME			= irc
SRC				= main.cpp Channel.cpp \
				Server.cpp Client.cpp \
				Command.cpp \
				cmds/cmd_join.cpp \
				cmds/cmd_who.cpp \
				cmds/cmd_privmsg.cpp

OBJ				= $(SRC:.cpp=.o)
SRCDIR      	= ./srcs/
OBJDIR      	= ./objs/
SRCFILES    	= $(addprefix $(SRCDIR), $(SRC))
OBJFILES    	= $(addprefix $(OBJDIR), $(OBJ))
DEPFILES    	= $(OBJFILES:.o=.d)
INCLUDEDIR  	= ./includes/
HEADERSFILES    = Server.hpp Client.hpp \
				Command.hpp Channel.hpp

HEADERS			:= $(addprefix $(INCLUDEDIR), $(HEADERSFILES))

CC          	= clang++
CFLAGS      	= -Wall -Wextra -Werror -I$(INCLUDEDIR)
DEBUGFLAGS           = -g3 -fsanitize=address

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJFILES)
	@$(CC) -o $@ $(OBJFILES) $(CFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR) $(OBJDIR)cmds/

debug: $(OBJDIR) $(OBJFILES)
	@$(CC) -o $@ $(OBJFILES) $(CFLAGS) $(DEBUGFLAGS)

clean:
	@/bin/rm -rf $(OBJDIR)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

save: fclean
		 git add .
		 git commit -m "$m"
		 git push