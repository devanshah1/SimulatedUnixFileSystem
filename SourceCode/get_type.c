#include "globaldata.h"

/*************************************************************************
 * sfs_gettype(char *pathName)
 * The sfs_gettype function is used to retrive the file type that for the 
 * specified pathName.
 *
 *    pathname - the full path name that was entered by the user 
 * Successful
 *	return  file type : returns either Directory or Regular
 * Error return
 * 	return -1 : Entered path name was invalid
 *	return -2 : Directory entered was invalid
 *	return -3 : Directory could not be found
 **************************************************************************/
int sfs_gettype(char *pathName)
{
	int index;

	/* Setting index equal to the value that is retrived from the findIndex_file
	 * function. Also in the findIndex_file the path name is passed through so
	 * that the pathName can be validated also.
	 */
	index = findIndex_file(pathName);

	// Returns -1 if pathName entered is not valid
	if(index == -1)
	{
		return -1;
	}	

	// returns -2 if the directory is invalid
	if(index == -2)
	{
		return -2;
	}
	
	// return -3 if the directory can not be found
	if(index == -3)
	{
		return -3;
	}
	
	/* Returns the file type using the File Control Block struct which stores
	 * the file type
	 */
	return FILE_CONTROL_BLOCK0[index].type;
}
