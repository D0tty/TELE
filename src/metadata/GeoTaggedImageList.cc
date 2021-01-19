//
// Created by Thomas Michelot on 12/9/20.
//

#include <misc/misc.hh>
#include "GeoTaggedImageList.hh"

void GeoTaggedImageList::PopulateImages(boost::filesystem::path path) {
    std::vector<boost::filesystem::path> files = getDirectoryFiles(path);
    int index = 0;
    for (boost::filesystem::path &p : files) {
        this->geoTaggedImageList_.emplace_back(GeoTaggedImage(p, index++));
    }
    this->it_ = this->geoTaggedImageList_.begin();
}

GeoTaggedImage *GeoTaggedImageList::CurrentImage() const {
    return this->it_.base();
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

long GeoTaggedImageList::GetListPosition() const {
    return this->it_ - this->geoTaggedImageList_.begin();
}

long GeoTaggedImageList::GetListLength() const {
    return this->geoTaggedImageList_.size();
}

void GeoTaggedImageList::clear() {
    this->geoTaggedImageList_.clear();
}

bool GeoTaggedImageList::allPointsSet() const {
    return std::all_of(
            this->geoTaggedImageList_.begin(),
            this->geoTaggedImageList_.end(),
            [](const GeoTaggedImage &img) {
                return img.coordinate_xy_.getX() != 0 && img.coordinate_xy_.getY() != 0;
            });
}
