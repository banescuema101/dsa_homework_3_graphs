build:
	gcc -Wall -Werror -Wextra -g -o tema3 tema3.c func.c func.h
run:
	./tema3
clean:
	rm tema3
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema3