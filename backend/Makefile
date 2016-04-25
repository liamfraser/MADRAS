all: precompile madras

clean:
	rm -r build

precompile:
	mkdir -p build

madras:
	./set_flags.pl
	cd build; make -j4

debug: precompile
	./set_flags.pl --debug
	cd build; make -j4

run:
	./build/MADRAS
