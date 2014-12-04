#include "globaldata.h"

/*************************************************************************
 * sfs_create(char *pathName,int type)
 * The sfs_create function is used to create a file that does not already 
 * exist in the file system.
 *
 *    int type - specifies either regular or Directory, 0 or 1 respectivily
 *    			 depending on user input
 *    char *pathName - the full path name that was entered by the user 
 * Successful
 *	return  1 : The file has been created sucessfully
 * Error return
 * 	return -1 : the pathName entered was not valid 
 *	return -2 : the Directory is invalid
 *	return -3 : the directory could not be found
 *	return -4 : the file already exists
 *	return -5 : the file creation failed 
 **************************************************************************/
int sfs_create(char *pathName,int type)
{
	int i,index = 0;
	char File_Name[8],*oldPathName = pathName;
	
	/* Checks to see that the pathname that was inputed by the user is valid
	 * the pathname is also passed to the function validatePath and if not valid
	 * then return -1, which refers to an invalid path name
	 */
	if(!validatePath(pathName)||!pathName)
	{
		fprintf(stdout,"Specified path Name is invalid.\n");
		fprintf(stdout,"TRY AGAIN!!\n");
		return -1;
	}

	// Increment the pathName to the next character
	pathName++;

	/* This while loop will continue to run until the loop reaches a
	 * break only if the singleFile function is true
	 */
	while(1)
	{	

		/* If the pathName is a singleFile that is going to be created then
		 * end the while loop */
		if(singleFile(pathName))
		{
			break;
		}
		/* Retrieve the type of the file from the specified FCB and
				 * compare it to the constant value FILETYPE_DIR if they
				 * are not equal then return an error for directory invalid
				 */
		if(FILE_CONTROL_BLOCK0[index].type != FILETYPE_DIR)
		{
			fprintf(stdout,"invalid Directory!\n");

			return -2;
		}
		// Setting the value that was retrived from getChildIndex_dir function to index
		index = getChildIndex_dir(index,File_Name);

		// PathName is incremented according to the return from the File_Name_parse function
		pathName += fileName_parse(pathName,File_Name);


		/* If the index that was retrives is less then 0 then that means
		 * that there is an error being sent from the getChildIndex 
		 * function, return -3 refers to directory was not found
		 */
		if(index < 0)
		{
			fprintf(stdout,"Directory \"%s\" not found\n",File_Name);

			return -3;
		}
	}
	/* The function getChildIndex returns either a positive or a negative value
	 * so if the value is positive that refers to the fact that
	 * the file already exists, so -4 is returned
	 */
	if(getChildIndex_dir(index,pathName) >= 0)
	{
		fprintf(stdout,"This file already exists\n");

		return -4;
	}

	// Copying the string that is stored in the pathName into File_Name
	strcpy(File_Name,pathName);

	i=0;
	/* As long as there is a character in the File_Name array then the while
	 * loop will run accordinly.
	 */
	while(File_Name[i])
	{
		// If any charater in File_Name contains "/" then replace it with null
		if(File_Name[i]=='/')
		{
			File_Name[i]='\0';
		}
		i++;
	}
	/* makeNewFile is used to check to make sure that the file is able to be
	 * created and if not then the function will return a -1 which will then
	 * be compared to SFS_FAILED, and then the return will be sent -5
	 */
	if(SFS_FAILED(makeNewFile(index,type,File_Name)))
	{
		fprintf(stdout,"New file creation failed\n");

		return -5;
	}
/* Used to output the new file name that has been created sucessfully
 * the way that the file name is outputed is using pathname that was
 * inputed by the user
 */
#ifdef SFS_DEBUG
    fprintf(stdout,"New file \"%s\" created successfully \n",oldPathName);
#endif

	return 1;
}

/*************************************************************************
 * makeNewFile(int parentIndex,int type,const char *name)
 * The makeNewFile function is used to create a new file and store it on
 * disk for later reference
 * Successful
 *		return 1 : file has has been sucessfull created
 * Return Failed
 * 		return -1 : The file header was not found
 * 		return -2 : Could not allocate new block
 * 		return -3 : The directory could not be found that was trying to be
 * 					access
 **************************************************************************/
