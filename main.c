#include <stdio.h>
#include "myfs.h"

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
	return 0;
}
