//
// Created by dotty on 12/4/20.
//

#include "misc.hh"

int strEqual(char *s1, std::string s2) {
    return s2.compare(s1) == 0;
}

void usage(char **argv) {
    std::string name(argv[0]);
    std::cout << "Usage: " << name << " -d <directory>" << std::endl;
}
