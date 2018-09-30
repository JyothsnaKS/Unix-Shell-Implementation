all: shell
shell: shell.c command_parser.c built_in_command.c external_command.c shell.h command_parser.h built_in_command.h external_command.h
	gcc -g -o shell shell.c command_parser.c built_in_command.c external_command.c -lreadline
