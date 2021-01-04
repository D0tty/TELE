//
// Created by dotty on 12/9/20.
//

#ifndef TELE_GEOTAGGEDIMAGE_HH
#define TELE_GEOTAGGEDIMAGE_HH

#include "data/Coordinate.hh"
#include <boost/filesystem/path.hpp>
#include <utility>

class GeoTaggedImage {
public:
    explicit GeoTaggedImage(boost::filesystem::path path, int index)
            : path_(std::move(path)), coordinate_xy_(), index_(index) {}

    Coordinate<int> coordinate_xy_;      // pixel image values
    boost::filesystem::path path_;
    int index_;
};


#endif //TELE_GEOTAGGEDIMAGE_HH
