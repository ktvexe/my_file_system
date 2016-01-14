#include <stdio.h>
#include <string.h>
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
	char choose[2];
	int fd;
	do{
		printf("Welcome to my file system\n");
		printf("==========================\n");
		printf("There are some option you can choose below:\n");
		printf("############################\n(Cs)reate_fs\n(D)estroy_fs\n(O)pen_file\n(Cf)lose_file\n(c)reate_file\n(d)elete_file\n(R)ead_file\n(W)rite_file\n(E)xit");
		printf("############################\nWhich do you want to choose?(Cs,D,O,Cf,c,d,R,W or E )\n");
		scanf("%s",choose);
		if(!strcmp(choose,"Cs")){
			char filesystem_name[256];
			int size;
			printf("You choose file system create\n");
			printf("Please input your file system name\n");
			scanf("%s",filesystem_name);
			printf("Please input your file system size (total number of files)\n");
			scanf("%d",&size);
			size *= 4096;
			printf("size:%d\n",size);
			myfs_create(filesystem_name, size);
			printf("\n\n\n");
		}
		if(!strcmp(choose,"D")){
			char filesystem_name[256];
			printf("You choose file system destroy\n");
			printf("Please input your file system name\n");
			scanf("%s",filesystem_name);
			myfs_destroy(filesystem_name);
			printf("\n\n\n");

		}
		if(!strcmp(choose,"O")){
			char file_name[256];
			printf("You choose open file \n");
			printf("Please input your file name\n");
			scanf("%s",file_name);
			//fd=myfs_file_open("try");
			fd=myfs_file_open(file_name);
			printf("\n\n\n");

		}
		if(!strcmp(choose,"Cf")){
			printf("You choose close file \n");
			myfs_file_close(fd);
			printf("\n\n\n");

		}
		if(!strcmp(choose,"c")){
			char file_name[256];
			printf("You choose create file \n");
			printf("Please input your file name\n");
			scanf("%s",file_name);
			myfs_file_create(file_name);	
			printf("\n\n\n");

		}
		if(!strcmp(choose,"d")){
			char file_name[256];
			printf("You choose delete file \n");
			printf("Please input your file name\n");
			scanf("%s",file_name);
			myfs_file_delete(file_name);
			printf("\n\n\n");

		}
		if(!strcmp(choose,"R")){
			char content[4096];
			printf("You choose read file \n");
			myfs_file_read( fd, content, strlen(content));
			printf("%s\n",content);
			printf("\n\n\n");

		}
		if(!strcmp(choose,"W")){
			char content[256];
			printf("You choose write file \n");
			printf("Please input your contents\n");
			scanf("%s",content);
			//scanf("%[^\n]",content);
			myfs_file_write( fd, content, strlen(content));
			printf("\n\n\n");

		}
		if(!strcmp(choose,"E")){
			printf("Goodbye!!\n");
			break;
		}
	}while(1);
	
	/*
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
	*/
	return 0;
}
