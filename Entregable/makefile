build_dir=build
src_dir=src

all: folders
	gcc -o $(build_dir)/mandelbrot.o $(src_dir)/mandelbrot.c -lm -lpthread
	gcc -o $(build_dir)/mandelbrotp.o $(src_dir)/mandelbrotp.c -lm -lpthread

folders:
	mkdir $(build_dir)
