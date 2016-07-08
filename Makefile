.PHONY: all default clean test ktxes debug

all: default ktxes test

default: gen_mipmaps.c fix_mali_ktx.c gen_ktx.c gen_array.c piglit_ktx.c
	gcc -g gen_mipmaps.c -o gen_mipmaps -L ~/repos/mesa/lib -lGL -lwaffle-1 -lktx
	gcc -g gen_array.c piglit_ktx.c -o gen_array -L ~/repos/mesa/lib -lGL -lwaffle-1 -lktx
	gcc -g gen_mipmaps_man.c -o gen_mipmaps_man -L ~/repos/mesa/lib -lGL -lwaffle-1 -lktx
	gcc -g fix_mali_ktx.c -o fix_mali_ktx -lktx -L ~/repos/mesa/lib -lGL
	gcc -g gen_ktx.c -o gen_ktx -lktx
	make -C astc-encoder/Source

ktxes:
	./make_ktxes.sh waffles.jpg

test:
	ninja -C ~/repos/piglit2
	MESA_DEBUG=1 ~/repos/piglit2/bin/khr_compressed_astc-miptree_gl

debug:
	ninja -C ~/repos/piglit2
	MESA_DEBUG=1 gdb --args ~/repos/piglit2/bin/khr_compressed_astc-miptree_gl

clean:
	git clean -df decompressed compressed

