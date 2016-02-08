#define KTX_OPENGL 1
#include <ktx.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct _astc_header
{
        uint8_t magic[4];
        uint8_t blockdim_x;
        uint8_t blockdim_y;
        uint8_t blockdim_z;
        uint8_t xsize[3];
        uint8_t ysize[3];
        uint8_t zsize[3];
} astc_header;


static void
format_ktx_header(astc_header* header, GLenum format, KTX_texture_info * kti)
{
#define get_val(x) (x[0] + (x[1] << 8) + (x[2] << 16))
	kti->glType 			= 0;
	kti->glTypeSize			= 1;
	kti->glFormat			= 0;
	kti->glInternalFormat		= format;
	kti->glBaseInternalFormat	= GL_RGBA;
	kti->pixelWidth 		= get_val(header->xsize);
	kti->pixelHeight 		= get_val(header->ysize);
	kti->pixelDepth 		= get_val(header->zsize) > 1 ? get_val(header->zsize) : 0;
	kti->numberOfArrayElements 	= 0; // assuming array texture
	kti->numberOfFaces 		= 1; // assuming not cubemap or cubemap array
	kti->numberOfMipmapLevels 	= 1; // assuming only 1 miplevel
#undef get_val
}

static void
print_astc_header(astc_header * header)
{
	printf("Magic is 0x%X", header->magic[3]);
	printf("%X", header->magic[2]);
	printf("%X", header->magic[1]);
	printf("%X\n", header->magic[0]);
	printf("Blockdimx is %d\n", header->blockdim_x);
	printf("Blockdimy is %d\n", header->blockdim_y);
	printf("Blockdimz is %d\n", header->blockdim_z);
}

static void *
gen_ktx_from_astc(const char *filename, int *data_size, GLenum format, KTX_texture_info * kti)
{

	/* Open up the file */
	printf("Opening %s\n", filename);
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
		printf("File not opened\n");


	/* Generate ktx header given astc_header in file */
	int astc_header_sz = sizeof(astc_header);
	astc_header astc_header;
	size_t size_read = fread(&astc_header, 1, astc_header_sz, file);

	if (size_read < astc_header_sz)
		printf("bad read\n");
	else
		print_astc_header(&astc_header);

	format_ktx_header(&astc_header, format, kti);


	/* Get blob size then move to start of blob */
	int error = fseek(file, 0, SEEK_END);
	if (error)
		printf("File seek bad\n");
	*data_size = ftell(file) - astc_header_sz;
	error = fseek(file, astc_header_sz, SEEK_SET);
	if (error)
		printf("File seek set bad\n");

	/* Load image blob from file */
	void *astc_img = malloc(*data_size);
	if (astc_img == NULL)
		printf("OOM\n");
	size_read = fread(astc_img, 1, *data_size, file);
	if (size_read < *data_size)
		printf("bad read2\n");

	/* Cleanup */
	if (file != NULL)
		fclose(file);

	return astc_img;
}


/* Accepts the astc file and the hex ASTC enum */
int
main(int argc, char *argv[])
{
	/* Get arguments */
	char * filename = argv[1];
	char * output = argv[2];
	GLenum format = strtol(argv[3], NULL, 16);

	/* Write memory object to file */
	KTX_image_info kii;
	KTX_texture_info kti;
	kii.data = (GLubyte*) gen_ktx_from_astc(filename, &kii.size, format, &kti);
	ktxWriteKTXN(output, &kti, 0, NULL, 1, &kii);

	/* Cleanup */
	free(kii.data);
	return 0;
}
