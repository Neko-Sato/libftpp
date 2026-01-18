# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/24 19:05:06 by hshimizu          #+#    #+#              #
#    Updated: 2026/01/18 15:34:45 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			:= libftpp.a

SRCS			:= 

OUTDIR			:= .out
OBJS			:= $(addprefix $(OUTDIR)/, $(SRCS:.cpp=.o))
DEPS			:= $(addprefix $(OUTDIR)/, $(SRCS:.cpp=.d))
DEPS_DEV		:= $(addprefix $(OUTDIR)/, $(SRCS:.cpp=_dev.d))

CXX				:= c++
CFLAGS			:= -Wall -Wextra -Werror -std=c+23 -pedantic
CFLAGS			+= -fno-builtin -fno-common
CFLAGS			+= -fPIC -MMD -MP
AR				:= ar
ARFLAGS			:= rcs
IDFLAGS			:= -I.
LDFLAGS			:=

CFLAGS			+= -g -fsanitize=address
ifneq ($(shell $(CXX) --version | grep -i clang),)
CFLAGS			+= -fstandalone-debug
endif

.PHONY: all clean fclean re bonus

all: $(NAME)
bonus: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OUTDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IDFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OUTDIR)

fclean: clean
	$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE)

-include $(DEPS)
