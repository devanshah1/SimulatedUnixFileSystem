#include "globaldata.h"

/*************************************************************************
 * sfs_delete(char *pathName)
 * The sfs_delete function is used to delete the specified file or directory
 * 
 *    char *pathName - the full path name that was entered by the user 
 * Successful
 *	return  1 : The file has been deleted sucessfully
 * Error return
 * 	return -1 : the pathName entered was not valid 
 *	return -2 : the Directory is invalid
 *	return -3 : the directory could not be found
 *	return -4 : Can't delete opened file
 *	return -5 : file does not exist
 **************************************************************************/

int sfs_delete(char *pathName)
{
	int i,parentIndex=0,index=0;
	char fileName[8],*oldPathName=pathName;

	/* Checks to see that the pathname that was inputed by the user is valid
	 * the pathname is also passed to the function validatePath and if not valid
	 * then return -1
	 */
	if(!pathName||!validatePath(pathName))
	{
		fprintf(stdout,"Path name invalid\n");

		return -1;
	}
	// Increment the pathName to the next character
	pathName++;

	/* This while loop will continue to run until the loop reaches a
	 * break only if the singleFile function is true
	 */
	while(1)
	{
		/* Retrive the type of the file type from the specified FCB and
		 * compare it to the constant value for the FILETYPE_DIR if they
		 * are not equal then return an error for directory invalid*/
		if(FILE_CONTROL_BLOCK0[index].type != FILETYPE_DIR)
		{
			fprintf(stdout,"Directory Invalid!\n");

			return -2;
		}
		/* If the pathName is a singleFile that is going to be created then
		 * end the while loop
		 */
		if(singleFile(pathName))
		{
			break;
		}

		// PathName is incremented according to the return from the fileName_parse function
		pathName+=fileName_parse(pathName,fileName);
		// Retrive the index from the function getChildIndex_dir
		index=getChildIndex_dir(index,fileName);
		/* If the index that was retrived is less then 0 then that means
		 * that there is an error being sent from the getChildIndex
		 * function return -3 refers to directory was not found
		 */
		if(index<0)
		{
			fprintf(stdout,"Directory Not Found\n");

			return -3;
		}
	}

	/* Checking all the open files to make sure that the files that are open
	 * are not the one that want to be deleted by the user
	 */
	for(i=0; i < MAXOPEN_FILES; i++)
	{
		// If the open file type is not equal to the value of FILE_TYPE_NONE then
		if(OpenFileReferance[i].type != FILETYPE_NONE)
		{
			/* This if structure is comparing the string values that are present
			 * in pathName and also the name of the that is stored on the FCB block
			 * according to the open file index. Return error -4 if compare is
			 * sucessfull which refers to opened file cannot be deleted
			 */
			if(strcmp(pathName,FILE_CONTROL_BLOCK0[OpenFileReferance[i].index].name)==0)
			{
				fprintf(stdout,"Cannot delete open file: \"%s\"\n",pathName);

				return -4;
			}
		}
	}

	// Copying the string that is stored in the pathName into fileName
	strcpy(fileName,pathName);

	/* As long as there is a character in the fileName array then the while
	 * loop will run accordinly.
	 */
	while(fileName[i])
	{
		if(fileName[i]=='/')
		{
			// if any charater in fileName contains "/" then replace it with null
			fileName[i]='\0';
		}
		i++;
	}

	parentIndex=index;
	index=dirChildIndex_remove(index,pathName);
	/* If the index that was retrives is less then 0 then that means
	 * that there is an error being sent from the dirChildIndex_remove
	 * function return -5 refers to file does not exist*/
	if(index < 0)
	{
		fprintf(stdout,"File does not exist!\n");

		return -5;
	}
	// set the current files FCB type of the file to null
	FILE_CONTROL_BLOCK0[index].type = FILETYPE_NONE;
	/* This loop is being runned to clean the iNode table of the current file
	 * that is in question
	 */
	for(i=0; i < 8; i++)
	{
		/*if the inodeTable is not already refresed to null then set to null
		 * using the setBlock_asUsed function. Refer to setBlock_asUsed for
		 * further explanation.
		 */
		if(InodeTable[i][index] != 0)
		{
			setBlock_asUsed(InodeTable[i][index],0);
		}
	}
/* Used to output file name that was deleted sucessfully
 * the way that the file name is outputed is using pathname that was
 * inputed by the user. Return 1 for sucess
 */
#ifdef SFS_DEBUG
    fprintf(stdout,"Deleted file \"%s\" successfuly\n",oldPathName);
#endif

	return 1;
}
