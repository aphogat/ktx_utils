#define KTX_OPENGL 1
#include <ktx.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <waffle-1/waffle.h>

int
no_dot_output_filter (const struct dirent * dir_entity)
{
	return strlen(dir_entity->d_name) > 2 &&
		strstr(dir_entity->d_name, "simple.ktx") == NULL &&
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

	/* Write a file from Memory */
	/* Write memory object to file */
	strcpy(cur_path, output_dir);
	strcat(cur_path, filename);
	FILE* fileo = fopen(cur_path, "wb");
	if (fileo == NULL)
		printf("File not opened\n");



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


		/* Read the identifier and endianness */
		uint32_t header_top[4];
		size_t size_read = fread(header_top, 1, 16, file);
		if (size_read < 16)
			printf("bad read 0\n");

		/* Read the appropriate header bits */
		KTX_texture_info ltex_info;
		size_read = fread(&ltex_info, 1, sizeof(ltex_info), file);
		if (size_read < sizeof(ltex_info))
			printf("bad read 1\n");

		/* Skip the bytes of Key Value Data */
		int error = fseek(file, 4, SEEK_CUR);
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

		/* Clean up */
		free(namelist[cur_lev]);

		/* Close the file */
		if (file != NULL)
			fclose(file);

		/* Fill out final struct */
		if (cur_lev == 0) {
			memcpy(&tex_info, &ltex_info, sizeof(ltex_info));

			/* Update the mipmap section */
			tex_info.numberOfMipmapLevels = levels;

			/* Update the final file with this data */
			uint32_t nokvdat = 0;
			fwrite(header_top, 16, 1, fileo);
			fwrite(&tex_info, sizeof(tex_info), 1, fileo);
			fwrite(&nokvdat, sizeof(nokvdat), 1, fileo);
		}

		/* Update the final file with this data */
		fwrite(lsize, sizeof(GLubyte), 1, fileo);
		fwrite(*ldata, *lsize, 1, fileo);
		
		/* mip padding */
		error = fseek(fileo, 3-((*lsize+3) % 4), SEEK_CUR);
		if (error)
			printf("File seek bad\n");

        }

	///KTX_error_code kec;
	///kec = ktxWriteKTXN(cur_path, &tex_info, 0, NULL, levels, img_info);
	///printf("%s - %s\n", cur_path, ktxErrorString(kec));


	/* Close the file */
	if (fileo != NULL)
		fclose(fileo);

	/* Cleanup */
        free(namelist);
        for (cur_lev = 0; cur_lev < levels; ++cur_lev)
		free(img_info[cur_lev].data);
        free(img_info);
	return 0;
}
