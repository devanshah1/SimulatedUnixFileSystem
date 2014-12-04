#include "globaldata.h"

/*************************************************************************
  * singleFile(const char *pathName)
  * The singleFile function is used to check if the pathName is a single file
  *
  * Successful
  *	return 1 : To show that the pathName is a single file
  **************************************************************************/
int singleFile(const char *pathName)
{
  int i=0;
  /* The while loop will run as long as the entire pathName
   * been checked
   */
  while(pathName[i] != 0)
  {
    if(pathName[i] == '/')
    {
      if(pathName[i+1] != 0)
      {
    	  return 0;
      }
    }

    i++;
  }
  return 1;
}


/*************************************************************************
  * fileName_parse(const char *pathName,char *fileName)
  * The fileName_parse function is used to make sure that the path that is passed
  * through to the validatePath function is in the correct format.
  *
  * Successful
  *		return i : returning the current value of i to specify how many
  *				   element are in the going to be the fileName
  **************************************************************************/
int fileName_parse(const char *pathName,char *fileName)
{
 int i=0;
 /* Run the loop as long as the designated pointer element is
  * not equal to null and "/", the loop is used to store the
  * pathName elements into the fileName pointer
  */
 while(pathName[i] != '\0'&&pathName[i] != '/')
 {
	// Setting each of the pathName elements to the specified fileName element
   fileName[i] = pathName[i];
   i++;
 }
 // Storing null in the last element of the fileName pointer
 fileName[i] = '\0';

 if(pathName[i] == '/')
 {
	  i++;
 }

 return i;
}


/*************************************************************************
 * findIndex_file(char *pathName)
 * The findIndex_file function is used to retrieve the index of the pathName
 * Successful
 *		return index : returning the index of the give pathName
 * Return Errors
 * 		return -1 : the pathName entered was not valid
 *		return -2 : the Directory is invalid
 *		return -3 : the directory could not be found
 **************************************************************************/
int findIndex_file(char *pathName)
{
  int index=0;
  char fileName[7]="";
  /* Checks to see that the pathname that was inputed by the user is valid
   * the pathname is also passed to the function validatePath and if not valid
   * then return -1, which refers to an invalid path name
   */
  if(!pathName || !validatePath(pathName))
  {
    fprintf(stdout,"Path name invalid!\n");

    return -1;
  }

  // Increment the pathName to the next character
  pathName++;
  /* This while loop will continue to run until the loop reaches a
   * break only if the break is activated
   */
  while(1)
  {
	// If the first element in the pathName is null then break the while loop
    if(pathName[0] == '\0')
    {
    	break;
    }

	/* Retrieve the type of the file from the specified FCB and
	 * compare it to the constant value FILETYPE_DIR if they
	 * are not equal then return an error for directory invalid
	 */
	if(FILE_CONTROL_BLOCK0[index].type != FILETYPE_DIR){
		fprintf(stdout,"Directory invalid!\n");

		return -2;
	}

	// PathName is incremented according to the return from the fileName_parse function
	pathName += fileName_parse(pathName,fileName);
	// Setting the value that was retrieved from getChildIndex_dir function to index
	index = getChildIndex_dir(index,fileName);
	/* If the index that was retrieves is less then 0 then that means
	 * that there is an error being sent from the getChildIndex
	 * function, return -3 refers to directory was not found
	 */
	if(index < 0)
	{
		fprintf(stdout,"Directory \"%s\" not found\n",fileName);

		return -3;
	}
  }

  return index;
}



/*************************************************************************
 * getChildIndex_dir(int index,const char *fileName)
 * The getChildIndex_dir function is used to retrieve the index of the child
 * of the directory
 * Successful
 *		return i : returning the current value of i to specify how many
 *				   element are in the going to be the fileName
 **************************************************************************/
int getChildIndex_dir(int index,const char *fileName)
{
  int i,j;
  DirBlock dirDataBlock;

  /* This for loop will run through the the entire INodeTable which
   * is of length 8
   */
  for(j=0; j < 8; j++)
  {
	/* If in the InodeTable there is a null value in there the
	 * the for loop will exit
	 */
    if(InodeTable[j][index] == 0)
    {
    	break;
    }

    /* Retrieves either success or false from the get_block
     * the values that are passed through to the get_block function
     * are specified InodeTable value and also the DirBlock as a buffer
     */
    get_block(InodeTable[j][index],(char*)&dirDataBlock);

    /* For loop used to check all the entries that are present
     * in the DirBlock
     */
    for(i=0; i < 16; i++)
    {
      /* Check all the entries that are present in the DirBlock to
       * and if the ID of any of the entries are equal to 0 then
       * continue
       */
      if(dirDataBlock.entry[i].ID == 0)
      {
    	  continue;
      }
      /* If the fileName and the name entry that is store in the
       * Directory Block are equal the same that return the ID
       */
      if(strcmp(fileName,dirDataBlock.entry[i].name) == 0)
      {
    	/* Casting the value that is retrieved from the DirBlock structure
    	 * so that the return value is an integer not a unsigned value
    	 */
        return (int)dirDataBlock.entry[i].ID;
      }
    }
  }

  return -1;
}


