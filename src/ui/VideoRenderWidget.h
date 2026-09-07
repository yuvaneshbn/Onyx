#ifndef VIDEO_RENDER_WIDGET_H
#define VIDEO_RENDER_WIDGET_H

#include <QWidget>
#include <QImage>
#include <QString>
#include <QElapsedTimer>

namespace Onyx {

class VideoRenderWidget : public QWidget {
    Q_OBJECT
public:
    explicit VideoRenderWidget(const QString& title = "Video Stream", QWidget* parent = nullptr);
    ~VideoRenderWidget() override = default;

    void setFrame(const QImage& frame);
    void clearFrame();
    void setOverlayText(const QString& text);
    void setShowStats(bool show) { m_showStats = show; update(); }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString m_title;
    QString m_overlayText;
    QImage m_currentFrame;
    bool m_showStats = true;

    QElapsedTimer m_fpsTimer;
    int m_frameCount = 0;
    int m_fps = 0;
};

} // namespace Onyx

#endif // VIDEO_RENDER_WIDGET_H
