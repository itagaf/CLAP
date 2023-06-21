
#ifndef	CLAP_H
#define CLAP_H





#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <sstream>


using namespace std;
template <typename T>
struct val_return;
//{
//    using type = T;
//};
namespace TYPE_MAP
{
    static map<string, int> type_map{ { "int",            0 },
                                    { "long",           1 },
                                    { "long long",      2 },
                                    { "float",          3 },
                                    { "double",         4 },
                                    { "long double",    5 },
                                    { "bool",           6 } };

}

class ParamValue
{
private:
    string m_str; // parameter value - stored in the form of string. cast to actual type on calling Parser::var()

public:
    ParamValue(string& str);

    ParamValue();

    void set_value(const string& str);

    bool cast_to_bool(const string& str);

    string get_str();

    template <typename T>
    void cast_value(T& val, const string& type)
    {
        if (TYPE_MAP::type_map.find(type) == TYPE_MAP::type_map.end())
        {
            string exception{ "unsupported type : " + type };
            throw exception;
        }
        int type_code = TYPE_MAP::type_map[type];

        switch (type_code)
        {
        case 0:         val = std::stoi(m_str);     break;
        case 1:         val = std::stol(m_str);     break;
        case 2:         val = std::stoll(m_str);    break;
        case 3:         val = std::stof(m_str);     break;
        case 4:         val = std::stod(m_str);     break;
        case 5:         val = std::stold(m_str);    break;
        case 6:         val = cast_to_bool(m_str); break;

        default:
            string exception{ "Cannot cast value \"" + m_str + "\" to type \"" + type + "\" [ParamValue.cast_value(const string & type)]\n" };
            throw exception;
        };
    }
};




struct Param
{
    string m_label; // name to be used to extract parameter's value (var function)
    string m_desc; // description of parameter to be prompted (help fuction)
    string m_param_flag; // form of invoking the parameter by user (e.g. "-t" / "--read")
    string m_type; // premitive type of parameter
    bool m_required; // required argument for program. naturaly evokes error if not passed
    bool m_passive; // flag is/ is not followed by value (false if not)
    bool m_user_entred; // tracks if argument was entred by progrem user

    ParamValue m_value; // value object of the parameter

    Param(const string& label, const string& flag, const string& type, const string& desc, const bool& required, const bool& passive);

    void printParam();

    void set_param_desc(const string& desc);
};



  
class Parser
{
private:

    int m_argc;                 // holds argc of program written by user
    vector<string> m_argv;      // holds argv of program written by user

    bool m_err_usage;           // if true, calls print_usage when error occurs

    map<string, Param> m_params;   // map to hold Param objects and to be accessed by "label" (variables names)
    map<string, string> m_flags;   // translates flags to labels (variables names)
    
    vector<bool> m_args_processed; // keep track of proccessed args
    map<string, bool> m_args_required; // keep track of proccessed args

    string m_general_desc;         // desctription of program 
public:
    Parser(int argc, char** argv, bool err_usage=false);
    
    //// initialization of the parser ======

private:

    bool is_flag(const string& str);

    void match_flag(string flag, const string& label);

    //// flag activation and value assigning =====

    void assign_passive_value(Param& p, const size_t& arg);

    void assign_adj_value(Param& p, const size_t& arg);

    void process_flag(string& label, size_t arg);

    bool label_exists(const string& label);

    //// client API =====
public:
    // 
    void set_param(const string& flag, const string& label, const string& type, const bool& required = false, const string& desc = "", const bool& passive = false);

    void set_flag(const string& flag, const string& label, const string& type, const bool& required=false, const string& desc = "");

    void printUsage();

    void printHelp();

    void printFlags();

    void search_active_flags();

    void set_general_desc(const string& str);
    
    template<typename T>
    val_return<T>::type var(const string& label)
    {
        // var - overloaded template function to return parameter value of the various types supported by the ParamValue::cast_value function
        // args: label (name of the variable)
        // returns: string variable of parameter "label"
        try
        {
            if (label_exists(label))
            {
                Param& p = m_params[label];
                if (!p.m_user_entred)
                {
                    string exception{ "label: \"" + label + "\" was not passed by user, therefor cannot be extracted.\n" };
                    throw exception;
                }
                T val{ NULL };

                if (p.m_type == "string")
                {
                    string exception{ "label: \"" + label + "\" should be invoked with var() function (not var<T>()).\n" };
                    throw exception;
                }

                p.m_value.cast_value(val, p.m_type);
                return val;
            }
            else 
            {
                string exception{ "label: \"" + label + "\" is not defined.\n" };
                throw exception;
            }

        }
        catch (const string& exception)
        {
            std::cerr << exception << " [Parser.var<T>(const string& label)]" << std::endl;
            
            exit(0);
        }
        return NULL;
    }


    string var(const string& label);

    Param* operator[](const string& label);

    bool is_avail(const string& label);

    ~Parser();
    
};
#include "CLAP.cpp"
#endif