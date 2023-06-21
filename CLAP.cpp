// MathLibrary.cpp : Defines the exported functions for the DLL.

#include <utility>
#include <limits.h>
#include <iostream>
#include "CLAP.h"

// DLL internal state variables:
using namespace std;




template <typename T>
struct val_return {
    using type = T;
};



//class ParamValue

ParamValue::ParamValue(string& str) :m_str(str) {}


ParamValue::ParamValue() {}


void ParamValue::set_value(const string& str)
{
    m_str = str;
}


bool ParamValue::cast_to_bool(const string& str)
{

    if (str == "1" || str == "true" || m_str == "1") // m_str is "1" in case that Param.m_passive is true
    {
        return true;
    }
    else if (str == "0" || str == "false")
    {
        return false;
    }
    string exception{ "Unsupported boolean casting : " + str + " [ParamValue.cast_value(const string & type)]" };
    throw exception;
}


string ParamValue::get_str()
{
    return m_str;
}

/// end ParamValue class


// struct Param

Param::Param(const string& label = "\0", 
            const string& flag = "\0", 
            const string& type = "\0", 
            const string& desc = "", 
            const bool& required = false, 
            const bool& passive = false)
    : m_label(label), m_param_flag(flag), m_type(type), m_desc(desc), m_required(required), m_passive(passive)
{
    m_user_entred = false;
    
}

void Param::printParam() 
{
    assert(m_label != "\0" && "param label is undefined");
    string req;
    
    req = m_required ? " {Required} ": "";
    
    std::cout << "\t" << m_param_flag << " \t:" << m_label << req << " - " << m_desc << std::endl;
}

void Param::set_param_desc(const string& desc)
{
    m_desc = desc;
}

// class Parser


Parser::Parser(int argc, char** argv, bool err_usage) : m_argc(argc), m_err_usage(err_usage)
{
    // Parser constructor: copies argc and argv into private members 
    // args: int argc, char** argv
    m_argv.resize(argc);
    m_args_processed.resize(argc - 1);

    for (int i = 0; i < argc; i++)
    {
        m_argv[i] = argv[i];
    }


}

//// initialization of the parser ======

bool Parser::is_flag(const string& str)
{
    // is_flag - UNFINISHED: checks if argv[i] string is a flag by its prefix "-"/"--" etc.
    // args: str
    // returns: bool (flag/ no flag (param/flag value)
    if (str == "\0" || str.length() <= 1)
    {
        return false;
    }
    else if (str[0] == '-')
    {
        return true;
    }
    else
        return false;
}

void Parser::match_flag(string flag, const string& label)
{
    // set_flag - adds a flag: label as key: value to map m_flags. this is to map flags to Params objects when used by progrem usercast_to
    // args: flag (string), label (string)
    // returns: 

    stringstream sstream{ flag };
    string f;
    while (sstream >> f)
    {
        if (is_flag(f))
        {
            m_flags[f] = label;
        }
        else
        {
            string exception{ f + " is not in flag foramt\n" };
            throw exception;
        }
    }
}

// flag activation and value assigning =====

void Parser::assign_passive_value(Param& p, const size_t& arg)
{
    // assign_passive_value - assigns a value to flag ("passive" - e.i without adjacent value). bool value is stored in Param object as string "1".
    // args:
    // returns:

    p.m_type = "bool";
    p.m_value.set_value("1");
    m_args_processed[arg - 1] = true;
}

void Parser::assign_adj_value(Param& p, const size_t& arg)
{
    // assign_adj_value - assigns a value to parameter. value is stored in Param object as string.
    // args:
    // returns:

    if (arg == m_argv.size() - 1 || is_flag(m_argv[arg + 1]))
    {
        string exception{ "flag: " + m_argv[arg] + " has no adjecent value\n" };

        if (p.m_type == "bool")
        {
            exception += " (paramater was defined as boolean. should it be defined as passive?)\n";
        }
        throw exception;
    }
    string value = m_argv[arg + 1];
    p.m_value = value;
    m_args_processed[arg - 1] = true;
    m_args_processed[arg] = true;
}

void Parser::process_flag(string& label, size_t arg)
{
    // process_flag - assigns param/flag (in use by program user) it's designated value. 
    //              - distinguishes between flags (no adjacent values - boolean) and parameters (with adjacent value)
    // args:
    // returns: 

    

    if (label_exists(label))
    {
        Param& p = m_params[label];
        if (p.m_user_entred)
        {
            string exception{ "multiple values passed for parameter: \"" + label + "\"\n"};
            throw exception;
        }
        if (p.m_passive)
        {
            assign_passive_value(p, arg);
        }
        else
        {
            assign_adj_value(p, arg);
        }

        if (m_args_required.find(label) != m_args_required.end())
        {
            m_args_required[label] = false;
        }

        p.m_user_entred = true;
    }
    else
    {
            string exception{ label + " - label not defined\n" };
            throw exception;
    }
}

bool Parser::label_exists(const string& label)
{
    // label_exists - check if passed "label" is already a key in map m_params
    // args: label (string) 
    
    if (m_params.find(label) != m_params.end())
    {
        return true;
    }
    else
        return false;
}

