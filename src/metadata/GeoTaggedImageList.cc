//
// Created by dotty on 12/9/20.
//

#include <misc/misc.hh>
#include "GeoTaggedImageList.hh"

void GeoTaggedImageList::PopulateImages(boost::filesystem::path path) {
    std::vector<boost::filesystem::path> files = getDirectoryFiles(path);
    for (boost::filesystem::path &p : files) {
        this->geoTaggedImageList_.emplace_back(GeoTaggedImage(p));
    }
}
