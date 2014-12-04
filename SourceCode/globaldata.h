
#include "blockio.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Super block.
 */

typedef struct
{
	unsigned short ID;
	unsigned char free[64];
	unsigned char pad[62];
}SuperBlock;

/*
 *  File Control Block.
 */
typedef struct
{
	unsigned char type;
	char name[7];
	char pad[120];
}FCB;

/*
 *  Directory Entry
 */

typedef struct
{
	unsigned char ID;
	char name[7];
}DirectoryEntry;

/*
 *  Directory Block
 */

typedef struct
{
	DirectoryEntry entry[16];
}DirBlock;

/*
 *  Open File Reference
 */
typedef struct
{
	int type,pos,size,index;
	int dataIndex[8];
}OpenFileRef;


#define SFS_FAILED(a) (a == -1)
#define SFS_DEBUG
#define SFS_ID 0xABCD  					   //file system ID
#define NUM_BLOCKS 512 					   //number of blocks on disk
#define BLOCK_SIZE 128                     //size of each block (bytes)
#define FIRST_DATA_BLOCK 9                 //index of fist data block on disk
#define MAX_FILES 64	                   //max files allowed on disk
#define MAXOPEN_FILES 4	                   //max open files allowed by system
#define FILE_TYPE_REG  0	               //for regular files
#define FILETYPE_DIR  1	                   //for directories
#define FILETYPE_NONE 2	                   //null type
#define MAX_FILE_SIZE 1024	               //max size allowed for files (bytes)

int isBlockFree(int blocknum);
int setBlock_asUsed(int blockNum, int used);
int writeFCB(int index);
int NewBlock_alloc();
void freeMem(void *data, int size);
int startNewDisk();
int singleFile(const char *pathName);
int validatePath(const char *pathName);
int fileName_parse(const char *pathName,char *fileName);
int getChildIndex_dir(int index,const char *fileName);
int makeNewFile(int parentIndex,int type,const char *name);
int findIndex_file(char *pathName);
int readFile(int fd,void *voidData,int size);
int fileExpand(int fd, int size);
int writeFile(int fd,void *voidData,int size);
int dirChildIndex_remove(int index,const char *fileName);

static const char *error_string=NULL;
short int InodeTable[(MAX_FILE_SIZE>>7)][MAX_FILES];


SuperBlock SuperBlock0;
FCB FILE_CONTROL_BLOCK0[MAX_FILES];
OpenFileRef OpenFileReferance[MAXOPEN_FILES];




