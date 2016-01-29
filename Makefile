.PHONY: all default clean test ktxes debug

executables =		\
	fix_mali_ktx	\
	gen_array	\
	gen_ktx		\
	gen_mipmaps	\
	gen_mipmaps_man

objs =	astc-encoder/Source/*.o

intermediate_image_files =	\
	waffles-[0-9].jpg	\
	waffles-[0-9]_[0-9].jpg

all: default ktxes test

default: gen_mipmaps.c fix_mali_ktx.c gen_ktx.c gen_array.c piglit_ktx.c
	gcc -g3 gen_mipmaps.c -o gen_mipmaps -L ~/projects/mesa/lib -lGL -lwaffle-1 -L. -lktx
	gcc -g3 gen_array.c piglit_ktx.c -o gen_array -L ~/projects/mesa/lib -lGL -lwaffle-1 -L . -lktx
	gcc -g3 gen_mipmaps_man.c -o gen_mipmaps_man -L ~/projects/mesa/lib -lGL -lwaffle-1 -L . -lktx
	gcc -g3 fix_mali_ktx.c -o fix_mali_ktx -L . -lktx -L ~/projects/mesa/lib -lGL
	gcc -g3 gen_ktx.c -o gen_ktx -L . -lktx
	make -C astc-encoder/Source

ktxes:
	./make_ktxes.sh waffles.jpg

clean:
	make clean -C astc-encoder/Source
	rm -rf decompressed compressed
	rm -f $(executables)
	rm -f $(intermediate_image_files)

