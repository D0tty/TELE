//
// Created by dotty on 12/9/20.
//

#ifndef TELE_GEOTAGGEDIMAGE_HH
#define TELE_GEOTAGGEDIMAGE_HH

#include "data/Coordinate.hh"
#include "data/Image.hh"

#include <utility>

class GeoTaggedImage {
public:
    explicit GeoTaggedImage(boost::filesystem::path path)
            : path_(std::move(path)), coordinate_longlat_(), coordinate_xy_() {}

    Coordinate<double> coordinate_longlat_; // cyclone center coordinate (long, lat)
    Coordinate<int> coordinate_xy_;      // pixel image values
    boost::filesystem::path path_;
};


#endif //TELE_GEOTAGGEDIMAGE_HH
