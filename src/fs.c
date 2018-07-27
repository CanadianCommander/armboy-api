#include "fs.h"
#include "core.h"
#include "util.h"

void loadFileSystemDriver(void){
  LOAD_KERNEL_MOD(FILE_SYSTEM_DRIVER_ID);
}

bool isFileSystemDriverLoaded(){
  return isLoaded(FILE_SYSTEM_DRIVER_ID);
}

void initSDandFAT(void){
  SYS_CALL(FILE_SYSTEM_DRIVER_ID, FS_FUNCTION_INIT_SD, NULL);
  SYS_CALL(FILE_SYSTEM_DRIVER_ID, FS_FUNCTION_INIT_FAT, NULL);
}

bool listDirectory(char * path, LsDir * lsDir){
  LsDirArgs lsd;
  lsd.lsDir = lsDir;
  lsd.path = path;
  lsd.res = false;
  SYS_CALL(FILE_SYSTEM_DRIVER_ID, FS_FUNCTION_LIST_DIR, &lsd);
  return lsd.res;
}

bool openFile(char * path, FileDescriptor * fd){
  GetFileArgs gfa;
  gfa.path = path;
  gfa.fd = fd;
  gfa.res = false;
  SYS_CALL(FILE_SYSTEM_DRIVER_ID, FS_FUNCTION_OPEN_FILE, &gfa);
  return gfa.res;
}

uint32_t readFile(uint8_t * buffer, uint32_t  count, FileDescriptor * fd){
  ReadBytesArgs rba;
  rba.fd = fd;
  rba.count = count;
  rba.buffer = buffer;
  rba.res = 0;
  SYS_CALL(FILE_SYSTEM_DRIVER_ID, FS_FUNCTION_READ_FILE, &rba);
  return rba.res;
}

void seekFile(uint32_t offset, uint8_t reference, FileDescriptor * fd){
  SeekArgs sa;
  sa.fd = fd;
  sa.offset = offset;
  sa.reference = reference;
  SYS_CALL(FILE_SYSTEM_DRIVER_ID, FS_FUNCTION_SEEK_FILE, &sa);
}
