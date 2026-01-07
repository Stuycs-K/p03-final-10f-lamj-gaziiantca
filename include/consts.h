#ifndef CONSTS_H_ANOTHER_GENERIC_FILE_NAME
#define CONSTS_H_ANOTHER_GENERIC_FILE_NAME

#ifdef DEFINE_GLOBALS 
#define EXTERN 
#else 
#define EXTERN extern  
//I forgot what this does but I used it one time a few months ago and this is how I wrote it for some reason so im blaming the compiler for it thats probably the reason 
#endif 

#include "types.h"

EXTERN const u8 debugLogs; 

#endif

