/*
 * Copyright (C) 2009 Niek Linnenbank
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __FILESYSTEM_LINN_FILESYSTEM_H
#define __FILESYSTEM_LINN_FILESYSTEM_H

#include <FileSystem.h>
#include <FileSystemPath.h>
#include <FileSystemMessage.h>
#include <Storage.h>
#include <Types.h>
#include <Array.h>
#include <HashTable.h>
#include <Integer.h>
#include "LinnSuperBlock.h"
#include "LinnInode.h"
#include "LinnGroup.h"

/**
 * @defgroup linn LinnFS (Linnenbank Filesystem) 
 * @{
 */

/**
 * @name Filesystem limits.
 * @{
 */

/** Minimum blocksize. */
#define LINN_MIN_BLOCK_SIZE 1024

/** Maximum blocksize. */
#define	LINN_MAX_BLOCK_SIZE 4096

/**
 * @}
 */

#ifndef __HOST__

/**
 * Linnenbank FileSystem (LinnFS).
 */
class LinnFileSystem : public FileSystem
{
    public:
    
	/**
	 * Class constructor function.
	 * @param path Path to which we are mounted.
	 * @param storage Storage provider.
	 */
	LinnFileSystem(const char *path, Storage *storage);

	/**
	 * Retrieve the superblock pointer.
	 * @return Pointer to the superblock for this filesystem.
	 * @see LinnSuperBlock
	 */
	LinnSuperBlock * getSuperBlock()
	{
	    return &super;
	}

	/**
	 * Get the underlying Storage object.
	 * @return Storage pointer.
	 * @see Storage
	 */
	Storage * getStorage()
	{
	    return storage;
	}

	/**
	 * Read an inode from the filesystem.
	 * @param inodeNum Inode number.
	 * @return Pointer to an LinnInode on success, ZERO on failure.
	 * @see LinnInode
	 */
	LinnInode * getInode(u32 inodeNum);

	/**
	 * Read a group descriptor from the filesystem.
	 * @param groupNum Group descriptor number.
	 * @return Pointer to an LinnGroup on success, ZERO on failure.
	 * @see LinnGroup
	 */
	LinnGroup * getGroup(u32 groupNum);

	/**
	 * Read a group descriptor from the filesystem, given an inode number.
	 * @param inodeNum Find the corresponding group via this inode number.
	 * @return Pointer to an LinnGroup on success, ZERO on failure.
	 * @see LinnGroup
	 * @see LinnInode
	 */
	LinnGroup * getGroupByInode(u32 inodeNum);

	/**
	 * Calculates the offset inside storage for a given block.
	 * @param inode LinnInode pointer.
	 * @param blk Calculate the offset for this block.
	 * @return Offset in bytes in storage.
	 * @see LinnInode
	 */
	u64 getOffset(LinnInode *inode, u32 blk);

	/**
         * Load a file corresponding to the given path from underlying storage.
         * @param path Full path to the file to load.
         * @return Pointer to FileCache object if the file exists, or ZERO otherwise.
         */
	FileCache * lookupFile(FileSystemPath *path);

    private:
	
	/**
	 * Creates a new LinnFile.
	 * @param msg Describes the file creation request.
	 * @param path Full path to the file to create.
	 */
	Error createFile(FileSystemMessage *msg,
                         FileSystemPath *path);

	/** Provides storage. */
	Storage *storage;
	
	/** Describes the filesystem. */
	LinnSuperBlock super;
	
	/** Group descriptors. */
	Array<LinnGroup> *groups;

	/** Inode cache. */
	HashTable<Integer<u32>,LinnInode> inodes;
};

#endif /* __HOST__ */

/**
 * @}
 */

#endif /* __FILESYSTEM_LINN_FILESYSTEM_H */
