//
// Created by Thomas Michelot on 1/4/21.
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
#include <QtGui/QImage>
#include <QtGui/QPixmap>

class Export {
public:
    enum {
        Error = false,
        Success = true
    };

    static bool exportCSV(const std::string &path);

    static bool exportImage(const QString& path, const QPixmap &pixmap);

private:
    static std::vector<std::string> taggedImageToStringVector(
            const GeoTaggedImage &image0,
            const GeoTaggedImage &image1);
};


#endif //TELE_EXPORT_HH
