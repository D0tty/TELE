//
// Created by Thomas Michelot on 12/4/20.
//

#ifndef TELE_MISC_HH
#define TELE_MISC_HH

#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/range/iterator.hpp>

/**
 * takes path of dir, returns a lexicographically sorted vector of paths
 * @param path self explanatory
 * @return sorted vector of path to images
 */
std::vector<boost::filesystem::path> getDirectoryFiles(boost::filesystem::path &path);

#endif //TELE_MISC_HH