//// client API =====


void Parser::set_param(const string& flag,
                        const string& label,    
                        const string& type, 
                        const bool& required, 
                        const string& desc, 
                        const bool& passive)
{
    // set_param - initialize and add a Param object (using args) to Parser.m_params map with asigned "label"
    // args: flag(string), label(string), type(string), desc(string), required(bool), passive(bool)
    // returns:

    if (label_exists(label))
    {
        std::cerr << "Label: " << label << " is allready defined. cannot define label more then once. " << std::endl;
        return;
    }

    Param p{ label, flag, type, desc, required ,passive }; 
    m_params[label] = p;
    
    
    if (required)
    {
        m_args_required[label] = true;
    }
    
    match_flag(flag, label);

}

void Parser::set_flag(const string& flag, const string& label, const string& type, const bool& required, const string& desc)
{
    set_param(flag, label, type, required, "", true);
}




void Parser::printHelp()
{
    // printHelp - iterate through all set parameters and for each calls Param::printParam()
    std::cout << "\n" << m_general_desc << "\n";
    for (auto& p : m_params)
    {
        p.second.printParam();
    }
}

void Parser::printUsage()
{
    // printUsage - UNFINISHED
    // prints example of program call with all flags/ params of sort: "./prog -x -y -z" followed by printHelp call
    std::cout << "Usage: " << m_argv[0] << " ";

    for (auto& p : m_params)
    {
        if (p.second.m_required)
        {
            std::cout << "{" << p.second.m_param_flag << "} ";
        }
        else {
            std::cout << "[" << p.second.m_param_flag << "] ";
        }
    }
    std::cout << "\n";
    printHelp();
}

void Parser::printFlags()
{
    // printFlags - prints all flags set by writer

    for (auto& f : m_flags)
    {
        std::cout << f.first << "\n";
    }
}


void Parser::search_active_flags() 
{ 
    // search_active_flags - proccess program-user's CL arguments and checks which of the set parameters and flags are active/ used.
    //                     - calls assign_flag() to assign each flag/param with its designated value.
    //                     - keep tracks of number of processed flags and errors if non-set flags were passed. 
    // args:
    // returns:

    try
    {
        for (size_t ar = 1; ar < m_argv.size(); ar++)
        {
            if (is_flag(m_argv[ar]))
            {
                if (m_flags.find(m_argv[ar]) != m_flags.end())
                {
                    process_flag(m_flags[m_argv[ar]], ar);
                }
                else
                {
                    string exception{ m_argv[ar] + " - flag not defined\n" };
                    throw exception;
                }

            }


        }
        for (bool arg : m_args_processed)
        {
            if (!arg)
            {
                throw "Not all argument were correctly processed.\n";
            }
        }

        string required_args = "\0";
        for (auto arg : m_args_required)
        {
            if (arg.second)
            {
                required_args += arg.first + " ";
            }
        }
        if (required_args != "\0")
        {
            string exception{ "Required arguments: " + required_args + "\n"};
            throw exception;
        }
    }
    catch (string& exception)
    {
        std::cerr << exception << " [Parser.search_active_flags]" << std::endl;
        if (m_err_usage)
        {
            printUsage();
        }
        exit(0);
    }
    
}

void Parser::set_general_desc(const string& str)
{
    m_general_desc = str;
}




string Parser::var(const string& label) 
{
    // var - overloaded function to return parameter value of string types
    // args: label (name of the variable)
    // returns: string variable of parameter "label"
    try
    { 
        if (label_exists(label))
        {
            Param& p = m_params[label];
            if (!p.m_user_entred)
            {
                string exception{ "lable: \"" + label + " was not passed by user, therefor cannot be extracted.\n" };
                throw exception;
            }
            if (p.m_type == "string") {
                return p.m_value.get_str();
            }
            else {
                string exception{ "unsuitable Parser.var() call for lable - use template function Parser.var<T>(): \"" + label + ".\n" };
                throw exception;
            }
        }
        else {
            string exception{ "label: \"" + label + "\" is not defined.\n" };
            throw exception;
        }
    }
    catch (string& exception)
    {
        std::cerr << exception << " [Parser.var()]" << std::endl;
        if (m_err_usage)
        {
            printUsage();
        }
        exit(0);
    }
    return NULL;
}

Param* Parser::operator[](const string& label)
{
    // overloaded operator[] 
    // args: "label" (string name of parameter)
    // returns: pointer to Param object with name label

    if (!label_exists(label))
    {
        string exception{ "label: \"" + label + "\" is not defined.\n" };
        throw exception;
    }
    else 
    {
        return &m_params[label];
    }
}

bool Parser::is_avail(const string& label)
{
    // is_set - checks if a parameter was defined by prog writer AND set by prog user (is so, returns true)
    // args: label (name of the variable)
    // returns: bool 

    if (label_exists(label))
    {
        return (m_params[label].m_user_entred);
    }
    return false;
}

Parser::~Parser()
{
    m_argv.clear();
    m_params.clear();
    m_flags.clear();
    m_args_processed.clear();
    m_args_required.clear();
    m_general_desc.clear();
}

