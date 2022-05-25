#include<iostream>
#include<unordered_map>
#include<string>
#include<fstream>

using std::string;
using argMap = std::unordered_map<string, string>;


// --- functions to generate an unordered map of command line args ---
int FindEquals(string str) { return str.find("="); }

string GetKey(string str) { return str.substr(0, FindEquals(str)); }

string GetValue(string str)
{
    return (FindEquals(str) != -1) ? str.substr(FindEquals(str) + 1, (int)str.size()) : "";
}

argMap GenerateMap(int Argc, char* Argv[])
{
    argMap argMap{};
    for(int n=1; n<Argc; n++) // start iterating at n=1 to ignore the argv[0] exe file path
    {
        if(argMap.count( GetKey(Argv[n]) )) // prohibits multiple read/write per command
        {
            std::cout << "Multiple read/write commands supplied, input a maximum of one each.\n";
        }
        else{ argMap[GetKey(Argv[n])] = GetValue(Argv[n]); }
    }
    return argMap;
}


// --- execute commands within the map ---
void ReadFile(string file_name)
{
    if(std::ifstream read_stream{file_name, std::ios::binary | std::ios::ate})
    {
        auto file_size = read_stream.tellg();
        string str(file_size, '\0');
        read_stream.seekg(0);
        if(read_stream.read(&str[0], file_size))
        {
            std::cout << str << "\n";
        }
        read_stream.close();
    }
    else
    {
        std::cout << "Could not open the file to be read.\n";
    }

}

void WriteFile(string file_name, string output)
{
    std::cout << "Creating file: " << file_name << "\n";
    std::ofstream write_stream(file_name);
    write_stream << output << std::endl;
    write_stream.close();
    std::cout << "Text has been written to the file.\n";
}

template<typename K, typename V>
void ExecuteMapCommands(std::unordered_map<K, V>& m)
{
    for (auto& [key,val] : m)
    {
        if(key == "-read")
        {
            (val == "") 
            ? std::cout << "Invalid -read command: filename not provided." 
            : ReadFile(val);
        }
        else if(key == "-write")
        {
            (val == "") 
            ? std::cout << "Invalid -write command: filename not provided."
            : WriteFile(val, m["-text"]);
        }
        else if(key != "-text")
        {
            std::cout << "Invalid command line argument supplied.";
        }
    }
}



int main(int argc, char* argv[])
{   system("cls");

    argMap aMap = GenerateMap(argc, argv);
    ExecuteMapCommands(aMap);
    return 0;
}