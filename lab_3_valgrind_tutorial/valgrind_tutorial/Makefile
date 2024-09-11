VG_OPTS=--leak-check=full --dsymutil=yes --log-file=vgoutput.txt --read-var-info=yes --show-reachable=yes

vgexample: vgexample.cpp
	g++ -Wall -g -O0 $^ -o $@

run: vgexample
	./valgrind_example

runvg: vgexample
	valgrind $(VG_OPTS) ./$<

output.txt vgoutput.txt: vgexample
	valgrind $(VG_OPTS) ./$< | tee output.txt
	
