.SILENT:
files = SortedList.h SortedList.c lab2_list.c Makefile README *.png profile.out test.sh lab2b.gp lab2b_list.csv 

default:
	gcc -pthread -lrt -std=gnu99 -o lab2_list -g lab2_list.c SortedList.c SortedList.h

tests: default
	chmod +x test.sh
	./test.sh

profile: default
	rm -f ./raw.gperf
	LD_PRELOAD= /u/eng/class/classkth/lib/libprofiler.so.0 \
	CPUPROFILE=raw.gperf ./lab2_list --threads=12 iterations=1000 --sync=s
	pprof --text lab2_list ./raw.gperf > profile.out
	pprof --list=test_run ./lab2_list ./raw.gperf >> profile.out
	rm -f ./raw.gperf

graphs: default tests
	export PATH=/usr/local/cs/bin:$PATH
	gnuplot ./lab2b.gp
dist:
	tar -zcvf lab2b-504539433.tar.gz $(files)

clean:
	rm -f lab2_list *~ 

