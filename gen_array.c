#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>

#include <waffle-1/waffle.h>
#include "piglit_ktx.h"

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
	WAFFLE_CONTEXT_API,         WAFFLE_CONTEXT_OPENGL,
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
	if (!waffle_display_supports_context_api(dpy, WAFFLE_CONTEXT_OPENGL)
		|| !waffle_dl_can_open(WAFFLE_DL_OPENGL))
	{
		exit(EXIT_FAILURE);
	}

	config = waffle_config_choose(dpy, config_attrs);
	window = waffle_window_create(config, window_width, window_height);
	ctx = waffle_context_create(config, NULL);
	waffle_make_current(dpy, window, ctx);
}

struct piglit_ktx {
	struct piglit_ktx_info info;

	/** \brief The raw KTX data. */
	void *data;

	/**
	 * \brief Array of images.
	 *
	 * Array length is piglit_ktx_info::num_images.
	 */
	struct piglit_ktx_image *images;
};

/* Accepts the mipmap dir, the output filename */
int
main(int argc, char *argv[])
{
	/* Get arguments */
	char * filename = argv[1];
	int miplevels = atoi(argv[2]) + 1;
   int arg_offset = 3;
	unsigned imgs = argc - arg_offset;

	/* Create GLES2 context */
	make_context();

	/* Structs used to write the final output */
	KTX_image_info img_info[miplevels];
	KTX_texture_info tex_info;

   /* Fill out final struct */
   /* Read in the file from memory */
   char * cur_file = argv[arg_offset];
   FILE* file = fopen(cur_file, "rb");
   if (file == NULL)
      printf("File not opened\n");

   /* Skip the identifier and endianness */
   int error = fseek(file, 16, SEEK_SET);
   if (error)
      printf("File seek bad\n");

   /* Read the appropriate header bits */
   size_t size_read = fread(&tex_info, 1, sizeof(tex_info), file);
   if (size_read < sizeof(tex_info))
      printf("bad read 1\n");

   /* Close the file */
   if (file != NULL)
      fclose(file);

   /* Update the array section */
   tex_info.numberOfArrayElements = imgs;


   int cur_lev;
	int cur_img;
   int w, h, expt;
   GLuint tex[imgs];
   glGenTextures(imgs, tex);
   glPixelStorei(GL_PACK_ALIGNMENT, 1);

	/* Read in the files into memory */
   for (cur_img = 0; cur_img < imgs; ++cur_img) {
      cur_file = argv[cur_img+arg_offset];
      printf("%s\n", cur_file);
      struct piglit_ktx* file = piglit_ktx_read_file(cur_file);
      if (!piglit_ktx_load_texture(file, &tex[cur_img], NULL)) {
         printf("bad stuff just happened\n");
         return 1;
      }
      w = file->info.pixel_width;
      h = file->info.pixel_height;
      piglit_ktx_destroy(file);
   }

   for (cur_lev = 0; cur_lev < miplevels; ++cur_lev) {
		/* Get level size. All images have the same dimension */
      printf("W: %d H: %d\n", w, h);
      expt = w*h*_mesa_bytes_per_pixel(tex_info.glFormat, tex_info.glType);

      /* Create the texture array */
		img_info[cur_lev].size = expt*imgs;
		img_info[cur_lev].data = (GLubyte*) malloc(img_info[cur_lev].size);

     for (cur_img = 0; cur_img < imgs; ++cur_img) {
         glBindTexture(GL_TEXTURE_2D, tex[cur_img]);
         glGetTexImage(GL_TEXTURE_2D, cur_lev, tex_info.glFormat, tex_info.glType,
                        &img_info[cur_lev].data[cur_img*expt]);
      }

      w = w/2 ? w/2 : 1;
      h = h/2 ? h/2 : 1;

   }

	/* Write memory object to file */
	KTX_error_code kec;
	kec = ktxWriteKTXN(filename, &tex_info, 0, NULL, miplevels, img_info);
	printf("%s - %s\n", filename, ktxErrorString(kec));

	/* Cleanup */
   glDeleteTextures(imgs, tex);
   for (cur_lev = 0; cur_lev < miplevels; ++cur_lev)
      free(img_info[cur_lev].data);
	return 0;
}
