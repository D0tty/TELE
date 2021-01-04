//
// Created by dotty on 12/14/20.
//

#include "Imageviewer.hh"

#include <QApplication>
#include <QColorSpace>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStatusBar>
#include <QMouseEvent>
#include <iostream>
#include <cmath>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include "data/Export.hh"

//region shared Functions
void showPoint(SubQLabel *label, int x, int y, const QColor &color = Qt::GlobalColor::red) {
    auto img = label->pixmap(Qt::ReturnByValue).toImage();
    img.setPixelColor(x, y, color);
    label->setPixmap(QPixmap::fromImage(img));
}

void dialogExport() {
    QDialog dialog;
    QFormLayout form(&dialog);
    form.addRow(new QLabel("Export data ?"));

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    //FIXME show modal only if there is a point that is not set
    if (dialog.exec() == QDialog::Accepted) {
        QString fileName = QFileDialog::getSaveFileName(
                &dialog, QObject::tr("Export data as CSV"),
                nullptr,
                QObject::tr("CSV (*.csv);;All Files (*)"));
        Export::exportCSV(fileName.toStdString()); //FIXME Modal on error ?
    }
}
//endregion

//region SubQLabel implementation
[[maybe_unused]] void SubQLabel::setFactor(const double &factor) {
    scaleFactor = factor;
}

void SubQLabel::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();

    int x_image = (int) std::round(x / scaleFactor);
    int y_image = (int) std::round(y / scaleFactor);

    if (event->button() == Qt::LeftButton) {
        auto &xy_coord = GeoTaggedImageList::instance().it_.base()->coordinate_xy_;
        xy_coord.setX(x_image);
        xy_coord.setY(y_image);
        // Immediately show selected point on image
        showPoint(this, x_image, y_image);
    }
}
//endregion

//region ImageViewer implementation
ImageViewer::ImageViewer(QWidget *parent)
        : QMainWindow(parent), imageLabel(new SubQLabel), scrollArea(new QScrollArea) {
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);
    setCentralWidget(scrollArea);

    createActions();

    auto s = QGuiApplication::primaryScreen()->availableVirtualSize() * 2 / 5; //magic number to fix x11 display bug
    resize(s);
    loadFile(QString::fromStdString(GeoTaggedImageList::instance().it_.base()->path_.string()));
}

bool ImageViewer::loadFile(const QString &fileName) {
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(fileName),
                                                              reader.errorString()));
        return false;
    }

    setImage(newImage);

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2/%3")
            .arg(QDir::toNativeSeparators(
                    QString::fromStdString(boost::filesystem::path(fileName.toStdString()).stem().string())))
            .arg(GeoTaggedImageList::instance().GetListPosition() + 1)
            .arg(GeoTaggedImageList::instance().GetListLength());
    statusBar()->showMessage(message);
    imageLabel->setStatusTip(message);
    return true;
}

void ImageViewer::setImage(const QImage &newImage) {
    image = newImage;
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;
    scaleImage(scaleFactor);

    // if the point is present show it
    auto *const img = GeoTaggedImageList::instance().CurrentImage();
    if (img->coordinate_xy_.getX() != 0 && img->coordinate_xy_.getY() != 0) {
        showPoint(imageLabel, img->coordinate_xy_.getX(), img->coordinate_xy_.getY());
    }

    scrollArea->setVisible(true);
    //fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

void ImageViewer::open() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    if (dialog.exec() == QDialog::Accepted) {
        GeoTaggedImageList::instance().clear();
        GeoTaggedImageList::instance().PopulateImages(dialog.directory().absolutePath().toStdString());
        loadCurrentImage();
    }
}

void ImageViewer::nextImage() {
    GeoTaggedImageList::instance().NextImage();
    loadCurrentImage();
}

void ImageViewer::previousImage() {
    GeoTaggedImageList::instance().PreviousImage();
    loadCurrentImage();
}

void ImageViewer::zoomIn() {
    scaleImage(1.25);
}

void ImageViewer::zoomOut() {
    scaleImage(0.8);
}

void ImageViewer::normalSize() {
    imageLabel->adjustSize();
    scaleFactor = 1.0;
    emit scaleFactorChanged(scaleFactor);
}

void ImageViewer::fitToWindow() {
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void ImageViewer::about() {
    QMessageBox::about(this, tr("About Image Viewer"),
                       tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
                          "and QScrollArea to display an image. QLabel is typically used "
                          "for displaying a text, but it can also display an image. "
                          "QScrollArea provides a scrolling view around another widget. "
                          "If the child widget exceeds the size of the frame, QScrollArea "
                          "automatically provides scroll bars. </p><p>The example "
                          "demonstrates how QLabel's ability to scale its contents "
                          "(QLabel::scaledContents), and QScrollArea's ability to "
                          "automatically resize its contents "
                          "(QScrollArea::widgetResizable), can be used to implement "
                          "zooming and scaling features. </p><p>In addition the example "
                          "shows how to use QPainter to print an image.</p>"));
}

void ImageViewer::createActions() {

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &ImageViewer::open);
    openAct->setShortcut(QKeySequence::Open);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &ImageViewer::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &ImageViewer::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &ImageViewer::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    viewMenu->addSeparator();

    QAction *nextImageAct = viewMenu->addAction(tr("&Next Image..."), this, &ImageViewer::nextImage);
    nextImageAct->setShortcut(QKeySequence::Forward);

    QAction *previousImageAct = viewMenu->addAction(tr("&Previous Image..."), this, &ImageViewer::previousImage);
    previousImageAct->setShortcut(QKeySequence::Back);

    viewMenu->addSeparator();

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &ImageViewer::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    QMenu *exportMenu = menuBar()->addMenu(tr("&Export"));
    exportMenu->addAction(tr("CSV file"), this, &dialogExport);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &ImageViewer::about);
    helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);

    connect(this, SIGNAL(scaleFactorChanged(const double&)), imageLabel, SLOT(setFactor(const double&)));
}

void ImageViewer::updateActions() {
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor) {
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);

    emit scaleFactorChanged(scaleFactor);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep() / 2)));
}

void ImageViewer::loadCurrentImage() {
    loadFile(QString::fromStdString(GeoTaggedImageList::instance().it_.base()->path_.string()));
}
//endregion