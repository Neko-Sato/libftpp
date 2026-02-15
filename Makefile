# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/24 19:05:06 by hshimizu          #+#    #+#              #
#    Updated: 2026/02/16 01:42:54 by hshimizu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			:= libftpp.a

SRCS			:= \
	./data_structures/data_buffer.cpp \
	./mathematics/perlin_noise_2D.cpp \
	./mathematics/ivector2.cpp \
	./mathematics/ivector3.cpp \
	./mathematics/random_2D_coordinate_generator.cpp \
	./threading/thread_safe_iostream.cpp \
	./threading/thread.cpp \
	./threading/worker_pool.cpp \
	./threading/persistent_worker.cpp \
	./design_patterns/memento.cpp

OUTDIR		:= .out
OBJS			:= $(addprefix $(OUTDIR)/, $(SRCS:.cpp=.o))
DEPS			:= $(addprefix $(OUTDIR)/, $(SRCS:.cpp=.d))

CXX				:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++23 -pedantic
CXXFLAGS	+= -fno-builtin -fno-common
CXXFLAGS	+= -fPIC -MMD -MP
AR				:= ar
IDFLAGS		:= -I.
LDFLAGS		:=

# CXXFLAGS	+= -g -fsanitize=address
# ifneq ($(shell $(CXX) --version | grep -i clang),)
# CXXFLAGS	+= -fstandalone-debug
# endif

.PHONY: all clean fclean re bonus

all: $(NAME)
bonus: $(NAME)

$(NAME): $(OBJS)
	$(AR) rcs $@ $^

$(OUTDIR)/%.o: %.cpp | $(PCHS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(IDFLAGS) -c $< -o $@


clean:
	$(RM) -r $(OUTDIR)

fclean: clean
	$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE)

-include $(DEPS)
