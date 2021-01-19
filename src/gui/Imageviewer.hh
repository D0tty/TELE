//
// Created by Thomas Michelot on 12/14/20.
//

#ifndef TELE_IMAGEVIEWER_HH
#define TELE_IMAGEVIEWER_HH

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <metadata/GeoTaggedImageList.hh>

//region Class forward declarations
QT_BEGIN_NAMESPACE
class QAction;

class QLabel;

class QMenu;

class QScrollArea;

class QScrollBar;

class SubQLabel;

QT_END_NAMESPACE
//endregion

//region ImageViewer class
class ImageViewer : public QMainWindow {
Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    bool loadFile(const QString &);

private slots:

    void open();

    void nextImage();

    void previousImage();

    void zoomIn();

    void zoomOut();

    void normalSize();

    void about();

    void authors();

signals:

    void scaleFactorChanged(const double &);

private:
    void loadCurrentImage();

    void createActions();

    void setImage(const QImage &newImage);

    void scaleImage(double factor);

    static void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QImage image;
    SubQLabel *imageLabel;
    double scaleFactor = 1.0;
    QScrollArea *scrollArea;
    QAction *zoomInAct{};
    QAction *zoomOutAct{};
    QAction *normalSizeAct{};
};
//endregion

//region SubQLabel class
class SubQLabel : public QLabel {
Q_OBJECT

public:
    void mousePressEvent(QMouseEvent *event) override;

public slots:

    [[maybe_unused]] void setFactor(const double &factor);

private:
    double scaleFactor = 1.0;

};
//endregion

#endif //TELE_IMAGEVIEWER_HH
