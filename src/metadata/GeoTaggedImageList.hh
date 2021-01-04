//
// Created by dotty on 12/9/20.
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

    void PopulateImages(boost::filesystem::path path);

    void clear();

    [[nodiscard]] GeoTaggedImage *CurrentImage() const;

    void NextImage();

    void PreviousImage();

    [[nodiscard]] int GetListPosition() const;

    [[nodiscard]] int GetListLength() const;

    std::vector<GeoTaggedImage>::iterator it_;
    std::vector<GeoTaggedImage> geoTaggedImageList_;
};


#endif //TELE_GEOTAGGEDIMAGELIST_HH
