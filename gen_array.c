#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>

#include <waffle-1/waffle.h>
#include "piglit_ktx.h"


int
no_dot_output_filter (const struct dirent * dir_entity)
{
	return strlen(dir_entity->d_name) > 2 &&
		strstr(dir_entity->d_name, "waffles.ktx") == NULL &&
		strstr(dir_entity->d_name, ".ktx") != NULL &&
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
	KTX_image_info *img_info = (KTX_image_info*)malloc(imgs * sizeof(KTX_image_info));
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
   struct piglit_ktx* files[imgs];
   GLuint tex[imgs];
   glGenTextures(imgs, tex);
   for (cur_lev = 0; cur_lev < miplevels; ++cur_lev) {
     int level_size = 0;
     for (cur_img = 0; cur_img < imgs; ++cur_img) {

      char * cur_file = argv[cur_img+arg_offset];

		/* Read in the files into memory */
      if (cur_lev == 0) {
         printf("%s\n", cur_file);
         GLenum my_gl_error;
         files[cur_img] = piglit_ktx_read_file(cur_file);
         piglit_ktx_load_texture(files[cur_img], &tex[cur_img], &my_gl_error);
      }
         

		/* Get level size */
         w = (files[cur_img]->info.pixel_width >> cur_lev) ? (files[cur_img]->info.pixel_width >> cur_lev) : 1;
         h = (files[cur_img]->info.pixel_height >> cur_lev) ? (files[cur_img]->info.pixel_height >> cur_lev) : 1;
         expt = w*h*6;
      level_size += piglit_ktx_get_image(files[cur_img], cur_lev, 0)->size;
     }

      /* Create the texture array */
		img_info[cur_lev].size = expt*imgs;
		img_info[cur_lev].data = (char*) malloc(level_size);

     for (cur_img = 0; cur_img < imgs; ++cur_img) {
         glBindTexture(GL_TEXTURE_2D, tex[cur_img]);
         glGetTexImage(GL_TEXTURE_2D, cur_lev, tex_info.glFormat, tex_info.glType,
                        img_info[cur_lev].data);
      }

   }

	/* Write memory object to file */
	KTX_error_code kec;
	kec = ktxWriteKTXN(filename, &tex_info, 0, NULL, imgs, img_info);
	printf("%s - %s\n", filename, ktxErrorString(kec));

	/* Cleanup */
   // for (cur_img = 0; cur_img < imgs; ++cur_img)
   //   free(img_info[cur_img].data);
   // free(img_info);
	return 0;
}
