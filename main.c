#warning "Below are lot of trash code! Keep your eyes safe. You've been warned."

#define FUSE_USE_VERSION 29

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

/* Strings representing our folder structure. */
static const char *bar_path = "/bar";
static const char *foo_path = "/foo";
static const char *baz_path = "/baz";
static const char *bin_path = "/bar/bin";

/* Example file with sample text */
static const char *example_path = "/baz/example";
static const char *example_str = "Hello world\n";

/* Filename and it's text from 11 lines (as number of last to figures of zachoDka) */
static const char *test_path = "/foo/test.txt";
static const char *test_str = "She was, my Lord; and only Isabel\n\
Was all the daughters that this Phillip had,\n\
Whom afterward your father took to wife;\n\
And from the fragrant garden of her womb\n\
Your gracious self, the flower of Europe's hope,\n\
Derived is inheritor to France.\n\
But note the rancor of rebellious minds:\n\
When thus the lineage of le Bew was out,\n\
The French obscured your mother's Privilege,\n\
And, though she were the next of blood, proclaimed\n\
John, of the house of Valois, now their king\n";

/* Obvious :-) */
static const char *readme_path = "/bar/bin/readme.txt";
static const char *readme_str = "Student Yevhenia Sakovets, 1111\n";

/* Path to binary file */
static const char *paste_path = "/bar/bin/paste";

static int paste_fd;
static struct stat stbufs[1204];

/* Supplementary function for checking rights of file */
int check_rights(int flags, int mode)
{
	mode = mode & 0777;
	mode = mode / 0100;
	flags = flags & 3;
	if (((mode & 04) == 04 && (flags == O_RDONLY)) ||
		((mode & 02) == 02 && (flags == O_WRONLY)) ||
		((mode & 06) == 06 && (flags == O_RDWR)))
		return 0;
	else
		return -EACCES;
}

/* Get attributs of file */
static int fs_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;

	/* Default: did not set attributes */
	static int is_set[1024];

    memset(stbuf, 0, sizeof(struct stat));
	
	/* Setting owner of file and group owner */
	stbuf->st_uid = getuid();
	stbuf->st_gid = getuid();

	/* Checking path, checking set flag and setting or just getting attributes
	 * Result returned at stbuf pointer */
    if(strcmp(path, "/") == 0) {
		if (!is_set[0]) {
        	stbuf->st_mode = S_IFDIR | 0777;
        	stbuf->st_nlink = 5;
			is_set[0] = 1;
			memcpy(&stbufs[0], stbuf, sizeof(struct stat));
		}
		else {
			memcpy(stbuf, &stbufs[0], sizeof(struct stat));
		}
	}
    else if(strcmp(path, bar_path) == 0) {
		if (!is_set[1]) {
        	stbuf->st_mode = S_IFDIR | 0755;
        	stbuf->st_nlink = 3;
			is_set[1] = 1;
			memcpy(&stbufs[1], stbuf, sizeof(struct stat));
		}
		else {
			memcpy(stbuf, &stbufs[1], sizeof(struct stat));
		}
    }
    else if(strcmp(path, bin_path) == 0) {
		if (!is_set[2]) {
        stbuf->st_mode = S_IFDIR | 0700;
        stbuf->st_nlink = 2;
			is_set[2] = 1;
			memcpy(&stbufs[2], stbuf, sizeof(struct stat));
		}
		else {
			memcpy(stbuf, &stbufs[2], sizeof(struct stat));
		}
    }
	else if(strcmp(path, foo_path) == 0) {
		if (!is_set[3]) {
        stbuf->st_mode = S_IFDIR | 0333;
        stbuf->st_nlink = 2;
			is_set[3] = 1;
			memcpy(&stbufs[3], stbuf, sizeof(struct stat));
		}
		else {
			memcpy(stbuf, &stbufs[3], sizeof(struct stat));
		}
	}
	else if(strcmp(path, baz_path) == 0) {
		if (!is_set[4]) {
        stbuf->st_mode = S_IFDIR | 0644;
        stbuf->st_nlink = 2;
			is_set[4] = 1;
			memcpy(&stbufs[4], stbuf, sizeof(struct stat));
		}
		else {
			memcpy(stbuf, &stbufs[4], sizeof(struct stat));
		}
	}
	else if(strcmp(path, example_path) == 0) {
		if (!is_set[5]) {
        stbuf->st_mode = S_IFREG | 0211;
        stbuf->st_nlink = 1;
		stbuf->st_size = strlen(example_str);
			is_set[5] = 1;
			memcpy(&stbufs[5], stbuf, sizeof(struct stat));
		}
		else {
			memcpy(stbuf, &stbufs[5], sizeof(struct stat));
		}
	}
	else if(strcmp(path, test_path) == 0) {
		if (!is_set[6]) {
        stbuf->st_mode = S_IFREG | 0707;
        stbuf->st_nlink = 1;
		stbuf->st_size = strlen(test_str);
			is_set[6] = 1;
			memcpy(&stbufs[6], stbuf, sizeof(struct stat));
		}
		else {
			memcpy(stbuf, &stbufs[6], sizeof(struct stat));
		}
	}
	else if(strcmp(path, readme_path) == 0) {
		if (!is_set[7]) {
        	stbuf->st_mode = S_IFREG | 0400;
        	stbuf->st_nlink = 1;
			stbuf->st_size = strlen(readme_str);
			memcpy(&stbufs[7], stbuf, sizeof(struct stat));
			is_set[7] = 1;
		}
		else {
			memcpy(stbuf, &stbufs[7], sizeof(struct stat));
		}
	}
	else if(strcmp(path, paste_path) == 0) {
		if (!is_set[8]) {
		struct stat buf;
		stat("/usr/bin/paste", &buf);
        stbuf->st_mode = S_IFREG | 0555;
        stbuf->st_nlink = 1;
		stbuf->st_size = buf.st_size;
			is_set[8] = 1;
			memcpy(&stbufs[8], stbuf, sizeof(struct stat));
		}
		else {
			memcpy(stbuf, &stbufs[8], sizeof(struct stat));
		}
	}
    else {
		/* Return -ENOENT if file does not exist */
        res = -ENOENT;
	}

    return res;
}

