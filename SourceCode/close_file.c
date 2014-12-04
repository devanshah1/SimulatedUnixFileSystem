#include "globaldata.h"

/*************************************************************************
 * sfs_close(int fd)
 * The sfs_close function is used to close a file with the following file
 * descriptor number that was entered by the user.
 *
 *    int fd - is the entered file descriptor number
 * Successful
 *	return  1 : The specified file has been closed
 * Error return
 * 	return -1 : Trying to close a file with the wrong fd number
 *	return -2 : The file does not exist
 **************************************************************************/
int sfs_close(int fd)
{
	/* If the current file that wants to be closed is null then return -2
		 * uses the struct of OpenFileRef to retrieve the type of the current file
		 */
		if(OpenFileReferance[fd].type == FILETYPE_NONE)
		{
			return -2;
		}
	/* If trying to close a file that does not have a file
	 * descriptor number or if the file descriptor number entered
	 * is greater then the number of files that are allowed to be open
	 * at a time.
	 */
	if(fd >= MAXOPEN_FILES || fd < 0 )
	{
		return -1;
	}

	/* Once the first two if statement are checked and passed then
	 * the current file's type is changed FILE_TYPE_NONE
	 */
	OpenFileReferance[fd].type = FILETYPE_NONE;

/* Used to output the file name that has been closed successfully
 * the way that the file name is outputed using the struct for FCB and
 * OpenFileRef struct to retrieve the name of the file. The structures
 * store the various information of the files that are in question
 */
#ifdef SFS_DEBUG
    fprintf(stdout,"File \"%s\" closed successful\n",FILE_CONTROL_BLOCK0[OpenFileReferance[fd].index].name);
#endif

	return 1;
}