int makeNewFile(int parentIndex,int type,const char *name)
{
  int i,j,index;
  DirBlock dirDataBlock;

  /* The following for loop is going to run through all of the files blocks
   * to retrieve a place for the file to be stored
   */
  for(index=0; index < MAX_FILES; index++)
  {
	/* Once type of the fist FCB block is found to be of FILETYPE_NONE
	 * the loop will end and the index variable will have a specific value
	 * the index where the file will be stored
	 */
    if(FILE_CONTROL_BLOCK0[index].type == FILETYPE_NONE)
    {
    	break;
   	}
  }

  // If the index is equal to the MAX_FILES then the following error will be returned
  if(index == MAX_FILES)
  {
    error_string = "Empty File Header could not be found, sorry.";

    return -1;
  }

  for(j=0; j < 4; j++)
  {
	/* If value that is retried from the InodeTable is not 0 then the following will
	 * he executed if not then it will follow to the else structure below
	 */
    if(InodeTable[j][parentIndex] != 0)
    {
      get_block(InodeTable[j][parentIndex],(char*)&dirDataBlock);

      /* For loop is going to run through all the entries that are in
       * the DirBlock to retrieve certain data and set certain data
       */
      for(i=0; i < 16; i++)
      {
    	/* This if structure is checking to make sure that the following only
    	 * be performed if the entry ID in the block in question be equal to
    	 * 0, which means that it has not been used yet.
    	 */
        if(dirDataBlock.entry[i].ID == 0)
        {
          /* Setting the current index value that was found earlier and set if to the
           * the DirBlock entry ID
           */
          dirDataBlock.entry[i].ID = index;
          // Copy the name that was passed to the function into the DirBlock entry name
          strcpy(dirDataBlock.entry[i].name,name);
          /* Setting the type that was passed to the function to the FCB block of the
           * current index number
           */
          FILE_CONTROL_BLOCK0[index].type = type;
          // Copy the name that was passed to the function into the FCB structure variable name
          strcpy(FILE_CONTROL_BLOCK0[index].name,name);

          int k = 0;
          /* Running through the loop to set all the InodeTable values that are present
           * in the current index to 0
           */
          for(k=0; k < 8; k++)
          {
          	InodeTable[k][index] = 0;
          }

          // Write the the FCB of the current index to the disk
          writeFCB(index);
          // Put the DirBlock onto the disk
          put_block(InodeTable[j][parentIndex],(char*)&dirDataBlock);

          return 1;
        }
      }
    }
	/* If value that is retried from the InodeTable is equal to 0 then this
	 * structure will be followed
	 */
    else
    {
      /* Allocate a new block to the iNodeTable, the NewBlock_alloc function
       * will return a value of a block that is available and that value will be stored
       * on the INodeTable in the appropriate row and column
       */
      InodeTable[j][parentIndex] = NewBlock_alloc();
      // If the value from the NewBlock_alloc function was a 0 then the following error will occur
      if(InodeTable[j][parentIndex] == 0)
      {
        fprintf(stdout,"Could not allocate new block\n");

        return -2;
      }

      freeMem(&dirDataBlock,sizeof(DirBlock));
      /* Setting the current index value that was found earlier and set it to the
       * the DirBlock entry ID 0 because the first slot is available for use
       */
      dirDataBlock.entry[0].ID = index;
      // Copy the name that was passed to the function into the DirBlock entry name slot 0
      strcpy(dirDataBlock.entry[0].name,name);
      /* Setting the type that was passed to the function to the FCB block of the
       * current index number
       */
      FILE_CONTROL_BLOCK0[index].type = (unsigned char)type;
      // Copy the name that was passed to the function into the FCB structure variable name
      strcpy(FILE_CONTROL_BLOCK0[index].name,name);

     int k = 0;
     /* Running through the loop to set all the InodeTable values that are present
      * in the current index to 0
      */
     for(k=0; k < 8; k++){
          	InodeTable[k][index] = 0;
          }

     // Write the the FCB of the current index to the disk
     writeFCB(index);
     // Put the DirBlock onto the disk
     put_block(InodeTable[j][parentIndex],(char*)&dirDataBlock);

      return 1;
    }
  }

  error_string = "Empty directory could not be found!";

  return -3;
}
