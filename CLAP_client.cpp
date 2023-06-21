#include <iostream>
#include "Debug/CLAP.h" // locate CLAP.h & CLAP.cpp in your project | ONLY ADD CLAP.h!! (exclude CLAP.cpp)


int main(int argc, char *argv[])
{
    std::cout << std::boolalpha;

    
    Parser p{ argc, argv };       //
    //Parser p{ argc, argv , true}; // creates a Parser object which prompts program Usage when error occurs.

    // --- setting flags and parameters of program ---


    /* set_param(flags : [string] - different forms of invoking flags (space-seperated) by user (e.g "-t -T --thr", "-v --verbose").
    *            label : [string] - name of parameter, used to extract the argument's value passed by program user from Parser object.
    *            type  : [string] - specification of the value type of the parameter.
    *         required : [bool]   - (optional) set the parameter to be mandatory for the execution of the program.
           description : [string] - (optional) description of parameter's functionality to be prompted.
               passive : [bool]   - (optional) true if parameter acts as flag, e.i w/o adjacent value (using set_flag() is for better intuitive use)
    */    


    p.set_param("-t --threads", "THREADS","int", false, "number of threads to be used"); // optional parameter, of type int, with description.

    p.set_flag("-r -R -read", "READ","bool", true); // required flag (no adjacent value expexted), of type bool, no description
    // p.set_param("-r -R -read", "READ","bool", true, "", true) // last flag can be set like this as well.
    
    p.set_flag("-v -V --verbose", "VERB", "bool"); //  optional flag, of type bool.
    
    p.set_param("-i -I", "PATH", "string",true); // required parameter, of type string.

    p["READ"]->set_param_desc("read data from input path"); // setting description of a parameter, after defining it.
    
    
                                                            
    // --- After setting is done ---


    p.search_active_flags(); // parse the defined parameters and validates passed arguments by program user - MUST BE CALLED AFTER PARAMETER SETTING AND VARIABLE USAGE!! 
    

    // --- extracting and using arguments values ---


    int thr;
    if (p.is_avail("THREADS")) // check if defined parameter was passed by user.
    {
        thr = p.var<int>("THREADS"); // extracts the value of associated label (e.g "THREADS")
    }
    else {
        thr = 4;
    }

    string path{ p.var("PATH") }; // extracts the value of associated label, of type string.

    
    p.printUsage();
    
    
    return 0;
}

