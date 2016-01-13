#include <stdio.h>
#include "myfs.h"

int myfs_create(const char *filesystem_name, int max_size);
int myfs_destroy(const char *filesystem_name);

int myfs_file_open(const char *filename);
int myfs_file_close(int fd);
int myfs_file_create(const char *filename);
int myfs_file_delete(const char *filename);
int myfs_file_read(int fd, char *buffer, int count);
int myfs_file_write(int fd, char *buffer, int count);

int main (int argc,char* argv[]){
	myfs_create("test", 4096*8);
	myfs_file_create("try");	
	myfs_file_create("try1");	
	myfs_file_create("try2");	
	int fd=myfs_file_open("try");
	printf("fd:%d\n",fd);
	char* buf ="123456789012345";
	char buf1[30];
	myfs_file_write( fd, buf, 10);
	myfs_file_read( fd, buf1, 10);
	printf("buf:%s\n",buf);
	printf("buf1:%s\n",buf1);

	char* buf3 ="abcdefghijklmnop";
	myfs_file_write( fd, buf3, 10);
	myfs_file_read( fd, buf1, 20);
	printf("buf:%s\n",buf);
	printf("buf1:%s\n",buf1);
	
	myfs_file_close(fd);
	myfs_file_delete("try");
		
	//myfs_destroy(test);
	return 0;
}
