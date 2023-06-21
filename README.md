# CLAP 👏
a **C**ommand **L**ine **A**rgument **P**arser functionality for c++ aplications 
##

CLAP was created as project to provide means for a programmer to set parameters to modify the program's behavior, and to easily parse the arguments passed by the program's user. 

## setup
Simply locate CLAP.h & CLAP.cpp in your project's directory. However: only include CLAP.h in your project (for VS user, only add CLAP.h in "project properties")

## Documentation
#### class Parser
After ```#include "CLAP.h"```, you start by instantiate a ```Parser``` object with programs CL arguments:\
```Parser p{argc, argv};```\
```Parser p{argc, argv, true}; // calling Parser::printUsage() error upon exception```

#### Setting parameters/ flags
This step is necessary to define the variables to be used later in the program.\
Calling the the Parser object's methods ```set_param()``` and ```set_flag()``` lets you add features to be used by the program user:
```
set_param(flags : [string] - different forms of invoking flags (space-seperated) by user (e.g "-t -T --thr", "-v --verbose").
          label : [string] - name of parameter, used to extract the argument's value passed by program user from Parser object.
          type  : [string] - specification of the value type of the parameter.
       required : [bool]   - (optional) set the parameter to be mandatory for the execution of the program.
    description : [string] - (optional) description of parameter's functionality to be prompted.
        passive : [bool]   - (optional) true if parameter acts as flag, e.i w/o adjacent value (using set_flag() is for better intuitive use)
```
set_flag() is essentialy set_param(), just more convenient for defining flags (parameters with no adjacent arguments) 

- in case you want to add a description to a defined parameter, one can do so by calling
  
