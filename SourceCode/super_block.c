#include "globaldata.h"

/*
 * Low Level function required for
 * the implementation.
 *
 */

/*************************************************************************
 * isBlockFree(int blockNum)
 * The sisBlockFree function is used to check if a give block number is free
 **************************************************************************/
int isBlockFree(int blockNum)
{
  return SuperBlock0.free[blockNum>>3]&(1<<(blockNum&7)); // bitwise shift.
}

/*************************************************************************
 * setBlock_asUsed(int blockNum,int used)
 * The setBlock_asUsed function is used to set the given block to used
 **************************************************************************/
int setBlock_asUsed(int blockNum,int used)
{
  used = used>0;

  SuperBlock0.free[blockNum>>3]&=0xFF^(1<<(blockNum&7));
  SuperBlock0.free[blockNum>>3]|=used<<(blockNum&7);

  put_block(0,(char*)&SuperBlock0);

  return 1;
}

/*************************************************************************
 * writeFCB(int index)
 * The writeFCB function is used to put the given File Control Block on to
 * the disk
 **************************************************************************/
int writeFCB(int index)
{
	put_block((InodeTable[0][index]),(char*)&FILE_CONTROL_BLOCK0[index]);
}





