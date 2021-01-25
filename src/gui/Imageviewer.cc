//
// Created by Thomas Michelot on 12/14/20.
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
#include <cmath>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include "data/Export.hh"

//region shared Functions
void showPoint(SubQLabel *label, int x, int y, const QColor &color, qreal width) {
    auto img = label->pixmap(Qt::ReturnByValue).toImage();
    QPainter painter(&img);
    QPen pen(color);
    pen.setWidthF(width);
    painter.setPen(pen);
    painter.drawPoint(x, y);
    label->setPixmap(QPixmap::fromImage(img));
}

void showPoint(SubQLabel *label, const Coordinate<int> &coordinate, const QColor &color, qreal width) {
    showPoint(label, coordinate.getX(), coordinate.getY(), color, width);
}

void drawLine(SubQLabel *label, int srcX, int srcY, int dstX, int dstY, const QColor &color, qreal width) {
    auto img = label->pixmap(Qt::ReturnByValue).toImage();
    QPainter painter(&img);
    QPen red(color, width);
    painter.setPen(red);
    painter.drawLine(srcX, srcY, dstX, dstY);
    label->setPixmap(QPixmap::fromImage(img));
}

void drawLine(SubQLabel *label, const Coordinate<int> &srcCoordinate, const Coordinate<int> &dstCoordinate,
              const QColor &color, const qreal width) {
    drawLine(label, srcCoordinate.getX(), srcCoordinate.getY(), dstCoordinate.getX(), dstCoordinate.getY(),
             color, width);
}

void dialogExportCsv() {
    QDialog dialog;
    QFormLayout form(&dialog);
    form.addRow(new QLabel("All points are not set.\nDo you really want to export ?"));

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (GeoTaggedImageList::instance().allPointsSet() || dialog.exec() == QDialog::Accepted) {
        QString fileName = QFileDialog::getSaveFileName(
                &dialog, QObject::tr("Export data as CSV"),
                nullptr,
                QObject::tr("CSV (*.csv);;All Files (*)"));
        if (fileName.compare("") != 0) {
            if (Export::exportCSV(fileName.toStdString()) == Export::Error) {
                QMessageBox errorMessageBox;
                errorMessageBox.setText("There was an error while exporting.\nCheck file permissions and try again.");
                errorMessageBox.exec();
            }
        }
    }
}

void dialogExportImage(const QPixmap &pixmap) {
    QString fileName = QFileDialog::getSaveFileName(
            nullptr, QObject::tr("Export current image as PNG"),
            nullptr,
            QObject::tr("PNG (*.png);;All Files (*)"));
    if (fileName.compare("") != 0) {
        if (Export::exportImage(fileName, pixmap) == Export::Error) {
            QMessageBox errorMessageBox;
            errorMessageBox.setText("There was an error while exporting.\nCheck file permissions and try again.");
            errorMessageBox.exec();
        }
    }
}
//endregion

//region SubQLabel implementation
[[maybe_unused]] void SubQLabel::setFactor(const double &factor) {
    scaleFactor = factor;
}

void SubQLabel::mousePressEvent(QMouseEvent *event) {
    auto &instance = GeoTaggedImageList::instance();
    int x = event->x();
    int y = event->y();

    int xImage = (int) std::round(x / scaleFactor);
    int yImage = (int) std::round(y / scaleFactor);

    if (event->button() == Qt::LeftButton) {
        auto &xyCoord = instance.it_->coordinateXY_;
        xyCoord.setX(xImage);
        xyCoord.setY(yImage);
        // Immediately show trajectory on image
        if (instance.it_ != instance.geoTaggedImageList_.begin() &&
            std::prev(instance.it_)->coordinateXY_.isSet()) {
            drawLine(this, std::prev(instance.it_)->coordinateXY_, xyCoord);
            // draw previous point above line for clarity
            showPoint(this, std::prev(instance.it_)->coordinateXY_);
        }
        showPoint(this, xyCoord);
    }
    if (event->button() == Qt::RightButton) {
        if (event->modifiers() == Qt::ShiftModifier) {
            this->imageViewer_.previousImage();
        } else {
            this->imageViewer_.nextImage();
        }
    }
}

SubQLabel::SubQLabel(ImageViewer &imageViewer) : QLabel(), imageViewer_(imageViewer) {}

//endregion

//region ImageViewer implementation
ImageViewer::ImageViewer(QWidget *parent)
        : QMainWindow(parent), imageLabel(new SubQLabel(*this)), scrollArea(new QScrollArea) {
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
    loadFile(QString::fromStdString(GeoTaggedImageList::instance().it_->path_.string()));
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
            .arg(GeoTaggedImageList::instance().getListPosition() + 1)
            .arg(GeoTaggedImageList::instance().getListLength());
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
    auto begin = GeoTaggedImageList::instance().geoTaggedImageList_.begin();
    auto end = GeoTaggedImageList::instance().currentImage();

    for (auto &it = begin; it != end; it++) {
        const auto &next = std::next(it);
        if (next != end && next->coordinateXY_.isSet()) {
            drawLine(imageLabel, it->coordinateXY_, next->coordinateXY_);
        }

        showPoint(imageLabel, it->coordinateXY_);
    }

    scrollArea->setVisible(true);
}

void ImageViewer::open() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    if (dialog.exec() == QDialog::Accepted) {
        GeoTaggedImageList::instance().clear();
        GeoTaggedImageList::instance().populateImages(dialog.directory().absolutePath().toStdString());
        loadCurrentImage();
    }
}

void ImageViewer::nextImage() {
    GeoTaggedImageList::instance().nextImage();
    loadCurrentImage();
}

void ImageViewer::previousImage() {
    GeoTaggedImageList::instance().previousImage();
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

void ImageViewer::about() {
    QMessageBox::about(this, tr("About Tele"),
                       tr("<p>This program is here to assist you during the TELE's TDs.</p>"
                          "<p>Please refer to the readme for any assistance.</p>"
                          "<p>This program is opensource and is under the GNU GPL license.</p>"));
}

void ImageViewer::authors() { // NOLINT(readability-convert-member-functions-to-static)
    QMessageBox about;
    about.setTextFormat(Qt::RichText);
    about.setWindowTitle(tr("Authors"));
    about.setText(
            tr("Thomas `Dotty` Michelot ~<a href=\"mailto:thomas.michelot@epita.fr\">thomas.michelot@epita.fr</a>~"));
    about.exec();
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
    normalSizeAct->setShortcut(tr("Ctrl+N"));
    normalSizeAct->setEnabled(true);

    viewMenu->addSeparator();

    QAction *nextImageAct = viewMenu->addAction(tr("&Next Image..."), this, &ImageViewer::nextImage);
    nextImageAct->setShortcut(QKeySequence::Forward);

    QAction *previousImageAct = viewMenu->addAction(tr("&Previous Image..."), this, &ImageViewer::previousImage);
    previousImageAct->setShortcut(QKeySequence::Back);

    QMenu *exportMenu = menuBar()->addMenu(tr("&Export"));
    exportMenu->addAction(tr("&CSV file"), this, &dialogExportCsv);
    exportMenu->addAction(tr("&Image file"), this,
                          [this] { dialogExportImage(this->imageLabel->pixmap(Qt::ReturnByValue)); });

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &ImageViewer::about);
    helpMenu->addAction(tr("&Authors"), this, &ImageViewer::authors);

    connect(this, SIGNAL(scaleFactorChanged(const double&)), imageLabel, SLOT(setFactor(const double&)));
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
    loadFile(QString::fromStdString(GeoTaggedImageList::instance().it_->path_.string()));
}
//endregion