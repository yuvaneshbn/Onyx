#include "VideoRenderWidget.h"
#include <QPainter>
#include <QFont>
#include <QColor>

namespace Onyx {

VideoRenderWidget::VideoRenderWidget(const QString& title, QWidget* parent)
    : QWidget(parent),
      m_title(title) {
    setAttribute(Qt::WA_OpaquePaintEvent);
    setMinimumSize(320, 240);
    m_fpsTimer.start();
}

void VideoRenderWidget::setFrame(const QImage& frame) {
    m_currentFrame = frame;

    m_frameCount++;
    if (m_fpsTimer.elapsed() >= 1000) {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer.restart();
    }

    update();
}

void VideoRenderWidget::clearFrame() {
    m_currentFrame = QImage();
    update();
}

void VideoRenderWidget::setOverlayText(const QString& text) {
    m_overlayText = text;
    update();
}

void VideoRenderWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Deep slate background
    p.fillRect(rect(), QColor(10, 14, 23));

    if (!m_currentFrame.isNull()) {
        // Compute letterboxed target rectangle
        QSize imgSize = m_currentFrame.size();
        QSize widgetSize = size();

        float imgAspect = static_cast<float>(imgSize.width()) / imgSize.height();
        float widgetAspect = static_cast<float>(widgetSize.width()) / widgetSize.height();

        QRect targetRect;
        if (widgetAspect > imgAspect) {
            int targetWidth = static_cast<int>(widgetSize.height() * imgAspect);
            int xOffset = (widgetSize.width() - targetWidth) / 2;
            targetRect = QRect(xOffset, 0, targetWidth, widgetSize.height());
        } else {
            int targetHeight = static_cast<int>(widgetSize.width() / imgAspect);
            int yOffset = (widgetSize.height() - targetHeight) / 2;
            targetRect = QRect(0, yOffset, widgetSize.width(), targetHeight);
        }

        p.drawImage(targetRect, m_currentFrame);
    } else {
        // Placeholder state
        p.setPen(QColor(100, 116, 139));
        QFont font = p.font();
        font.setPointSize(12);
        font.setBold(true);
        p.setFont(font);
        p.drawText(rect(), Qt::AlignCenter, m_title + "\n(Stream inactive)");
    }

    // Top HUD banner
    if (m_showStats) {
        p.fillRect(QRect(0, 0, width(), 28), QColor(0, 0, 0, 140));

        p.setPen(QColor(241, 245, 249));
        QFont hudFont = p.font();
        hudFont.setPointSize(9);
        hudFont.setBold(true);
        p.setFont(hudFont);

        p.drawText(10, 19, m_title);

        QString stats = QString("FPS: %1 | %2x%3")
                            .arg(m_fps)
                            .arg(m_currentFrame.isNull() ? 0 : m_currentFrame.width())
                            .arg(m_currentFrame.isNull() ? 0 : m_currentFrame.height());

        if (!m_overlayText.isEmpty()) {
            stats += " | " + m_overlayText;
        }

        p.drawText(rect().adjusted(0, 0, -10, -height() + 28), Qt::AlignRight | Qt::AlignVCenter, stats);
    }
}

} // namespace Onyx
