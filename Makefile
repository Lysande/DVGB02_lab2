compile:
	gcc Jim.c Stud.c -o rdt
compile-wall:
	gcc -Wall Jim.c Stud.c -o rdt
test-timeout:
	gcc Jim.c Stud.c -o rdt
	./rdt<indata_timeout