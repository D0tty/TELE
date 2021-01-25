//
// Created by Thomas Michelot on 12/9/20.
//

#include <misc/misc.hh>
#include "GeoTaggedImageList.hh"

void GeoTaggedImageList::populateImages(boost::filesystem::path path) {
    std::vector<boost::filesystem::path> files = getDirectoryFiles(path);
    int index = 0;
    for (boost::filesystem::path &p : files) {
        this->geoTaggedImageList_.emplace_back(GeoTaggedImage(p, index++));
    }
    this->it_ = this->geoTaggedImageList_.begin();
}

std::vector<GeoTaggedImage>::iterator GeoTaggedImageList::currentImage() const {
    return this->it_;
}

void GeoTaggedImageList::nextImage() {
    if (this->it_ != --this->geoTaggedImageList_.end()) {
        this->it_++;
    }
}

void GeoTaggedImageList::previousImage() {
    if (this->it_ != this->geoTaggedImageList_.begin()) {
        this->it_--;
    }
}

long GeoTaggedImageList::getListPosition() const {
    return this->it_ - this->geoTaggedImageList_.begin();
}

long GeoTaggedImageList::getListLength() const {
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
                return img.coordinateXY_.getX() != 0 && img.coordinateXY_.getY() != 0;
            });
}
