#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

using namespace std;

string runCommand(const char* command);

vector<string> split_lines(const string &input_string);

vector<string> getThemes();

string getCurrTheme();

string getCurrThemeSize();

bool is_member_of(const string& element, const vector<string>& list);

vector<string> readFile(const string& path);

#endif