/* Reading content of directory. Returning predefined values. Check rights while accessing files. */
static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;

    if(strcmp(path, "/") == 0) {
		if (check_rights(O_RDONLY, stbufs[0].st_mode) == 0) {
			filler(buf, ".", NULL, 0);
			filler(buf, "..", NULL, 0);
			filler(buf, bar_path + 1, NULL, 0);
			filler(buf, foo_path + 1, NULL, 0);
			filler(buf, baz_path + 1, NULL, 0);
		}
		else
			return -EACCES;
	}
	else if (strcmp(path, "/bar") == 0) {
		if (check_rights(O_RDONLY, stbufs[1].st_mode) == 0) {
			filler(buf, ".", NULL, 0);
			filler(buf, "..", NULL, 0);
			filler(buf, bin_path + 5, NULL, 0);
		}
		else
			return -EACCES;
	}
	else if (strcmp(path, "/foo") == 0) {
		if (check_rights(O_RDONLY, stbufs[3].st_mode) == 0) {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, test_path + 5, NULL, 0);
		}
		else
			return -EACCES;
	}
	else if (strcmp(path, "/baz") == 0) {
		if (check_rights(O_RDONLY, stbufs[4].st_mode) == 0) {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, example_path + 5, NULL, 0);
		}
		else
			return -EACCES;
	}
	else if (strcmp(path, "/bar/bin") == 0) {
		if (check_rights(O_RDONLY, stbufs[2].st_mode) == 0) {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, readme_path + 9, NULL, 0);
		filler(buf, paste_path + 9, NULL, 0);
		}
		else
			return -EACCES;
	}
	else
        return -ENOENT;

    return 0;
}

