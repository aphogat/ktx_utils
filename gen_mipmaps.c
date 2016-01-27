#define KTX_OPENGL 1
#include <ktx.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <waffle-1/waffle.h>

GLint
_mesa_components_in_format(GLenum format)
{
   switch (format) {

   case GL_RGB:
   case GL_BGR:
      return 3;

   case GL_RGBA:
   case GL_BGRA:
      return 4;

   default:
      return -1;
   }
}

GLint
_mesa_bytes_per_pixel(GLenum format, GLenum type)
{
   GLint comps = _mesa_components_in_format(format);
   if (comps < 0)
      return -1;

   switch (type) {
   case GL_BYTE:
   case GL_UNSIGNED_BYTE:
      return comps * sizeof(GLubyte);
   case GL_SHORT:
   case GL_UNSIGNED_SHORT:
      return comps * sizeof(GLshort);
   case GL_INT:
   case GL_UNSIGNED_INT:
      return comps * sizeof(GLint);
   case GL_FLOAT:
      return comps * sizeof(GLfloat);
   case GL_HALF_FLOAT:
      return comps * sizeof(GLhalf);
   default:
      return -1;
   }
}




int
no_dot_output_filter (const struct dirent * dir_entity)
{
	return strlen(dir_entity->d_name) > 2 &&
		strstr(dir_entity->d_name, "waffles.ktx") == NULL &&
		strstr(dir_entity->d_name, "swp") == NULL;
}

void make_context()
{
	/* Generate a context */
	struct waffle_display *dpy;
	struct waffle_config *config;
	struct waffle_window *window;
	struct waffle_context *ctx;

	const int32_t init_attrs[] = {
	WAFFLE_PLATFORM, WAFFLE_PLATFORM_X11_EGL,
	0,
	};

	const int32_t config_attrs[] = {
	WAFFLE_CONTEXT_API,         WAFFLE_CONTEXT_OPENGL_ES2,
	WAFFLE_RED_SIZE,            8,
	WAFFLE_BLUE_SIZE,           8,
	WAFFLE_GREEN_SIZE,          8,

	0,
	};

	const int32_t window_width = 320;
	const int32_t window_height = 240;

	waffle_init(init_attrs);
	dpy = waffle_display_connect(NULL);

	// Exit if OpenGL ES2 is unsupported.
	if (!waffle_display_supports_context_api(dpy, WAFFLE_CONTEXT_OPENGL_ES2)
		|| !waffle_dl_can_open(WAFFLE_DL_OPENGL_ES2))
	{
		exit(EXIT_FAILURE);
	}

	config = waffle_config_choose(dpy, config_attrs);
	window = waffle_window_create(config, window_width, window_height);
	ctx = waffle_context_create(config, NULL);
	waffle_make_current(dpy, window, ctx);
}

/* Accepts the mipmap dir, the output filename */
int
main(int argc, char *argv[])
{
	/* Get arguments */
	char * output_dir = argv[1];
	char * filename = argv[2];

	/* Create GLES2 context */
	make_context();

	/* List directories (adapted from scandir manpage) */
	struct dirent **namelist;
	int levels = scandir(output_dir, &namelist, no_dot_output_filter, alphasort);
        if (levels < 0)
            perror("scandir");

	/* Structs used to write the final output */
	KTX_texture_info tex_info;
	KTX_image_info *img_info = (KTX_image_info*)malloc(levels * sizeof(KTX_image_info));

	int cur_lev;
	char cur_path[4096];
        for (cur_lev = 0; cur_lev < levels; ++cur_lev) {

		/* Create full file path */
		char * cur_file = namelist[cur_lev]->d_name;
		strcpy(cur_path, output_dir);
		strcat(cur_path, cur_file);
		//printf("%s\n", cur_path);

		/* Read in the file from memory */
		FILE* file = fopen(cur_path, "rb");
		if (file == NULL)
			printf("File not opened\n");


		/* Skip the identifier and endianness */
		int error = fseek(file, 16, SEEK_SET);
		if (error)
			printf("File seek bad\n");

		/* Read the appropriate header bits */
		KTX_texture_info ltex_info;
		size_t size_read = fread(&ltex_info, 1, sizeof(ltex_info), file);
		if (size_read < sizeof(ltex_info))
			printf("bad read 1\n");

		/* Fill out final struct */
		if (cur_lev == 0) {
			memcpy(&tex_info, &ltex_info, sizeof(ltex_info));

			/* Update the mipmap section */
			tex_info.numberOfMipmapLevels = levels;

		}

		/* Skip the bytes of Key Value Data */
		error = fseek(file, 4, SEEK_CUR);
		if (error)
			printf("File seek bad\n");

		/* Get the image size */
		GLsizei *lsize = &img_info[cur_lev].size;
		GLubyte **ldata = &img_info[cur_lev].data;
		size_read = fread(lsize, 1, 4, file);
		if (size_read < 4)
			printf("bad read 2\n");

		/* Get the image data */
		*ldata = (GLubyte*) malloc(*lsize);
		size_read = fread(*ldata, 1, *lsize, file);
		if (size_read < *lsize)
			printf("bad read 2\n");

		/* Convert miplevel to the original image's format */
		if (tex_info.glFormat != ltex_info.glFormat) {
			printf("MIPLEVEL %d: Different glFormat!\ncomps: %d vs %d\n", cur_lev,
					_mesa_components_in_format(tex_info.glFormat),
					_mesa_components_in_format(ltex_info.glFormat));
			/* Conversion step */
			GLsizei lsize2 = ltex_info.pixelHeight * ltex_info.pixelWidth * _mesa_bytes_per_pixel(tex_info.glFormat, tex_info.glType) ;
			GLubyte *ldata2 = (GLubyte*) malloc(lsize2);
			if (ldata2 == NULL)
				puts("malloc failed");
			int offset = 0;
			int offset2 = 0;
			while (offset < *lsize) {
				memcpy(ldata2 + offset2, *ldata+offset, 3);
				ldata2[offset2+3] = 255;
				offset+=3;
				offset2+=4;
			}
			/* Saving step */
			free(*ldata);
			*lsize = lsize2;
			*ldata = ldata2;
		}

		free(namelist[cur_lev]);

		/* Close the file */
		if (file != NULL)
			fclose(file);
        }

	/* Write memory object to file */
	KTX_error_code kec;
	strcpy(cur_path, output_dir);
	strcat(cur_path, filename);
	kec = ktxWriteKTXN(cur_path, &tex_info, 0, NULL, levels, img_info);
	printf("%s - %s\n", cur_path, ktxErrorString(kec));

	/* Cleanup */
        free(namelist);
        for (cur_lev = 0; cur_lev < levels; ++cur_lev)
		free(img_info[cur_lev].data);
        free(img_info);
	return 0;
}
