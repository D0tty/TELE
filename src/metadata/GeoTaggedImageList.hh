//
// Created by Thomas Michelot on 12/9/20.
//

#ifndef TELE_GEOTAGGEDIMAGELIST_HH
#define TELE_GEOTAGGEDIMAGELIST_HH

#include "GeoTaggedImage.hh"
#include <boost/filesystem/path.hpp>
#include <misc/Singleton.hh>
#include <vector>
#include <iterator>

class GeoTaggedImageList final : public Singleton<GeoTaggedImageList> {
public:
    GeoTaggedImageList() : geoTaggedImageList_(), it_() {}

    void populateImages(boost::filesystem::path path);

    void clear();

    [[nodiscard]] std::vector<GeoTaggedImage>::iterator currentImage() const;

    void nextImage();

    void previousImage();

    [[nodiscard]] long getListPosition() const;

    [[nodiscard]] long getListLength() const;

    [[nodiscard]] bool allPointsSet() const;

    std::vector<GeoTaggedImage>::iterator it_;
    std::vector<GeoTaggedImage> geoTaggedImageList_;
};


#endif //TELE_GEOTAGGEDIMAGELIST_HH
