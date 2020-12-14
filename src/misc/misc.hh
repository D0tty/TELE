//
// Created by dotty on 12/4/20.
//

#ifndef TELE_MISC_HH
#define TELE_MISC_HH

#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/range/iterator.hpp>

/**
 * takes path of dir, returns a lexicographically sorted vector of paths
 * @param path self explanatory
 * @return sorted vector of path to images
 */
std::vector<boost::filesystem::path> getDirectoryFiles(boost::filesystem::path &path);

int isPath(std::string &strPath);

int strEqual(char *s1, const std::string &s2);

void usage(char *argv[]);

#endif //TELE_MISC_HH
