#include "myfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
FILE *fptr = NULL;

int myfs_create(const char *filesystemname, int max_size){
	printf("create\n");
	if(!(fptr = fopen(filesystemname,"wb+"))){
		printf("Open file failed");
		return -1;
	}

	superblock_t superblock = {0};
    inode_t *inode = (inode_t*) calloc((max_size / BLOCK_SIZE), sizeof(inode_t));
	block_t *block = (block_t*) calloc((max_size / BLOCK_SIZE), sizeof(block_t));

    superblock.max_size = max_size;

    superblock.inode_total = max_size / BLOCK_SIZE;
//	printf("superblock:%d",superblock.inode_total);
    superblock.block_total = max_size / BLOCK_SIZE;
    
		
	for(int i=0;i<superblock.inode_total;i++ ){
		inode[i].index = i;
		inode[i].dirty =false;
	}	
	//sizeof change to 
	superblock.inode_count = superblock.inode_total * (sizeof(inode_t));
	//printf("superblock.inode_count:%d\n",superblock.inode_count);
    superblock.block_count = superblock.block_total * BLOCK_SIZE;
	//printf("superblock.block_count:%d\n",superblock.block_count);

    superblock.inode_init = sizeof(superblock_t);
    superblock.block_init = superblock.inode_init + superblock.inode_count;


	fwrite(&superblock, sizeof(superblock_t), 1, fptr);
    fwrite(inode, sizeof(inode_t), superblock.inode_total, fptr);
    fwrite(block, sizeof(block_t), superblock.block_total, fptr);
    //fwrite(inode_bitmap, sizeof(uint), superblock.inode_bitmap_count, fptr);
    //fwrite(block_bitmap, sizeof(uint), superblock.block_bitmap_count, fptr);
    //fclose(fptr);
	
	return 0;
}

int myfs_destroy(const char *filesystemname){
	if(remove(filesystemname)){
		return -1;
	}
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
	//printf("superblock:%d",superblock.inode_total);
	//unsigned int index_value = 0;
	//unsigned int max_index = 0;
	for(int i=0;i<superblock.inode_total;i++ ){
		fread(&inode, sizeof(inode_t), 1, fptr);
		if(inode.dirty){
			//printf("i:%d",i);
			//printf("filename:%s\n",filename);
			//printf("inode.name:%s\n",inode.name);
			if(!strcmp(filename,inode.name)){
				//printf("i:%d",i);
				printf("open file success\n");
				return i;
			}
		}
	}
	printf("The file %s doesn't exist\n",filename);
	return -1;

}
int myfs_file_close(int fd);
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
	//unsigned int index_value = 0;
	//printf("superblock:%d",superblock.inode_total);
	for(int i=0;i<superblock.inode_total;i++ ){
		//printf("i:%d\n",i);
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
		//printf("i:%d\n",i);
		fread(&inode, sizeof(inode_t), 1, fptr);
		//printf("inode:%d\n",inode.dirty);
		if(!inode.dirty){
			//printf("i will return 0\n");
			inode.dirty = true;
			//printf("inode:%d\n",inode.dirty);
			strncpy(inode.name,filename,strlen(filename));
	//		fseek(fptr, superblock.block_init+sizeof(block_t)*i, SEEK_SET );
	//fseek(fptr, superblock.block_init+BLOCK_SIZE*i, SEEK_SET );
	//printf("i:%d fptr:%p\n",i,fptr);
			inode.block_offset=superblock.block_init+BLOCK_SIZE*i;
			//inode.curfptr = fptr;
	//printf("fptr:%p\n",inode.curfptr);
			printf("create file success\n");
		//	fseek(fptr,-sizeof(inode_t),SEEK_CUR);
			fseek(fptr, superblock.inode_init+sizeof(inode_t)*i,SEEK_SET);
    		fwrite(&inode, sizeof(inode_t), 1, fptr);
			return 0;
		}
	}
	printf("disk is full\n");
	return -1;

}
int myfs_file_delete(const char *filename);
int myfs_file_read(int fd, char *buffer, int count){
	char buf[BLOCK_SIZE];
	fseek(fptr,0,SEEK_SET);
	superblock_t superblock = {0};
	fread(&superblock, sizeof(superblock_t), 1, fptr);
	//printf("i still live\n");
	//printf("fd:%d\n",fd);
	//printf("superblock.block_init:%d\n",superblock.block_init);
	//fseek(fptr, superblock.inode_count, SEEK_CUR);
	fseek(fptr, superblock.inode_count+sizeof(block_t)*fd, SEEK_CUR);
	//fseek(fptr,BLOCK_SIZE*fd, superblock.block_init );
    //inode_t inode ;
	//fseek(fptr,sizeof(inode_t)*fd, superblock.inode_init );
	//fread(&inode, sizeof(inode_t), 1, fptr);
	//fseek(fptr,sizeof(inode_t)*fd, superblock.inode_init );
	
	//fptr=inode.curfptr;
	//printf("i still live\n");
	fread(buf, sizeof(char), count, fptr);
	//printf("buffer:%s",buf);
	//inode.curfptr=fptr;
	strncpy(buffer,buf,strlen(buf));
	printf("read file success\n");
	return 0;

}
int myfs_file_write(int fd, char *buffer, int count){
	fseek(fptr,0,SEEK_SET);
	superblock_t superblock = {0};
    inode_t inode ;
	fread(&superblock, sizeof(superblock_t), 1, fptr);
	//fseek(fptr,sizeof(inode_t)*fd, superblock.inode_init );
	fseek(fptr,superblock.inode_init+sizeof(inode_t)*fd, SEEK_SET );
	fread(&inode, sizeof(inode_t), 1, fptr);
	//fseek(fptr, superblock.block_init+BLOCK_SIZE*fd, SEEK_SET );
	fseek(fptr, inode.block_offset, SEEK_SET );
//	printf("fd:%d fptr:%p inode:%p",fd,fptr,inode.curfptr);
	//fptr=inode.curfptr;
	//printf("fptr:%p",fptr);
	fwrite(buffer, sizeof(char), count, fptr);
	inode.block_offset += count*sizeof(char);
	//inode.curfptr=fptr;
	fseek(fptr,superblock.inode_init+sizeof(inode_t)*fd, SEEK_SET );
    fwrite(&inode, sizeof(inode_t), 1, fptr);
	printf("write file success\n");
	return 0;
}
