.SILENT:

default:
	gcc -g -pthread -std=gnu99 -lrt -o lab2_add lab2_add.c
	gcc -g -pthread -std=gnu99 -lrt -o lab2_list lab2_list.c SortedList.c SortedList.h 
clean:
	rm -f lab2_add lab2_list lab2a-504539433.tar.gz
dist:
	tar -zvcf lab2a-504539433.tar.gz lab2_add.c lab2_list.c *.png sample.sh SortedList.h SortedList.c README Makefile lab2_add.csv lab2_list.csv

tests:
	chmod u+x sample.sh
	./sample.sh
graphs: default
	export PATH=/usr/local/cs/bin:$PATH
	chmod u+x sample.sh
	./sample.sh
	gnuplot ./lab2_add.gp
	gnuplot ./lab2_list.gp

