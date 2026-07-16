# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/16 01:32:56 by kaaltint          #+#    #+#              #
#    Updated: 2026/07/16 06:19:47 by kaaltint         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= 42-examshell
ROOT		:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

CC			:= cc
# Ask the active compiler which warning switches it supports. 42 machines use
# clang while many Linux machines use GCC; GCC's format-truncation switch is
# an error under clang when -Werror is active.
cc-option	 = $(shell printf 'int main(void){return 0;}\n' | \
	$(CC) $(1) -Werror -x c -c -o /dev/null - >/dev/null 2>&1 && \
	printf '%s' '$(1)')
WARN_SUPPRESS	:= $(call cc-option,-Wno-format-truncation) \
			   $(call cc-option,-Wno-missing-field-initializers)
CFLAGS		:= -Wall -Wextra -Werror -O2 $(WARN_SUPPRESS)
CPPFLAGS	:= -Iinclude -DEXAMSHELL_ROOT=\"$(ROOT)\"
DEPFLAGS	:= -MMD -MP

SRCDIR		:= src
OBJDIR		:= .build
SRCS		:= $(wildcard $(SRCDIR)/*.c)
OBJS		:= $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS		:= $(OBJS:.o=.d)

PREFIX		?= $(HOME)/.local
LEGACY_NAME	:= examshell

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@printf "  \033[32mbuilt\033[0m   ./$(NAME)\n"
	@printf "  \033[90mrun\033[0m     ./$(NAME)\n"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(DEPS)

# Use a distinct command name: installing this simulator must never shadow
# 42's real `examshell` command.
install: $(NAME)
	@mkdir -p $(PREFIX)/bin
	@printf '#!/bin/sh\nEXAMSHELL_ROOT="%s" exec "%s/%s" "$$@"\n' \
		"$(ROOT)" "$(ROOT)" "$(NAME)" > $(PREFIX)/bin/$(NAME)
	@chmod +x $(PREFIX)/bin/$(NAME)
	@if [ -f "$(PREFIX)/bin/$(LEGACY_NAME)" ] \
		&& grep -q '^EXAMSHELL_ROOT=' "$(PREFIX)/bin/$(LEGACY_NAME)"; then \
		rm -f "$(PREFIX)/bin/$(LEGACY_NAME)"; \
		printf "  \033[33mremoved\033[0m   old simulator launcher %s\n" \
			"$(PREFIX)/bin/$(LEGACY_NAME)"; \
	fi
	@printf "  \033[32minstalled\033[0m $(PREFIX)/bin/$(NAME)\n"
	@case ":$$PATH:" in *":$(PREFIX)/bin:"*) ;; *) \
		printf "  \033[33mnote\033[0m      add %s to your PATH\n" \
		"$(PREFIX)/bin" ;; esac

uninstall:
	@rm -f $(PREFIX)/bin/$(NAME)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all install uninstall clean fclean re
