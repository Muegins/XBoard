#ifndef XBTHREADS_H
#define XBTHREADS_H 

#include <stdlib.h>
#include <mutex>
#include "math.h"

#include "ExecThreadArgs.h"

extern std::mutex stateMTX;

bool SpawnExecThread(ExecThreadArgs& ETA); //Takes a pointer to an ETA structure and spawns a thread to handle the bind. It spawns different threads based on the mode in the ButtonBind.
										   //It returns true if the thread was succcessfully spawned and false if it failed.

#endif //!XBTHREADS_H