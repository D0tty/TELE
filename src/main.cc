#include "misc/misc.hh"
#include "metadata/GeoTaggedImageList.hh"
#include <QApplication>
#include <gui/imageviewer.hh>
#include <QtWidgets/QFileDialog>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.exec();

//    boost::filesystem::path directoryPath();
    auto &geoTaggedImageList = GeoTaggedImageList::instance();
    geoTaggedImageList.PopulateImages(dialog.directory().absolutePath().toStdString());

    ImageViewer imageViewer;
    imageViewer.show();
    QApplication::exec();
    return 0;
}