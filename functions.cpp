#include <filesystem>
#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include <cstdlib>

using namespace std;

string runCommand(const char* command){
    FILE* pipe = popen(command, "r");
    if (!pipe) return "";

    char buffer[256];
    string output;

    while (fgets(buffer, sizeof(buffer), pipe))
        output += buffer;

    pclose(pipe);
    return output;
}

vector<string> split_lines(const string &input_string){
    vector<string> output;

    size_t start = 0;
    while (start < input_string.size()) {
        const size_t end = input_string.find('\n', start);
        if (end == string::npos) {
            output.push_back(input_string.substr(start));
            break;
        }

        output.push_back(input_string.substr(start, end - start));
        start = end + 1;
    }

    return output;
}

vector<string> getThemes(){
    string raw_output = runCommand("ls ~/.icons");
    return split_lines(raw_output);
}

string getCurrTheme(){
    string raw_output = runCommand("cat ~/.config/hypr/variables.lua | grep 'cursorTheme'");

    const size_t quoteStart = raw_output.find('"');
    if (quoteStart == string::npos) return "";

    const size_t quoteEnd = raw_output.find('"', quoteStart + 1);
    if (quoteEnd == string::npos) return "";

    return raw_output.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
}

string getCurrThemeSize(){
    string raw_output = runCommand("cat ~/.config/hypr/variables.lua | grep 'cursorSize'");

    const size_t eq = raw_output.find('=');
    if (eq == string::npos) return "24";

    const size_t comma = raw_output.find(',', eq);
    if (comma == string::npos) return "24";

    string val = raw_output.substr(eq + 1, comma - eq - 1);

    const size_t first = val.find_first_not_of(" \t");
    const size_t last  = val.find_last_not_of(" \t\r\n");
    if (first == string::npos) return "24";

    return val.substr(first, last - first + 1);
}

bool is_member_of(const string& element, const vector<string>& list) {
    return find(list.begin(), list.end(), element) != list.end();
}

vector<string>readFile(const string& path){
    vector<string> lines;

    ifstream file(path);

    if(!file.is_open()){
        cout << "Error while reading file" << endl;
    }

    string line;

    while(getline(file, line)){
        lines.push_back(line);
    }

    return lines;
}