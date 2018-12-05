all: clean leakchecker.so test1 test2 run

leakchecker.so: leakchecker.c
	clang -shared -fPIC leakchecker.c -o build/leakchecker.so

test1: test.c
	clang -g test.c build/leakchecker.so -o build/test1

test2: test.cpp
	clang++ -g test.cpp build/leakchecker.so -o build/test2

run:
	./build/test1
	./build/test2

clean:
	-@rm -r ./build/*.dSYM
	-@rm ./build/*
