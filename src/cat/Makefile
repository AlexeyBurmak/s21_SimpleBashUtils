CFLAGS=-Wall -Werror -Wextra -std=c11

all: s21_cat

s21_cat: ./s21_cat.o ../common/common.o
	gcc $^ -o ./s21_cat

s21_cat.o: ./s21_cat.c
	gcc -c $(CFLAGS) $^ -o ./s21_cat.o

common.o: ../common/common.c
	gcc -c $(CFLAGS) $^ -o ../common/common.o

clean:
	rm -rf ./*.o ./s21_cat ../common/*.o