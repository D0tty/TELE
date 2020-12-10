#include "misc/misc.hh"
#include "metadata/GeoTaggedImageList.hh"

int main(int argc, char *argv[]) {
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

    std::string strDirectoryPath(argv[2]);
    if (!isPath(strDirectoryPath)) {
        std::cout << "This is not a directory: " << strDirectoryPath << std::endl;
        exit(2);
    }

    boost::filesystem::path directoryPath(strDirectoryPath);
    auto &geoTaggedImageList = GeoTaggedImageList::instance();
    geoTaggedImageList.PopulateImages(directoryPath);

    return 0;
}