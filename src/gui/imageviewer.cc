//
// Created by dotty on 12/14/20.
//

#include "imageviewer.hh"

#include <QApplication>
#include <QColorSpace>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStatusBar>
#include <QMouseEvent>
#include <iostream>
#include <cmath>

//region SubQLabel implementation
void SubQLabel::setFactor(const double &factor) {
    scaleFactor = factor;
}

void SubQLabel::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();

    int x_image = (int) std::round(x / scaleFactor);
    int y_image = (int) std::round(y / scaleFactor);

    if (event->button() == Qt::LeftButton) {
        //FIXME here add logic to match (x,y) to the coord of the current image
        std::cout << "base x: " << x << std::endl;
        std::cout << "base y: " << y << std::endl;

        std::cout << "Image pixel: (" << x_image << ", " << y_image << ")" << std::endl;
        std::cout << "Image float pixel (" << event->x() / scaleFactor << ", " << event->y() / scaleFactor << ")"
                  << std::endl;
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
            .arg(GeoTaggedImageList::instance().GetListPosition())
            .arg(GeoTaggedImageList::instance().GetListLength());
    statusBar()->showMessage(message);
    return true;
}

//FIXME no need to load an image but a directory and populate images
//FIXME memory carefull, how to deallocate a singleton ?
void ImageViewer::setImage(const QImage &newImage) {
    image = newImage;
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    //scaleFactor = 1.0; // remove so scaling is preserved from one image to the other

    scrollArea->setVisible(true);
    //fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode) {
    static bool firstDialog = true;

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
                                              ? QImageReader::supportedMimeTypes()
                                              : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void ImageViewer::open() {
    QFileDialog dialog(this, tr("Open File"), QDir::currentPath());
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::nextImage() {
    GeoTaggedImageList::instance().NextImage();
    loadCurrentImage();
    //FIXME save image state ?
    //FIXME then load the image
}

void ImageViewer::previousImage() {
    GeoTaggedImageList::instance().PreviousImage();
    loadCurrentImage();
    //FIXME save image state ?
    //FIXME load previous image
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
    scaleImage(1);
}
//endregion