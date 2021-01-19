//
// Created by Thomas Michelot on 12/4/20.
//

#include "misc.hh"

std::vector<boost::filesystem::path> getDirectoryFiles(boost::filesystem::path &path) {
    std::vector<boost::filesystem::path> files;

    try {
        for (auto &entry: boost::filesystem::directory_iterator(path)) {
            if (boost::filesystem::is_regular_file(entry.path())) {
                files.push_back(entry.path());
            }
        }
    }
    catch (const boost::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        exit(2);
    }

    std::sort(files.begin(), files.end());

    return files;
}
