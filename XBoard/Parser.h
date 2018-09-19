#ifndef PARSER_H
#define PARSER_H

#include "ExecThreadArgs.h"

int ParseConfigFile(ExecThreadArgs* ThreadDataArray); //Recieves an array of ETA structures and populates the ButtonBind field based on a config file 
                                                      //returns the number of lines it parses in the config file.

#endif //!EXECTHREADARGS_H