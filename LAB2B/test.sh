# throughput of mutex and spin
./lab2_list --threads=1  --iterations=1000 --sync=m > lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=s > lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv

# How many iterations does it take to fail
./lab2_list --threads=1  --iterations=1   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=2   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=4   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=8   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=16  --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=2   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=4   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=8   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=16  --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=2   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=4   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=8   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=16  --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=2   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=4   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=8   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=16  --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=2   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=4   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=8   --yield=id --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=16  --yield=id --lists=4 >> lab2b_list.csv


# test with sync=s option for threads 1,4,8,12,16 and iterations 10,20,40,80
# generates lab2_3.png
./lab2_list --threads=1  --iterations=10  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=1  --iterations=20  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=1  --iterations=40  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=1  --iterations=80  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=4  --iterations=10  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=4  --iterations=20  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=4  --iterations=40  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=4  --iterations=80  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=8  --iterations=10  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=8  --iterations=20  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=8  --iterations=40  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=8  --iterations=80  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=10  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=10  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40  --yield=id --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80  --yield=id --lists=4 --sync=s >> lab2b_list.csv

# Test with sync=, option for threads = 1,4,8,12,16 and iterations = 10,20,40,80
# generates lab2_3.png

./lab2_list --threads=1  --iterations=10  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=1  --iterations=20  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=1  --iterations=40  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=1  --iterations=80  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=4  --iterations=10  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=4  --iterations=20  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=4  --iterations=40  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=4  --iterations=80  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=8  --iterations=10  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=8  --iterations=20  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=8  --iterations=40  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=8  --iterations=80  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=10  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=10  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=10  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=10  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40  --yield=id --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80  --yield=id --lists=4 --sync=m >> lab2b_list.csv


# test without yields for 1000 iterations with threads = 1,2,4,8,12 and lists = 1,4,8,16
# generates lab2_4.png and lab2_5.png

./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
./lab2_list --threads=1  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
./lab2_list --threads=2  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
./lab2_list --threads=4  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
./lab2_list --threads=8  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
