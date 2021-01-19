//
// Created by Thomas Michelot on 1/4/21.
//

#include "Export.hh"

bool Export::exportCSV(const std::string &path) {
    std::fstream outStream;

    outStream.open(path, std::fstream::out);
    if (outStream.fail() || !outStream.good() || outStream.bad()) {
        return false;
    }

    auto writer = csv::make_csv_writer(outStream);
    static const auto header = std::vector<std::string>(
            {"Image ID",
             "Line",
             "Column",
             "Latitude",
             "Longitude",
             "Speed (km/h)",
             "Orientation (degree)"});

    writer << header;

    int i;
    for (i = 0; i < GeoTaggedImageList::instance().GetListLength() - 1; i++) {
        auto line = TaggedImageToStringVector(
                GeoTaggedImageList::instance().geoTaggedImageList_[i],
                GeoTaggedImageList::instance().geoTaggedImageList_[i + 1]
        );
        writer << line;
    }

    auto &lastImage = GeoTaggedImageList::instance().geoTaggedImageList_[i];
    std::vector<std::string> line;

    line.push_back(std::to_string(lastImage.index_ + 1)); // Image ID
    line.push_back(std::to_string(lastImage.coordinate_xy_.getY())); // Line
    line.push_back(std::to_string(lastImage.coordinate_xy_.getX())); // Column
    line.push_back(std::to_string(Convert::getLatitude(lastImage)));
    line.push_back(std::to_string(Convert::getLongitude(lastImage)));
    line.emplace_back("N/A");
    line.emplace_back("N/A");

    writer << line;
    return true;
}

std::vector<std::string> Export::TaggedImageToStringVector(const GeoTaggedImage &image0, const GeoTaggedImage &image1) {
    std::vector<std::string> line;

    line.push_back(std::to_string(image0.index_ + 1)); // Image ID
    line.push_back(std::to_string(image0.coordinate_xy_.y_)); // Line
    line.push_back(std::to_string(image0.coordinate_xy_.x_)); // Column
    line.push_back(std::to_string(Convert::getLatitude(image0)));
    line.push_back(std::to_string(Convert::getLongitude(image0)));
    line.push_back(std::to_string(Convert::getSpeed(image0, image1)));
    line.push_back(std::to_string(Convert::getOrientation(image0, image1)));

    return line;
}
