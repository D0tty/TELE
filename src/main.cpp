#include "misc/misc.hh"

int main(int argc, char *argv[])
{
    if (argc == 2 && strEqual(argv[1], "-h")) {
        usage(argv);
        return 0;
    }
    if (argc != 3) {
        usage(argv);
        exit(2);
    }
    if (!strEqual(argv[1], "-d")) {
        usage(argv);
    }

    std::string directoryPath(argv[2]);
    return 0;
}