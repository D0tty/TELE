//
// Created by Thomas Michelot on 12/6/20.
//

#include "misc/misc.hh"
#include "metadata/GeoTaggedImageList.hh"
#include <QApplication>
#include <gui/Imageviewer.hh>
#include <QtWidgets/QFileDialog>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    if (dialog.exec() == 0) {
        std::cout << "No directory was selected." << std::endl;
        return 2;
    }

    auto &geoTaggedImageList = GeoTaggedImageList::instance();
    geoTaggedImageList.populateImages(dialog.directory().absolutePath().toStdString());

    ImageViewer imageViewer;
    imageViewer.show();
    QApplication::exec();
    return 0;
}