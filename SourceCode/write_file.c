#include "globaldata.h"

/*************************************************************************
 * sfs_read(int fd,int start,int length,char *mem_pointer)
 * The sfs_read function is used to read a desinaged file from the disk.
 * The data is store on memory 
 *
 *	fd - the file descriptor of the file to be read
 *	stat - The starting byte of the file 
 *	length - The length of the area to be read in bytes 
 *	*mem_pointer - used to store the length bytes of data to the memory 
 *		       location
 * Successful
 *	return  1 : the file has been read sucessfully
 * Error Return
 *	return -1 : if the file type is not regular then data can't be written
 *	return -2 : File could not be expanded 
 *	return -3 : writing to the disk has failed
 **************************************************************************/
int sfs_write(int fd,int start,int length,char *mem_pointer)
{
	/* Checks to see that the file trying to be written by the following
	 * fd number has got a file type of a regular if not return error
	 */
	if(OpenFileReferance[fd].type != FILE_TYPE_REG)
	{
		fprintf(stdout,"Cannot write data!\n");

		return -1;
	}
	/* If the start value entered by the user is greater then 0 then that start
	 * value is start value is saved in the position variable in the
	 * OpenFileRef structure
	 */
	if(start >= 0)
	{
		OpenFileReferance[fd].pos = start;
	}
	else
	{
		// Store the value in position to the size of the structure
		OpenFileReferance[fd].pos = OpenFileReferance[fd].size;
		/* If the SFS_FAILED function retrives an -1 that means that the
		 * fileExpand function has resulted in an error
		 */
		if(SFS_FAILED(fileExpand(fd,OpenFileReferance[fd].size+length)))
		{
			fprintf(stdout,"File expansion failed\n");

			return -2;
		}
	}
	/* If the SFS_FAILED function retrives an -1 that means that the writeFile
	 * function has returned an error which referes to the fact that the data
	 * can not be written
	 */
	if(SFS_FAILED(writeFile(fd,mem_pointer,length)))
	{
		fprintf(stdout,"Write failure!\n");

		return -3;
	}
/* If the writeFile returns 1 then the following will be outputed with the following
 * information like file descripter, start and length of the file
 */
#ifdef SFS_DEBUG
    fprintf(stdout,"Write file: %d [%d %d] successful!\n",fd,start,length);
#endif

	return 1;
}


/*************************************************************************
 * writeFile(int fd,void *voidData,int size)
 * The writeFile function is used to write data on a disk
 * Successful
 *		return 1 : the data was sucessfull written to a file on the disk
 **************************************************************************/
 int writeFile(int fd,void *voidData,int size)
{
  int blockIndex = OpenFileReferance[fd].pos/BLOCK_SIZE;
  int pos = OpenFileReferance[fd].pos%BLOCK_SIZE;
  int i;
  unsigned char block[BLOCK_SIZE],*data = (unsigned char*)voidData;

  // Retrieving the block where the the desired file is stored
  get_block(OpenFileReferance[fd].dataIndex[blockIndex],(char*)block);

  /* This for loop will run till all of the data that will be written and
   * stored in to the data pointer, size refers to the length
   * of the characters that were inputed buy the user
   */
  for(i=0; i < size; i++)
  {
    block[pos] = data[i];

    pos++;

    /* If the position is greater then or equal to the BLOCK_SIZE this
     * means that there will need to be use of another block to store
     * the extra data
     */
    if(pos >= BLOCK_SIZE)
    {
      // put the data on to the block that is ready
      put_block(OpenFileReferance[fd].dataIndex[blockIndex],(char*)block);

      // set the position back to 0 as it is a new block where the data will be store
      pos=0;
      // Increment the blockIndex
      blockIndex++;
      // retrieve the new block that is available for the data to be stored on
      get_block(OpenFileReferance[fd].dataIndex[blockIndex],(char*)block);
    }
  }
  // put the data on to the block that is ready
  put_block(OpenFileReferance[fd].dataIndex[blockIndex],(char*)block);
  // setting the position of the read file to be the following
  OpenFileReferance[fd].pos = BLOCK_SIZE*blockIndex+pos;

  return 1;
}