/*************************************************************************
  * validatePath(const char *pathName)
  * The validatePath function is used to make sure that the path that is passed
  * through to the validatePath function is in the correct format.
  *
  * Successful
  *		return 1 : pathName is valid
  *		return 0 : pathName is not valid
  **************************************************************************/
 int validatePath(const char *pathName)
{
  int i,j=1;
  // Checks the first pointer in in pathName to make sure that it is a "/"
  if(pathName[0] == '/')
  {
	/* Once the first element of the pathName has been check and is true
	 * then the wile loop will be called into action and will continue
	 * to run as long as the pathName element is not null
	 */
    while(pathName[j] != 0)
    {
      /* If the pathName element in question is "/" and also the pathName
       * element previous is equal to "/" then return a 0
       */
      if(pathName[j] == '/' && pathName[j-1] == '/')
      {
    	  return 0;
      }

      /* Run the for loop until all of the pathname element have been tested
       */
      for(i=1; i <= 6; i++)
      {
        j++;
        // Element that are after the first element are equal to "/" break loop
        if(pathName[j] == '/')
        {
        	break;
        }
        /* If the element is equal to null then return 0 as all element have been
         * checked
         */
        if(pathName[j] == 0)
        {
        	return 1;
       	}
      }

      if(pathName[j] != 0 && pathName[j] != '/')
      {
    	  return 0;
      }

      j++;
    }

    return 1;
  }
  // Return 0 if there first element is anything else except for "/"
  else return 0;
}

 /*************************************************************************
  * dirChildIndex_remove(int index,const char *fileName)
  * The dirChildIndex_remove function is used to remove a child's index from
  * the parent directory
  * Successful
  *		return ID : returning the index of the give pathName
  * Return Errors
		return -1 : If the child's Index can not be removed from the directory
  **************************************************************************/
 int dirChildIndex_remove(int index,const char *fileName)
{
  int i,j,ID;
  DirBlock dirDataBlock;

    for(j=0; j < 4; j++)
    {
  	/* If in the InodeTable there is a null value in there the
  	 * the for loop will exit
  	 */
      if(InodeTable[j][index] == 0)
      {
      	break;
      }

      /* Retrieves either success or false from the get_block
       * the values that are passed through to the get_block function
       * are specified InodeTable value and also the DirBlock as a buffer
       */
      get_block(InodeTable[j][index],(char*)&dirDataBlock);

      /* For loop used to check all the entries that are present
       * in the DirBlock
       */
      for(i=0; i < 16; i++)
      {
        /* Check all the entries that are present in the DirBlock to
         * and if the ID of any of the entries are equal to 0 then
         * continue
         */
        if(dirDataBlock.entry[i].ID == 0)
        {
      	  continue;
        }
        /* If the fileName and the name entry that is store in the
         * Directory Block are equal the same that return the ID
         */

      if(strcmp(fileName,dirDataBlock.entry[i].name) == 0)
      {
    	/* Setting the ID variable equal to the ID that is present in the
    	 * DirBlock entry
    	 */
        ID = dirDataBlock.entry[i].ID;
        // Know setting the DirBlock entry ID to 0 as it was removed
        dirDataBlock.entry[i].ID = 0;
        // put the new DirBlock on to the disk with updated information
        put_block(InodeTable[j][index],(char*)&dirDataBlock);

        return ID;
      }
    }
  }

  return -1;
}


 /*************************************************************************
  * fileExpand(int fd,int size)
  * The fileExpand function is used to expand a file to make more room for
  * the data entries.
  *		return 1 : sucessfull expanded the file
  * Return Errors
  * 		return -1 : File Expansion failed
  **************************************************************************/
 int fileExpand(int fd,int size)
 {
   size = (size+BLOCK_SIZE-1)/BLOCK_SIZE;
   // return error if the size is greater then 128
   if(size > 128)
   {
 	  return -1;
   }
   /* This while loop will run as long as the size is greater then 0,
    * the size is calculated above
    */
   while(size > 0)
   {
 	/* If the dataIndex of the open file is equal to 0 then
 	 */
     if(OpenFileReferance[fd].dataIndex[size] == 0)
     {
       /* Setting the dataIndex of the file to the value that is retried from the
        * function NewBlock_alloc
        */
       OpenFileReferance[fd].dataIndex[size] = NewBlock_alloc();
       OpenFileReferance[fd].size += BLOCK_SIZE;
       // Setting the INodetable reference to the dataIndex
       InodeTable[size][OpenFileReferance[fd].index] = OpenFileReferance[fd].dataIndex[size];
       // Write the new index to the disk
       writeFCB(OpenFileReferance[fd].index);
     }

     size--;
   }

   return 1;
 }

 /*************************************************************************
  * freeMem(void *data,int size)
  * The freeMem function is used to free up memory for the new blocks that
  * exits on the disk.
  **************************************************************************/
 void freeMem(void *data, int size)
 {
   // Casting data to char* type
   char *c = (char*)data;
   int i;

   /* Runs through the loop setting all of the pointer values to 0
    */
   for(i=0; i < size; i++)
   {
 	  c[i] = 0;
   }
 }

 /*************************************************************************
  * NewBlock_alloc()
  * The NewBlock_alloc function is used to allocate memory for a new block
  * using the functions isBlockFree and setBlock_asUsed (more information
  * available about functions above).
  *
  * Successful
  *	return all blocks have been allocated
  * Error return
  * 	return -1 : Error if the free block can not be allocated
  **************************************************************************/
 int NewBlock_alloc()
 {
   int i;

 /* Iterating through blocks and checking for free block.
  * if block is not free than it sets block as used by calling
  * setBlock_asUsed. The blocks are starting at 9 because data
  * blocks start at block 9 the blocks before that are either
  * super block or i node table blocks.
  */
   for(i=9; i < NUM_BLOCKS; i++)
   {
     if(!isBlockFree(i))
     {
       setBlock_asUsed(i,1);

       return i;
     }
   }

  /* Returns error if all the block are used.
   * Thus there should be no block to allocate.
   */
   error_string= "Free Block cannot be allocated, sorry.";

   return -1;
 }

