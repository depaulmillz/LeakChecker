all: clean leakchecker.so leakcheckerpp.so test1 test2 run

leakchecker.so: leakchecker.c
	clang -shared -fPIC leakchecker.c -o build/leakchecker.so

leakcheckerpp.so: leakcheckerpp.cpp leakchecker.so
	clang++ -shared -fPIC leakcheckerpp.cpp build/leakchecker.so -o build/leakcheckerpp.so

test1: test.c
	clang -g test.c build/leakchecker.so -o build/test1

test2: test.cpp
	clang++ -g test.cpp build/leakcheckerpp.so -o build/test2

run:
	./build/test1
	./build/test2

clean:
	-@rm -r ./build/*.dSYM
	-@rm ./build/*
