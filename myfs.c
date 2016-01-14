#include "myfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
FILE *fptr = NULL;

int myfs_create(const char *filesystemname, int max_size){
	if(!(fptr = fopen(filesystemname,"wb+"))){
		printf("Open file failed");
		return -1;
	}

	superblock_t superblock = {0};
    inode_t *inode = (inode_t*) calloc((max_size / BLOCK_SIZE), sizeof(inode_t));
	block_t *block = (block_t*) calloc((max_size / BLOCK_SIZE), sizeof(block_t));

    superblock.max_size = max_size;

    superblock.inode_total = max_size / BLOCK_SIZE;
    superblock.block_total = max_size / BLOCK_SIZE;
    
		
	for(int i=0;i<superblock.inode_total;i++ ){
		inode[i].index = i;
		inode[i].dirty =false;
	}	
	superblock.inode_count = superblock.inode_total * (sizeof(inode_t));
    superblock.block_count = superblock.block_total * BLOCK_SIZE;

    superblock.inode_init = sizeof(superblock_t);
    superblock.block_init = superblock.inode_init + superblock.inode_count;


	fwrite(&superblock, sizeof(superblock_t), 1, fptr);
    fwrite(inode, sizeof(inode_t), superblock.inode_total, fptr);
    fwrite(block, sizeof(block_t), superblock.block_total, fptr);
	printf("Create file system successful\n");
	
	return 0;
}

int myfs_destroy(const char *filesystemname){
	if(remove(filesystemname)){
		return -1;
	}
	printf("Destroy file system successful\n");
	return 0;
}



int myfs_file_open(const char *filename){
	printf("file open\n");
	if(!fptr){
		printf("err");
		return -1;
	}
	fseek(fptr,0,SEEK_SET);
	superblock_t superblock = {0};
    inode_t inode ;
	fread(&superblock, sizeof(superblock_t), 1, fptr);
	for(int i=0;i<superblock.inode_total;i++ ){
		fread(&inode, sizeof(inode_t), 1, fptr);
		if(inode.dirty){
			if(!strcmp(filename,inode.name)){
				printf("open file success\n");
				return i;
			}
		}
	}
	printf("The file %s doesn't exist\n",filename);
	return -1;

}
int myfs_file_close(int fd){
	printf("file close\n");
	if(!fptr){
		printf("err");
		return -1;
	}
	fseek(fptr,0,SEEK_SET);
	superblock_t superblock = {0};
    inode_t inode ;
	fread(&superblock, sizeof(superblock_t)+sizeof(inode_t)*fd, 1, fptr);
	fread(&inode, sizeof(inode_t), 1, fptr);
	if(!inode.dirty){
		printf("File have not been opend\n");
		return -1;
	}
	else
		return 0;
}
int myfs_file_create(const char *filename){
	printf("file create\n");
	if(!fptr){
		printf("err\n");
		return -1;
	}
	fseek(fptr,0,SEEK_SET);
	superblock_t superblock = {0};
    inode_t inode ;
	fread(&superblock, sizeof(superblock_t), 1, fptr);
	for(int i=0;i<superblock.inode_total;i++ ){
		fread(&inode, sizeof(inode_t), 1, fptr);
		if(inode.dirty){
			if(!strcmp(filename,inode.name)){
				printf("file %s is exist!\n",filename);
				return -1;
			}
		}
	}
	fseek(fptr,superblock.inode_init,SEEK_SET);
	for(int i=0;i<superblock.inode_total;i++ ){
		fread(&inode, sizeof(inode_t), 1, fptr);
		if(!inode.dirty){
			inode.dirty = true;
			strncpy(inode.name,filename,strlen(filename));
			inode.block_offset=superblock.block_init+BLOCK_SIZE*i;
			printf("create file success\n");
			fseek(fptr, superblock.inode_init+sizeof(inode_t)*i,SEEK_SET);
    		fwrite(&inode, sizeof(inode_t), 1, fptr);
			return 0;
		}
	}
	printf("disk is full\n");
	return -1;

}
int myfs_file_delete(const char *filename){
	printf("file delete\n");
	if(!fptr){
		printf("err\n");
		return -1;
	}
	char buf[BLOCK_SIZE]={};
	fseek(fptr,0,SEEK_SET);
	superblock_t superblock = {0};
    inode_t inode ;
	fread(&superblock, sizeof(superblock_t), 1, fptr);
	for(int i=0;i<superblock.inode_total;i++ ){
		fread(&inode, sizeof(inode_t), 1, fptr);
		if(inode.dirty){
			if(!strcmp(filename,inode.name)){
				fseek(fptr,superblock.block_init+i*sizeof(block_t),SEEK_SET);
				fwrite(buf, sizeof(char), BLOCK_SIZE, fptr);
				inode.dirty = false;
				strncpy(inode.name,"",0);
				inode.block_offset=0;
				fseek(fptr,superblock.inode_init+sizeof(inode_t)*i, SEEK_SET );
			    fwrite(&inode, sizeof(inode_t), 1, fptr);
				return 0;
			}
		}
	}

}
int myfs_file_read(int fd, char *buffer, int count){
	char buf[BLOCK_SIZE];
	fseek(fptr,0,SEEK_SET);
	superblock_t superblock = {0};
	fread(&superblock, sizeof(superblock_t), 1, fptr);
	fseek(fptr, superblock.inode_count+sizeof(block_t)*fd, SEEK_CUR);
	fread(buf, sizeof(char), count, fptr);
	strncpy(buffer,buf,strlen(buf));
	printf("read file success\n");
	return 0;

}
int myfs_file_write(int fd, char *buffer, int count){
	fseek(fptr,0,SEEK_SET);
	superblock_t superblock = {0};
    inode_t inode ;
	fread(&superblock, sizeof(superblock_t), 1, fptr);
	fseek(fptr,superblock.inode_init+sizeof(inode_t)*fd, SEEK_SET );
	fread(&inode, sizeof(inode_t), 1, fptr);
	fseek(fptr, inode.block_offset, SEEK_SET );
	fwrite(buffer, sizeof(char), count, fptr);
	inode.block_offset += count*sizeof(char);
	fseek(fptr,superblock.inode_init+sizeof(inode_t)*fd, SEEK_SET );
    fwrite(&inode, sizeof(inode_t), 1, fptr);
	printf("write file success\n");
	return 0;
}
