#include "globaldata.h"

/*************************************************************************
 * sfs_getsize(char *pathName)
 * The sfs_getsize function is used to retrive the size of the file that is 
 * specified by the pathName. 
 *
 *    pathname - the full path name that was entered by the user 
 * Successful
 *	return  size : returns the size of the file 
 * Error return
 * 	return -1 : Entered path name was invalid
 *	return -2 : Directory entered was invalid
 *	return -3 : Directory could not be found
 **************************************************************************/

int sfs_getsize(char *pathName)
{
	int index,i,size = 0;

	/* Setting index equal to the value that is retried from the findIndex_file
	 * function. Also in the findIndex_file the path name is passed through so
	 * that the pathName can be validated also.
	 */
	index = findIndex_file(pathName);

	// Returns -1 if pathName entered is not valid
	if(index == -1)
	{
		return -1;
	}	

	// Returns -2 if the directory is invalid
	if(index == -2)
	{
		return -2;
	}
	
	// Return -3 if the directory can not be found
	if(index == -3)
	{
		return -3;
	}
	int open_size = sfs_open(pathName);
	

	// Used to increment the the size of the current file accordingly
	for(i=0; i < 8; i++)
	{	
		size += InodeTable[i][index] > 0;
	}

	return 128*size;
}
