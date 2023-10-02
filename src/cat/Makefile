all: clean s21_cat
s21_cat:
	gcc -Wall -Werror -Wextra s21_cat.c -o s21_cat

style:
	clang-format -style=Google -n -Werror *.c

test: s21_cat
	bash test_func_cat.sh 
clean:
	rm -f *.o s21_cat
rebuild: clean all
