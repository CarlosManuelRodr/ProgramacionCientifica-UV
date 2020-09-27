#pragma once
#include <string>
#include <initializer_list>

bool directory_exist(std::string path);
bool file_exist(std::string fileName);
std::string get_app_folder();
void create_directory(std::string directory_name);
std::string filename_join(std::initializer_list<std::string> path);
std::string file_basename(const std::string path);
std::string file_extension(const std::string filepath);