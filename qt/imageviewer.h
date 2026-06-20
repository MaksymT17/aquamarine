#pragma once

#include <QWidget>
#include <QImage>
#include <QPoint>

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(const QString& placeholderText = "", QWidget *parent = nullptr);
    ~ImageViewer() override;

    void setImage(const QImage& image);
    void clearImage();
    const QImage& image() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void resetView();

    QImage m_image;
    double m_scaleFactor;
    QPointF m_offset;
    
    bool m_isPanning;
    QPoint m_lastMousePos;
    QString m_placeholderText;
};
