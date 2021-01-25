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

//region Shared Functions

static constexpr float defaultWidth = 5.0;
static const QColor defaultLineColor = Qt::GlobalColor::red;
static const QColor defaultPointColor = Qt::GlobalColor::darkBlue;

void showPoint(SubQLabel *label, int x, int y, const QColor &color = defaultPointColor, qreal width = defaultWidth);

void showPoint(SubQLabel *label, const Coordinate<int> &coordinate, const QColor &color = defaultPointColor,
               qreal width = defaultWidth);

void drawLine(SubQLabel *label, int srcX, int srcY, int dstX, int dstY, const QColor &color = defaultLineColor,
              qreal width = defaultWidth);

void drawLine(SubQLabel *label, const Coordinate<int> &srcCoordinate, const Coordinate<int> &dstCoordinate,
              const QColor &color = defaultLineColor, qreal width = defaultWidth);

void dialogExportCsv();

void dialogExportImage(const QPixmap &pixmap);
//endregion Shared Functions

//region ImageViewer class
class ImageViewer : public QMainWindow {
Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    bool loadFile(const QString &);

public slots:

    void nextImage();

    void previousImage();

private slots:

    void open();

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
    explicit SubQLabel(ImageViewer &imageViewer);

    void mousePressEvent(QMouseEvent *event) override;

public slots:

    [[maybe_unused]] void setFactor(const double &factor);

private:
    double scaleFactor = 1.0;
    ImageViewer &imageViewer_;

};
//endregion

#endif //TELE_IMAGEVIEWER_HH
