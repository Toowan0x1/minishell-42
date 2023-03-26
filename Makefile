all:
	$(MAKE) -C libft
	gcc -Wall -Wextra -Werror minishell.c get_next_line/get_next_line.c libft/libft.a -o minishell

clean:
	rm -rf *.o
	rm -rf libft/*.o

fclean: clean
	rm -rf minishell