#include "globaldata.h"

/*************************************************************************
 * sfs_open(int *pathname)
 * The sfs_open function is used to open a file described by the pathname
 * that was supplied by the user.
 *
 *	pathname - the full path name that was entered by the user  
 * Successful
 *	return  fd : file descripter number associated with the opened file
 * Error return
 * 	return -1 : Entered path name was invalid
 *	return -2 : Directory entered was invalid
 *	return -3 : Directory could not be found
 *	return -4 : limit has been reached for open files
 **************************************************************************/
int sfs_open(char *pathName)
{
	int index,fd,i;
	// retriving the index of the specified pathname
	index=findIndex_file(pathName);

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

	/* fd is incremented as the files are opened if the file opened does not
	* have a file type then end the loop
	*/
	for(fd=0; fd<MAXOPEN_FILES; fd++)
	{
		if(OpenFileReferance[fd].type == FILETYPE_NONE)
			break;
	}
	// Return -4 if the max open files is reached
	if(fd == MAXOPEN_FILES)
	{
		fprintf(stdout,"Limit reached for open files!\n");

		return -4;
	}
	// Setting the the file type that is stored in the FCB block associated
	// with the indexed file
	OpenFileReferance[fd].type = FILE_CONTROL_BLOCK0[index].type;
	// Setting the postition of the specified file to 0
	OpenFileReferance[fd].pos = 0;
	// Setting the index of the file to the related file
	OpenFileReferance[fd].index = index;

	int k = 0 ;
	/* Using the current fd and setting the dataIndex to each of the related
	 * inodetable entries according to the index numbers that was grabed from
	 * the above function findIndex_file
	 */
	for (k =0; k<8; k++)
	{
		OpenFileReferance[fd].dataIndex[k] = InodeTable[k][index];
	}

	/* Used to store the size of the file in the opened file reference
	 * structure
	 */
	for(i=0; i<8; i++)
	{
		OpenFileReferance[fd].size += InodeTable[i][index]>0;
		OpenFileReferance[fd].size *= BLOCK_SIZE;
	}
// Outputs the following statement if the file is sucessfully opened
#ifdef SFS_DEBUG
    fprintf(stdout,"Opened file \"%s\" [%d] successfuly!\n",pathName,fd);
#endif
	// Return fd if the file has been sucessfull opended only
	return fd;
}
