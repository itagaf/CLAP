# CLAP 👏
a **C**ommand **L**ine **A**rgument **P**arser functionality for c++ applications 
##

CLAP was created as a project to provide means for a programmer to set parameters to modify the program's behavior, and to easily parse the arguments passed by the program's user. 

## setup
Simply locate CLAP.h & CLAP.cpp in your project's directory.\
However: only include CLAP.h in your project (for VS user, only add CLAP.h in "project properties")

## Documentation
#### class Parser
After ```#include "CLAP.h"```, you start by instantiate a ```Parser``` object with programs CL arguments:\
```Parser p{argc, argv};```\
```Parser p{argc, argv, true}; // calling Parser::printUsage() error upon exception```

#### Setting parameters/ flags
This step is necessary to define the variables to be used later in the program.
- Calling the the Parser object's methods ```set_param()``` and ```set_flag()``` lets you add features to be used by the program user:
```
set_param(flags : [string] - different forms of invoking flags (space-seperated) by user (e.g "-t -T --thr", "-v --verbose").
          label : [string] - name of parameter, used to extract the argument's value passed by program user from Parser object.
          type*  : [string] - specification of the value type of the parameter.
       required : [bool]   - (optional) set the parameter to be mandatory for the execution of the program.
    description : [string] - (optional) description of parameter's functionality to be prompted.
        passive : [bool]   - (optional) true if parameter acts as flag, e.i w/o adjacent value (using set_flag() is for better intuitive use)
```
**for "type" - currently supported types of parameters: "int", "long", "long long", "float", "double", "long double", "bool"*


set_flag() is essentialy set_param(), just more convenient for defining flags (parameters with no adjacent arguments) 

- in case you want to add a description to a defined parameter, one can do so by calling ```set_param_desc()``` in the following manner:
```p["THREADS"]->set_param_desc("number of threads to utilize"); // p is Parser object```

- to set the program's general description, simply call ```set_general_desc()```:\
  ```p.set_general_desc("Program to keep track after my dog.\nBest program ever!"); // p is Parser object```

#### Processing parameters and arguments
- Once everything is set, call the Parser method ```search_active_flags()``` to process user's input arguments and and validate that all is in accordance.

#### Extracting user's input
- use the ```var()``` functions to extract and use user's arguments:\
  ```p.var<int>("THREADS"); // returns value of type int (p is a Parser object)```
- for variable of type "string", use nontemplate version of var():\
  ```p.var("PATH"); // returns value of type string```
- another useful method is is_avail() which return boolean value whether a given parameter was passed by user:\
  ```p.is_avail("SUFFIX"); // "true" if parameter "SUFFIX" was used by user```

- To prompt the programs help manual and usage description, simply call ```printUsage()``` from the object.\
All the pre-defined parameters and their descriptions (if set) will be prompet.\
upon calling, the program will output:
```
Usage: C:\Users\john_doe\CLAP_client.exe {-i -I} {-r -R -read} [-t --threads] [-v -V --verbose]

Program to keep track after my dog.
Best program ever!
        -i -I           :PATH {Required}  - path of input file (abs)
        -r -R -read     :READ {Required}  - read data from input path
        -t --threads    :THREADS - number of threads to utilize
        -v -V --verbose :VERB - verbose prompting
```
**```{Required}``` and ```{}``` are added to parameters if required=true in ```set_param()```/```set_flag()``` (see above)* 

## Pay attention:
- The CLAP interface assumes that only one object of class Parser will be instantiated.\
 Therfore, no methods or infrastructure were made to deal with multiple objects (e.g copy constructors etc).
- For clarity, the term "flag" used here to describe a parameter with no adjacent argument (the flag IS the argument, thus interpreted as boolean value)   
- ```search_active_flags()``` proccess the program user's arguments. All parameters MUST be defined before calling this method.\
(Doing otherwise will cause unexpected behavior).
- Order of parameters to be prompted by ```printUsage()``` method is according to the order of their setting.\
If you wish for the required parameters to be prompted first, simply set them earlier.   
