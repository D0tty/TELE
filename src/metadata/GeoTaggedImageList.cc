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
    this->it_ = this->geoTaggedImageList_.begin();
}

void GeoTaggedImageList::NextImage() {
    if (this->it_ != --this->geoTaggedImageList_.end()) {
        this->it_++;
    }
}

void GeoTaggedImageList::PreviousImage() {
    if (this->it_ != this->geoTaggedImageList_.begin()) {
        this->it_--;
    }
}

int GeoTaggedImageList::GetListPosition() const {
    return 1 + this->it_ - this->geoTaggedImageList_.begin();
}

int GeoTaggedImageList::GetListLength() const {
    return this->geoTaggedImageList_.size();
}
