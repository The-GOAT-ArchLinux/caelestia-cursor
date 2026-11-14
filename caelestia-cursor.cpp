#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdlib>

#include "functions.h"

using namespace std;

int main(int argc, char* argv[]) {
    const vector<string> arguments = {"--set-size", "--set-theme", "--help", "--current-theme", "-S", "-T", "-H", "-C"};

    if(argc < 2){
        std::cout << "Invalid Syntax.\nUse caelestia-cursor --help for further information." << endl;
        return 1;
    }
    else if(!is_member_of(argv[1], arguments)){
        std::cout << "Invalid Syntax.\nUse caelestia-cursor --help for further information." << endl;
        return 1;
    }
    else if(is_member_of(argv[1], arguments)){
        const string option = argv[1];

        if(option == "--help" || option == "-H"){
            std::cout << "Use: Caelestia-Cursor [OPTION] Value\n" << endl; 
            std::cout << "--help or -H : Prints this manual" << endl;
            std::cout << "--set-size or -S SIZE : Sets the cursor to the size refered in SIZE (SIZE is the INTEGER size in pixels of the cursor) " << endl;
            std::cout << "--set-theme or -T THEME-NAME : Sets the theme by it's folder name" << endl;
            std::cout << "--current-theme or -C : Prints the current cursor theme" << endl;
        }
        else if(option == "--current-theme" || option == "-C"){
            std::cout << "Current theme: " + getCurrTheme() << endl;
        }
        else if(option == "--set-size" || option == "-S"){
            if (argc < 3) {
                std::cout << "Missing cursor size." << endl;
                return 1;
            }
            else if(atoi(argv[2]) < 8 || atoi(argv[2]) > 128){
                std::cout << "The size must be between 8 and 128" << endl;
            }
            else{
                const char* home = getenv("HOME");
                if (home == nullptr || home[0] == '\0') {
                    cerr << "Unable to determine the home directory." << endl;
                    return 1;
                }

                vector<string> lines = readFile(string(home) + "/.config/hypr/variables.lua");

                auto it = find(lines.begin(), lines.end(),"    cursorSize                 = " + getCurrThemeSize() + ",");

                if(it != lines.end()){
                    string BAK_PATH = string(home) + "/.config/hypr/variables_BAK.lua";

                    ifstream bak_file_in(BAK_PATH);
                    if(!bak_file_in.is_open()){
                        ofstream bak_file_out(BAK_PATH);
                        
                        for(const auto& i: lines){
                            bak_file_out << i << '\n';
                        }

                        std::cout << "As you did't have a backup of the variables.lua file, a backup has been created for you, and it is located at " + BAK_PATH << endl;
                    }

                    lines[distance(lines.begin(), it)] = string("    cursorSize                 = ") + argv[2] + ",";
                    
                    string PATH = string(home) + "/.config/hypr/variables.lua";

                    ofstream file(PATH);
                    
                    for(const auto& i: lines){
                        file << i << '\n';
                    }
                    file.close();

                    string output = runCommand(string("hyprctl setcursor " + getCurrTheme() + " " + argv[2]).c_str());
                    if(output.find("ok") == string::npos){
                        std::cout << "The cursor theme has been changed to: " + string(argv[2]) << endl;
                    }
                }
                else std::cout << "Cursor theme config line not found on the config file! \nAre you using caelestia? \n\tIf yes, did you update it?\n\tYou can update caelestia using 'caelestia update'." << endl;
            }
        }
        else if(option == "--set-theme" || option == "-T"){
            if (argc < 3) {
                std::cout << "Missing theme name." << endl;
                return 1;
            }

            vector<string> themes = getThemes();
            if(is_member_of(argv[2], themes) == false){
                std::cout << "Invalid theme!" << endl;
                std::cout << "Use one of the following:" << endl;
                for (const auto& theme : themes) {
                    std::cout << theme << endl;
                }
            }
            else {
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

                        std::cout << "As you did't have a backup of the variables.lua file, a backup has been created for you, and it is located at " + BAK_PATH << endl;
                    }

                    lines[distance(lines.begin(), it)] = string("    cursorTheme                = \"") + argv[2] + "\",";
                    
                    string PATH = string(home) + "/.config/hypr/variables.lua";

                    ofstream file(PATH);
                    
                    for(const auto& i: lines){
                        file << i << '\n';
                    }
                    file.close();
                    runCommand(string("hyprctl setcursor " + string(argv[2]) + " " + getCurrThemeSize()).c_str());
                    runCommand(string("gsettings set org.gnome.desktop.interface cursor-theme '" + string(argv[2]) + "'").c_str());
                    std::cout << "The cursor theme has been changed to: " + string(argv[2]) << endl;
                    
                }
                else std::cout << "Cursor theme config line not found on the config file! \nAre you using caelestia? \n\tIf yes, did you update it?\n\tYou can update caelestia using 'caelestia update'." << endl;


            }
        }
    }
    return 0;
}