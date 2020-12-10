//
// Created by dotty on 12/9/20.
//

#ifndef TELE_GEOTAGGEDIMAGE_HH
#define TELE_GEOTAGGEDIMAGE_HH

#include "data/Coordinate.hh"
#include "data/Image.hh"

class GeoTaggedImage {
public:
    explicit GeoTaggedImage(const boost::filesystem::path &path) : image_(path), coordinate_() {}

    Coordinate coordinate_;
    Image image_;
};


#endif //TELE_GEOTAGGEDIMAGE_HH
