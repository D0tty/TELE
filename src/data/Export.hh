//
// Created by dotty on 1/4/21.
//

#ifndef TELE_EXPORT_HH
#define TELE_EXPORT_HH

#include "maths/Convert.hh"
#include "metadata/GeoTaggedImageList.hh"
#include <csv.hpp>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

class Export {
public:
    static void exportCSV(const std::string &path);

private:
    static std::vector<std::string> TaggedImageToStringVector(
            const GeoTaggedImage &image0,
            const GeoTaggedImage &image1);
};


#endif //TELE_EXPORT_HH
