//
// Created by dotty on 12/4/20.
//

#ifndef TELE_MISC_HH
#define TELE_MISC_HH

#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>

std::vector<boost::filesystem::path> getDirectoryFiles(boost::filesystem::path &path);
int isPath(std::string &strPath);
void usage(char *argv[]);
int strEqual(char *s1, std::string s2);
#endif //TELE_MISC_HH
