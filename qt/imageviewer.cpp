#include "imageviewer.h"
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QStyleOption>
#include <QtMath>

ImageViewer::ImageViewer(const QString& placeholderText, QWidget *parent)
    : QWidget(parent)
    , m_scaleFactor(1.0)
    , m_offset(0, 0)
    , m_isPanning(false)
    , m_placeholderText(placeholderText)
{
    setMouseTracking(true); 
    setFocusPolicy(Qt::StrongFocus);
}

ImageViewer::~ImageViewer() {}

void ImageViewer::setImage(const QImage& image)
{
    m_image = image;
    resetView();
    update();
}

void ImageViewer::clearImage()
{
    m_image = QImage();
    resetView();
    update();
}

const QImage& ImageViewer::image() const
{
    return m_image;
}

void ImageViewer::resetView()
{
    m_scaleFactor = 1.0;
    m_offset = QPointF(0, 0); 
    
    if (!m_image.isNull()) {
        // Calculate initial scale to fit the image in the widget
        double wRatio = (double)width() / m_image.width();
        double hRatio = (double)height() / m_image.height();
        m_scaleFactor = qMin(wRatio, hRatio);
        // Add a little margin
        m_scaleFactor *= 0.95;
    }
}

void ImageViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    
    // To support QSS background color, border etc on QWidget
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    if (m_image.isNull()) {
        painter.setPen(QColor("#666666"));
        QFont font = painter.font();
        font.setPointSize(14);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, m_placeholderText);
        return;
    }

    painter.save();
    
    // Set smooth transformation
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    // Move to center of the widget
    painter.translate(rect().center());
    
    // Apply panning offset
    painter.translate(m_offset);
    
    // Apply zoom
    painter.scale(m_scaleFactor, m_scaleFactor);
    
    // Move back so the image is centered around 0,0 in the new coordinate system
    painter.translate(-m_image.width() / 2.0, -m_image.height() / 2.0);
    
    // Draw the image
    painter.drawImage(0, 0, m_image);
    
    painter.restore();
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{
    if (m_image.isNull()) return;

    double numDegrees = event->angleDelta().y() / 8.0;
    double numSteps = numDegrees / 15.0;

    if (numSteps == 0) {
        event->ignore();
        return;
    }

    double scaleMultiplier = qPow(1.15, numSteps);
    double newScale = m_scaleFactor * scaleMultiplier;
    
    // Limit bounds
    if (newScale < 0.05) newScale = 0.05;
    if (newScale > 50.0) newScale = 50.0;
    
    // Zoom relative to the mouse cursor position
    QPointF mousePos = event->position() - rect().center();
    m_offset = mousePos - (mousePos - m_offset) * (newScale / m_scaleFactor);
    
    m_scaleFactor = newScale;
    
    update();
    event->accept();
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton || event->button() == Qt::LeftButton) {
        m_isPanning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_offset += delta;
        m_lastMousePos = event->pos();
        update();
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event);
    }
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton || event->button() == Qt::LeftButton) {
        m_isPanning = false;
        unsetCursor();
        event->accept();
    } else {
        QWidget::mouseReleaseEvent(event);
    }
}
