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

int main(int argc, char* argv[]) {
    const vector<string> arguments = {"--set-size", "--set-theme", "--help", "--current-theme", "-S", "-T", "-H", "-C"};

    if(argc < 2){
        cout << "Invalid Syntax.\nUse caelestia-cursor --help for further information." << endl;
        return 1;
    }
    else if(!is_member_of(argv[1], arguments)){
        cout << "Invalid Syntax.\nUse caelestia-cursor --help for further information." << endl;
        return 1;
    }
    else if(is_member_of(argv[1], arguments)){
        const string option = argv[1];

        if(option == "--help" || option == "-H"){
            cout << "Use: Caelestia-Cursor [OPTION] Value\n" << endl; 
            cout << "--help or -H : Prints this manual" << endl;
            cout << "--set-size or -S SIZE : Sets the cursor to the size refered in SIZE (SIZE is the INTEGER size in pixels of the cursor) " << endl;
            cout << "--set-theme or -T THEME-NAME : Sets the theme by its givven name (NOT THE FOLDER NAME) Tip: check the index.html of that theme for the name" << endl;
            cout << "--current-theme or -C : Prints the current cursor theme" << endl;
        }
        else if(option == "--current-theme" || option == "-C"){
            cout << "Current theme: " + getCurrTheme() << endl;
        }
        else if(option == "--set-size" || option == "-S"){
            if (argc < 3) {
                cout << "Missing cursor size." << endl;
                return 1;
            }

            string command = "hyprctl setcursor";
            command = command + getCurrTheme() + argv[2];

            cout << runCommand(command.c_str()) << endl; 
        }
        else if(option == "--set-theme" || option == "-T"){
            if (argc < 3) {
                cout << "Missing theme name." << endl;
                return 1;
            }

            vector<string> themes = getThemes();
            if(is_member_of(argv[2], themes) == false){
                cout << "Invalid theme!" << endl;
                cout << "Use one of the following:" << endl;
                for (const auto& theme : themes) {
                    cout << theme << endl;
                }
            }
            else {
                cout << "The theme is valid" << endl;
                const char* home = getenv("HOME");
                if (home == nullptr || home[0] == '\0') {
                    cerr << "Unable to determine the home directory." << endl;
                    return 1;
                }

                vector<string> lines = readFile(string(home) + "/.config/hypr/variables.lua");

                auto it = find(lines.begin(), lines.end(),"    cursorTheme                = \"" + getCurrTheme() + "\",");

                if(it != lines.end()){
                    string BAK_PATH = string(home) + "/.config/hypr/variables_BAK.lua";

                    ifstream bak_file_in(BAK_PATH);
                    if(!bak_file_in.is_open()){
                        ofstream bak_file_out(BAK_PATH);
                        
                        for(const auto& i: lines){
                            bak_file_out << i << '\n';
                        }

                        cout << "As you did't have a backup of the variables.lua file, a backup has been created for you, and it is located at " + BAK_PATH << endl;
                    }

                    cout << lines[distance(lines.begin(), it)] << endl;
                    lines[distance(lines.begin(), it)] = string("    cursorTheme                = \"") + argv[2] + "\",";
                    
                    string PATH = string(home) + "/.config/hypr/variables.lua";

                    ofstream file(PATH);
                    
                    for(const auto& i: lines){
                        file << i << '\n';
                    }
                    
                }
                else cout << "Cursor theme config line not found on the config file! \nAre you using caelestia? \n\tIf yes, did you update it?\n\tYou can update caelestia using 'caelestia update'." << endl;


            }
        }
    }
    return 0;
}
