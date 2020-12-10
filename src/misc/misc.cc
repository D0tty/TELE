//
// Created by dotty on 12/4/20.
//

#include "misc.hh"

std::vector<boost::filesystem::path> getDirectoryFiles(boost::filesystem::path &path) {
    std::vector<boost::filesystem::path> files;

    try {
        for (auto &entry: boost::filesystem::directory_iterator(path)) {
            if (is_regular_file(entry.path())) {
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

int isPath(std::string &strPath) {
    auto p = boost::filesystem::path(strPath);
    if (boost::filesystem::exists(p)) {
        return boost::filesystem::is_directory(p);
    }
    return false;
}

int strEqual(char *s1, const std::string &s2) {
    return s2 == s1;
}

void usage(char **argv) {
    std::string name(argv[0]);
    std::cout << "Usage: " << name << " -d <directory>" << std::endl;
}