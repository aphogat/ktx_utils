#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Write GL_RGB16F into glInternalFormat */
int
main(int argc, char* argv[])
{

	/* Open up the file */
	const char *filename = argv[1];
	int compressed = strstr(filename, "decompressed") == NULL;
	int unorm8s = strstr(filename, "ldrs") != NULL;
	int hdr = strstr(filename, "hdr") != NULL;
	FILE* file = fopen(filename, "r+b");
	if (file == NULL)
		printf("File not opened\n");

#define GL_UNSIGNED_BYTE			0x1401
#define GL_HALF_FLOAT                     	0x140B

#define GL_RGB					0x1907
#define GL_RGBA					0x1908
#define GL_LUMINANCE				0x1909
#define GL_LUMINANCE_ALPHA			0x190A

#define GL_RGBA8				0x8058
#define GL_RGBA16F                        	0x881A
#define GL_RGB16F                         	0x881B
#define GL_SRGB8_ALPHA8                   	0x8C43
#define GL_SRGB8                          	0x8C41

	uint32_t d_hdr[5] = {GL_HALF_FLOAT,
				2,
				GL_RGB,
				GL_RGB16F,
				GL_RGB};

	uint32_t d_linear[5] = {GL_HALF_FLOAT,
				2,
				GL_RGBA,
				GL_RGBA16F,
				GL_RGBA};

	uint32_t d_srgb[5] = {GL_UNSIGNED_BYTE,
				1,
				GL_RGBA,
				GL_SRGB8_ALPHA8, 
				GL_RGBA};

	uint32_t *packet = NULL;
	if (hdr) {
		packet = d_hdr;
		fseek(file, 4*sizeof(uint32_t), SEEK_SET); // glType
		fwrite(packet, 5*sizeof(uint32_t), 1, file);
		puts("HDR");
	} else if (unorm8s) {
		packet = d_srgb;
		fseek(file, 4*sizeof(uint32_t), SEEK_SET); // glType
		fwrite(packet, 5*sizeof(uint32_t), 1, file);
		puts("SRGB");
	} else {
		packet = d_linear;
		fseek(file, 4*sizeof(uint32_t), SEEK_SET); // glType
		fwrite(packet, 5*sizeof(uint32_t), 1, file);
		puts("LINEAR");
	}

	/* Close the file */
	if (file != NULL)
		fclose(file);

	return 0;
}