/* Checking rights while opening files. */
static int fs_open(const char *path, struct fuse_file_info *fi)
{
	if(strcmp(path, "/") == 0) {
		if(check_rights(fi->flags, stbufs[0].st_mode) != 0)
			return -EACCES;
	}
	else if (strcmp(path, bar_path) == 0) {
		if(check_rights(fi->flags, stbufs[1].st_mode) != 0)
			return -EACCES;
	}
	else if (strcmp(path, bin_path) == 0) {
		if(check_rights(fi->flags, stbufs[2].st_mode) != 0)
			return -EACCES;
	}
	else if (strcmp(path, foo_path) == 0) {
		if(check_rights(fi->flags, stbufs[3].st_mode) != 0)
			return -EACCES;
	}
	else if (strcmp(path, baz_path) == 0) {
		if(check_rights(fi->flags, stbufs[4].st_mode) != 0)
			return -EACCES;
	}
	else if (strcmp(path, example_path) == 0) {
		if(check_rights(fi->flags, stbufs[5].st_mode) != 0)
			return -EACCES;
	}
	else if (strcmp(path, test_path) == 0) {
		if(check_rights(fi->flags, stbufs[6].st_mode) != 0)
			return -EACCES;
	}
	else if (strcmp(path, readme_path) == 0) {
		if(check_rights(fi->flags, stbufs[7].st_mode) != 0)
			return -EACCES;
	}
	else if (strcmp(path, paste_path) == 0) {
		if(check_rights(fi->flags, stbufs[8].st_mode) != 0)
			return -EACCES;

		paste_fd = open("/usr/bin/paste", O_RDONLY);
	}
	else
		return -ENOENT;

	return 0;
}

/* Faking files data here. */
static int fs_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;
    
	const char *str;

	if(strcmp(path, bar_path) == 0) {
	
	}
	else if (strcmp(path, foo_path) == 0) {
	
	}
	else if (strcmp(path, baz_path) == 0) {
	
	}
	else if (strcmp(path, bin_path) == 0) {
	
	}
	else if (strcmp(path, example_path) == 0 || strcmp(path, test_path) == 0 || strcmp(path, readme_path) == 0) {
		if (strcmp(path, example_path) == 0) {
			str = example_str;
		}
		else if (strcmp(path, test_path) == 0) {
			str = test_str;
		}
		else if (strcmp(path, readme_path) == 0) {
			str = readme_str;
		}

		len = strlen(str);
    	if (offset < len) {
        	if (offset + size > len)
            	size = len - offset;
	        memcpy(buf, str + offset, size);
    	}
		else
        	size = 0;
	}
	else if (strcmp(path, paste_path) == 0) {
		int res = read(paste_fd, buf, size);

		return res;
	}
	else
        return -ENOENT;

    return size;
}

/* Our callback function from chmod. We're saving modes in array of stat structures. */
static int fs_chmod(const char *path, mode_t bits)
{
	int res;

	if (strcmp(path, "/") == 0) {
		stbufs[0].st_mode = S_IFDIR | bits;
	}
	else if (strcmp(path, bar_path) == 0) {
		stbufs[1].st_mode = S_IFDIR | bits;
	}
	else if (strcmp(path, bin_path) == 0) {
		stbufs[2].st_mode = S_IFDIR | bits;
	}
	else if (strcmp(path, foo_path) == 0) {
		stbufs[3].st_mode = S_IFDIR | bits;
	}
	else if (strcmp(path, baz_path) == 0) {
		stbufs[4].st_mode = S_IFDIR | bits;
	}
	else if (strcmp(path, example_path) == 0) {
		stbufs[5].st_mode = S_IFREG | bits;
	}
	else if (strcmp(path, test_path) == 0) {
		stbufs[6].st_mode = S_IFREG | bits;
	}
	else if (strcmp(path, readme_path) == 0) {
		stbufs[7].st_mode = S_IFREG | bits;
	}
	else if (strcmp(path, paste_path) == 0) {
		stbufs[8].st_mode = S_IFREG | bits;
	}

	return 0;
}

static int fs_release(const char *path, struct fuse_file_info *fi)
{
	(void) fi;

	if (strcmp(path, paste_path) == 0) {
		close(paste_fd);
	}
}

/* Assigning here our callback functions */
static struct fuse_operations fs_oper = {
    .getattr	= fs_getattr,
	.chmod		= fs_chmod,
    .open		= fs_open,
	.release	= fs_release,
    .read		= fs_read,
    .readdir	= fs_readdir,
};

/* Main function, obvious :-) */
int main(int argc, char *argv[])
{
	/* Calling main fuse function from here.
	 * It mounting fuse filesystem and process
	 * everything with our callback functions
	 */
    return fuse_main(argc, argv, &fs_oper, NULL);
}
