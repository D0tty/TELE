//
// Created by Thomas Michelot on 12/9/20.
//

#ifndef TELE_GEOTAGGEDIMAGE_HH
#define TELE_GEOTAGGEDIMAGE_HH

#include "data/Coordinate.hh"
#include <boost/filesystem/path.hpp>
#include <utility>

class GeoTaggedImage {
public:
    explicit GeoTaggedImage(boost::filesystem::path path, int index)
            : path_(std::move(path)), coordinateXY_(), index_(index) {}

    Coordinate<int> coordinateXY_;      // pixel image values
    boost::filesystem::path path_;
    int index_;
};


#endif //TELE_GEOTAGGEDIMAGE_HH
