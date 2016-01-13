#ifndef MYFS_H
#define MYFS_H

#include <stdbool.h>
#include <stdio.h>
#define BLOCK_SIZE 4096

typedef struct {
    unsigned int max_size;
    unsigned int unused_size;

    unsigned int inode_init;
    unsigned int inode_count;
    //unsigned int inode_offset;
    unsigned int inode_total;
   // unsigned int inode_unused;

    unsigned int block_init;
    unsigned int block_count;
    //unsigned int block_offset;
    unsigned int block_total;
    //unsigned int block_unused;
}superblock_t;

/* Inode Structure */
typedef struct {
	unsigned int index;
    bool dirty;
	FILE* curfptr;
    unsigned int block_offset;
    //unsigned int block_count;
    //unsigned int block[16];
    char name[256];
}inode_t;

typedef struct {
	char buffer[BLOCK_SIZE];
}block_t;

int myfs_create(const char *filesystem_name, int max_size);
int myfs_destroy(const char *filesystem_name);

int myfs_file_open(const char *filename);
int myfs_file_close(int fd);
int myfs_file_create(const char *filename);
int myfs_file_delete(const char *filename);
int myfs_file_read(int fd, char *buffer, int count);
int myfs_file_write(int fd, char *buffer, int count);


#endif
