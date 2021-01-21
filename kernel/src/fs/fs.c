#include "common.h"
#include <sys/ioctl.h>
#include <string.h>

int fs_open(const char *pathname, int flags);
int fs_read(int fd, void *buf, int len);
int fs_write(int fd, void *buf, int len);
int fs_lseek(int fd, int offest, int whence);
int fs_close(int fd);

typedef struct
{
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum
{
	SEEK_SET,
	SEEK_CUR,
	SEEK_END
};

/* This is the information about all files in disk. */
static const file_info file_table[] __attribute__((used)) = {
	{"1.rpg", 188864, 1048576},
	{"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304},
	{"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032},
	{"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460},
	{"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582},
	{"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673},
	{"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367},
	{"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267},
	{"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941},
	{"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713},
	{"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989},
	{"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037},
	{"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

struct Fstate
{
	bool opened;
	uint32_t offest;
} fstate[NR_FILES + 3];

int fs_ioctl(int fd, uint32_t request, void *p)
{
	assert(request == TCGETS);
	return (fd >= 0 && fd <= 2 ? 0 : -1);
}

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */

int fs_open(const char *pathname, int flags)
{
	int i;
	for (i = 0; i < NR_FILES; i++)
	{
		if (strcmp(file_table[i].name, pathname) == 0)
		{
			fstate[i + 3].offest = 0;
			fstate[i + 3].opened = true;
			return i + 3;
		}
	}
	assert(0);
	return 0;
}
int fs_read(int fd, void *buf, int len)
{
	if (!fstate[fd].opened)
		return -1;
	int rlen;
	rlen = (file_table[fd - 3].size - fstate[fd].offest >= len ? len : file_table[fd - 3].size - fstate[fd].offest);
	ide_read(buf, file_table[fd - 3].disk_offset + fstate[fd].offest, rlen);
	fstate[fd].offest += rlen;
	return rlen;
}
void serial_printc(char);
int fs_write(int fd, void *buf, int len)
{
	int i, rlen;
	if (fd == 1 || fd == 2)
	{
		for (i = 0; i < len; i++)
		{
			serial_printc(*(char *)(buf + i));
		}
		return len;
	}
	if (!fstate[fd].opened)
		return -1;
	rlen = (file_table[fd - 3].size - fstate[fd].offest >= len ? len : file_table[fd - 3].size - fstate[fd].offest);
	ide_write(buf, file_table[fd - 3].disk_offset + fstate[fd].offest, rlen);
	fstate[fd].offest += rlen;
	return rlen;
}
int fs_lseek(int fd, int offest, int whence)
{
	if (!fstate[fd].opened)
		return -1;
	if (whence == SEEK_SET)
	{
		fstate[fd].offest = offest;
	}
	else if (whence == SEEK_CUR)
	{
		fstate[fd].offest += offest;
	}
	else if (whence == SEEK_END)
	{
		fstate[fd].offest = file_table[fd - 3].size - offest;
	}
	return fstate[fd].offest;
}
int fs_close(int fd)
{
	fstate[fd].opened = false;
	return 0;
}