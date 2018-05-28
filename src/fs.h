#ifndef FS_H_
#define FS_H_
#include <stdint.h>
#include "../drivers/armboy-fs/src/FAT32/fsSysCalls.h"

//file system driver id
#define FILE_SYSTEM_DRIVER_ID 3
//file system functions
#define FS_FUNCTION_INIT_SD   0
#define FS_FUNCTION_INIT_FAT  1
#define FS_FUNCTION_LIST_DIR  2
#define FS_FUNCTION_OPEN_FILE 3
#define FS_FUNCTION_READ_FILE 4
#define FS_FUNCTION_SEEK_FILE 5

void loadFileSystemDriver();


void initSDandFAT();

/**
  populate lsDir with LIST_DIR_FILE_COUNT directory entries.
  if this function returns true, then there are more files in the directory to be read.
  continue calling this function with the same lsDir structure.
*/
bool listDirectory(char * path, LsDir * lsDir);

/*
  open file indicated by path.
  @return ture on success
*/
bool openFile(char * path, FileDescriptor * fd);


/**
  read count bytes from file an place them in buffer.
  @return the number of bytes written to buffer
*/
uint32_t readFile(uint8_t * buffer, uint32_t count, FileDescriptor * fd);

/**
  seek the file. offset from either,
  #define SEEK_CURR   0
  #define SEEK_START  1
  #define SEEK_END    2
  reference points
*/
void seekFile(uint32_t offset, uint8_t reference, FileDescriptor * fd);

#endif /*FS_H_*/
