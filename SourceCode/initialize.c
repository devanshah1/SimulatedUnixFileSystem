#include "globaldata.h"

/*************************************************************************
 * sfs_initialize(int erase)
 * The sfs_initialize function needs to be the first function that is called
 * by the file system to initialize the file system.
 *
 *    erase = 0 - initially erase is set to 0
 *    erase = 1 - indicates that any file system on the simulated disk be
 *		  destroyed, and a brand new file system be created 
 * Successful
 *	return  1 : File system has just been initialized
 * Error return
 * 	return -1 : Disk setup failed
 * 	return -2 : Disk setup failed cause entered value was not 1 or 0
 **************************************************************************/
int sfs_initialize(int erase)
{
  if(erase == 1 || erase == 0)
  {
	int i;
	// Sends the following parameters to the get_block function
	get_block(0,(char*)&SuperBlock0);

	/* If erase is equal to SFS_ID or SuperBlock0.ID is not equal to SFS_ID then
	 * trys to start a new disk if the return of startNewDisk() is -1 then the
	 * following is outputed
	 */
	if(erase||SuperBlock0.ID != SFS_ID)
	{
		/* SFS_FAILED is equal to -1 then error is outputed
		 * Note: Refer to startNewDisk for more information on errors
		 */
		if(SFS_FAILED(startNewDisk()))
		{
			fprintf(stdout,"Disk setup failed!\n");

			return -1;
		}
	}
	// If erase is equal to anything else which includes 1
	else
	{
		int k = 0;
		// Loads the inode table from the disk into the memory
		for (k = 0; k<8; k++)
		{
			get_block(k,(char*)&InodeTable[k]);
		}
		// Saves the inode table back into the simdisk
		for (i=0;i<8;i++)
		{
			put_block(k,(char*)&InodeTable[k]);
		}
	}
	// Setting the first 4 file blocks types 2 which refers to no file type
	OpenFileReferance[0].type=FILETYPE_NONE;
	OpenFileReferance[1].type=FILETYPE_NONE;
	OpenFileReferance[2].type=FILETYPE_NONE;
	OpenFileReferance[3].type=FILETYPE_NONE;
// Returns if the system has been initialized sucessfully
#ifdef SFS_DEBUG
    fprintf(stdout,"File system initialized!\n");
#endif

	return 1;
  }
 else
    fprintf(stdout,"Incorrect Entry ... TRY AGAIN!!\n");
  	 return -2;
}


/*************************************************************************
 * startNewDisk()
 * The startNewDisk function is used to set up the new disk, this function
 * is mainly used by the sfs_initialize to refresh the simulated disk
 *
 * Successful
 *	return 1 : the disk has been successfully initialize
 * Error return
 * 	return -1 : if the function put_block returns an error
 **************************************************************************/
 int startNewDisk()
{
  int i;

  /* Setting the id of the super block to the value of that is
   * present in the SFS_ID variable
   */
  SuperBlock0.ID = SFS_ID;
  /* Run through the loop to set each of the super blocks free
   * info to 0
   */
  for(i=0; i < MAX_FILES; i++)
  {
	  SuperBlock0.free[i] = 0;
  }

  /* The following if statement is used to put the super block
   * information on to the disk, if the put_block function retrieves
   * a error then the return will be -1
   */
  if(put_block(0,(char*)&SuperBlock0) != 0)
  {
	  return -1;
  }
  /* The following if statement is going to set all the types
   * of the FCB to FILETYPE_NONE so that the FCB consists of
   * no regular or directory files
   */
  for(i=1; i < MAX_FILES; i++)
  {
	  FILE_CONTROL_BLOCK0[i].type = FILETYPE_NONE;
  }

  // Setting the first FCB's type to a directory
  FILE_CONTROL_BLOCK0[0].type = FILETYPE_DIR;
  // Setting the name of the first FCB to be null
  FILE_CONTROL_BLOCK0[0].name[0] = '\0';

  int k = 0;
  /* Setting all the INodeTable values to 0 except for the first
   * set of the array to 9
   */
  for (k=0; k < 8; k++)
  {
	  if (k == 0)
	  {
		InodeTable[k][0] = FIRST_DATA_BLOCK;
	  }

	  else
		  InodeTable[k][0] = 0;
  }

  /* Putting the root directory onto the disk and if the return
   * of the put_block function is -1 then that block was
   * not put properly
   */
  if((put_block(FIRST_DATA_BLOCK, (char*)&FILE_CONTROL_BLOCK0[0]) != 0))
  {
	  return -1;
  }

  /* Putting the inode Table onto the disk, putting the inode
   * table on the first 8 blocks after the superblock
   */
  for (k = 1; k < 9; k++)
  {
	  put_block(k,(char*)&InodeTable[k]);
  }

  return 1;
}
