#include "globaldata.h"

/*************************************************************************
 * sfs_read(int fd,int start,int length,char *mem_pointer)
 * The sfs_read function is used to read a desinaged file from the disk.
 * The data of the file is stored in the memory specified by *mem_pointer.
 *
 *	fd - the file descriptor of the file to be read
 *	stat - The starting byte of the file 
 *	length - The length of the area to be read in bytes 
 *	*mem_pointer - where name components will be stored for the diectory in 
 *		       question
 * Successful
 *	return  1 : the file has been read sucessfully
 * Error Return
 *	return -1 : Can not read data from file because wrong file type
 *	return -2 : The data can not be read
 **************************************************************************/
int sfs_read(int fd,int start,int length,char *mem_pointer)
{
	/* Checks to see that the file trying to be read by the following
	 * fd number has got a file type of a regular file.
	 */
	if(OpenFileReferance[fd].type != FILE_TYPE_REG)
	{
		fprintf(stdout,"Cannot read data from file!\n");

		return -1;
	}
	
	/* Setting the start that was entered by the user in to the unsigned
	 * position variable that is desinated by the OpenFileRef structure
	 */
	OpenFileReferance[fd].pos = start;

	/* If the SFS_FAILED function retrives an -1 that means that the readFile
	 * function has returned an error which referes to the fact that the data
	 * can not be read
	 */
	if(SFS_FAILED(readFile(fd,mem_pointer,length)))
	{
		fprintf(stdout,"Data read failure!\n");

		return -2;
	}
/* If the readFile returns 1 then the following will be outputed with the following
 * information like file descripter, start and length of the file
 */
#ifdef SFS_DEBUG
    fprintf(stdout,"Read file: %d [%d %d] successful!\n",fd,start,length);
#endif

	return 1;
}


/*************************************************************************
 * readFile(int fd,void *voidData,int size)
 * The readFile function is used to read data from file that are already open
 * Successful
 *		return 1 : the data was sucessfull read from the open file\
 * Return Errors
 * 		return -1 : size of the file is too starting of at the wrong position
 **************************************************************************/
int readFile(int fd,void *voidData,int size)
{
  int blockIndex = OpenFileReferance[fd].pos/BLOCK_SIZE;
  int pos = OpenFileReferance[fd].pos%BLOCK_SIZE;
  int i;
  unsigned char block[BLOCK_SIZE], *data=(unsigned char*)voidData;
  /* An error will be outputed if the open files position+size is
   * greater then the open file sizes.
   */
  if(OpenFileReferance[fd].pos+size > OpenFileReferance[fd].size)
  {
	  return -1;
  }
  // Retrieving the block where the the desired file is stored
  get_block(OpenFileReferance[fd].dataIndex[blockIndex],(char*)block);
  /* This for loop will run till all of the data has been read and
   * stored in to the data pointer, size refers to the length
   * of the characters that were inputed buy the user
   */
  for(i=0; i < size; i++)
  {
    data[i] = block[pos];

    pos++;
    /* is the position is greater then or equal to the BLOCK_SIZE this
     * means that there will need to be use of another block to store
     * the extra data
     */
    if(pos >= BLOCK_SIZE)
    {
        // set the position back to 0 as it is a new block where the data will be store
        pos = 0;
        // Increment the blockIndex
        blockIndex++;
        // Retrieve the new block that is available for the data to be stored on
        get_block(OpenFileReferance[fd].dataIndex[blockIndex],(char*)block);
    }
  }
  // Setting the position of the read file to be the following
  OpenFileReferance[fd].pos = BLOCK_SIZE*blockIndex+pos;

  return 1;
}
