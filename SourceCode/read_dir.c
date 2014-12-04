#include "globaldata.h"
/*************************************************************************
 * sfs_readdir(int fd, char *mem_pointer)
 * The sfs_readdir function is used to read the file name components from
 * a directory file and stores them into a place on the memory specified 
 * by the mem_pointer.
 *
 *	fd - the full path name that was entered by the user  
 *	*mem_pointer - where name components will be stored for the diectory in 
 *		       question
 * Successful
 *	return  1 : if a name component has been successfully read into memory
 *	return  0 : if nothing has been placed into the memory
 **************************************************************************/
int sfs_readdir(int fd,char *mem_pointer)
{
	// Retriving the DirectoryEntry structure and setting it to entry variable
	DirectoryEntry entry;
	// Checking till return is 1
	while(1)
	{
		/* ReadFile returns a value of -1 then return 0
		 * readFile takes inf the fd and the DirectoryEntry structure
		 * and also the size of the DirectoryEntry structure
		 */
		if(SFS_FAILED(readFile(fd,&entry,sizeof(DirectoryEntry))))
		{
			// Return 0 if there was nothing placed into memory
			return 0;
		}
		// If the entry of the directory is not equal to 0 then
		if(entry.ID!=0)
		{
			// Stores the name of the file into the mem_pointer
			strcpy(mem_pointer,entry.name);
			// Return 1 if sucessfully
			return 1;
		}
	}
	return 1;
}
