//
// Created by dotty on 12/9/20.
//

#ifndef TELE_GEOTAGGEDIMAGELIST_HH
#define TELE_GEOTAGGEDIMAGELIST_HH

#include "GeoTaggedImage.hh"
#include <boost/filesystem/path.hpp>
#include <misc/singleton.hh>
#include <vector>

class GeoTaggedImageList final : public Singleton<GeoTaggedImageList> {
public:
    GeoTaggedImageList() : geoTaggedImageList_() {}

    void PopulateImages(boost::filesystem::path path);

    std::vector<GeoTaggedImage> geoTaggedImageList_;
};


#endif //TELE_GEOTAGGEDIMAGELIST_HH
