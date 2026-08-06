// LevelsSliderCtrl.cpp
#include "LevelsSliderCtrl.h"
#include <QStyleOptionSlider>
#include <QPainter>
#include <QColor>

void CLevelsSliderCtrl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QStyleOptionSlider opt;
    initStyleOption(&opt);

    // Determine if we should custom draw (like TBS_ENABLESELRANGE check)
    // In MFC, TBS_ENABLESELRANGE is equivalent to having a selection range enabled.
    // We'll always draw custom when selection range is meaningful.
    // For simplicity, we'll always custom draw if we have a selection range set.
    // If not, we could fall back to default drawing. But to mirror original, we'll always custom draw.

    // Get the groove (channel) and handle (thumb) rectangles from style
    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    // --- Thumb drawing (CDDS_ITEMPREPAINT for TBCD_THUMB) ---
    // In original: deflate by (6,3) or (3,6) based on orientation, fill with RGB(170,170,170)
    {
        painter.save();
        QRect thumbRect = handleRect;
        if (orientation() == Qt::Vertical) {
            thumbRect.adjust(6, 3, -6, -3);
        } else {
            thumbRect.adjust(3, 6, -3, -6);
        }
        painter.fillRect(thumbRect, QColor(170, 170, 170));
        painter.restore();
        // Skip default thumb drawing (CDRF_SKIPDEFAULT) – we don't draw default thumb
    }

    // --- Channel drawing (CDDS_ITEMPREPAINT for TBCD_CHANNEL) ---
    {
        painter.save();
        QRect channelRect = grooveRect;

        // Fix Windows bug: swapped dimensions
        // The original checks if rect.right > rect.bottom (for vertical) and swaps left/top, right/bottom.
        // We'll apply the same logic to channelRect.
        if (orientation() == Qt::Vertical) {
            if (channelRect.right() > channelRect.bottom()) {
                // Swap left and top
                int temp = channelRect.left();
                channelRect.setLeft(channelRect.top());
                channelRect.setTop(temp);
                // Swap right and bottom
                temp = channelRect.right();
                channelRect.setRight(channelRect.bottom());
                channelRect.setBottom(temp);
            }
        } else {
            if (channelRect.bottom() > channelRect.right()) {
                int temp = channelRect.left();
                channelRect.setLeft(channelRect.top());
                channelRect.setTop(temp);
                temp = channelRect.right();
                channelRect.setRight(channelRect.bottom());
                channelRect.setBottom(temp);
            }
        }

        // Deflate rect by (2,2)
        channelRect.adjust(2, 2, -2, -2);

        int min = minimum();
        int max = maximum();
        int selmin = m_selMin;
        int selmax = m_selMax;
        int range = max - min;
        bool hot = false;

        // Calculate selection positions
        int selStartPos, selEndPos;
        if (orientation() == Qt::Vertical) {
            hot = (selmin <= 0);
            // In vertical, the top is minimum? Original uses: selmin from top? We need to align with MFC.
            // In MFC vertical slider, min is at bottom? Actually TBS_VERT typical: min is bottom. But their calculation:
            // selmin = (int)(((double)(selmin - min) / max * rect.Height()) + 0.5) + rect.top;
            // This maps min->top? Because if min=0, selmin=0 gives top? Let's analyze: if selmin-min=0, then 0 * height = 0, + rect.top -> top. So min maps to top.
            // So vertical: top is min, bottom is max. We'll adopt that.
            selStartPos = (int)(((double)(selmin - min) / range * channelRect.height()) + 0.5) + channelRect.top();
            selEndPos   = (int)(((double)(selmax - min) / range * channelRect.height()) + 0.5) + channelRect.top();
        } else {
            hot = (selmax >= maximum());
            selStartPos = (int)(((double)(selmin - min) / range * channelRect.width()) + 0.5) + channelRect.left();
            selEndPos   = (int)(((double)(selmax - min) / range * channelRect.width()) + 0.5) + channelRect.left();
        }

        // Background fill
        QColor bgColor;
        if (!m_isActive) {
            bgColor = palette().color(QPalette::Window); // GetBkColor equivalent (approx)
        } else {
            bgColor = palette().color(QPalette::Window); // COLOR_WINDOW -> use Window color
        }

        if (orientation() == Qt::Vertical) {
            QRect fullChannel = channelRect;
            painter.fillRect(fullChannel, bgColor);
            // Selection rectangle
            QRect selRect(channelRect.left(), selStartPos, channelRect.width(), selEndPos - selStartPos);
            QColor selColor = hot ? QColor(255, 0, 0) : palette().color(QPalette::Highlight);
            painter.fillRect(selRect, selColor);
            // Exclude clip rect (in original they exclude clip so no further drawing on channel)
            painter.setClipRect(channelRect, Qt::NoClip); // Actually exclude? They used ExcludeClipRect, so we set clip to outside? We'll just skip further channel drawing.
        } else {
            QRect fullChannel = channelRect;
            painter.fillRect(fullChannel, bgColor);
            QRect selRect(selStartPos, channelRect.top(), selEndPos - selStartPos, channelRect.height());
            QColor selColor = hot ? QColor(255, 0, 0) : palette().color(QPalette::Highlight);
            painter.fillRect(selRect, selColor);
            painter.setClipRect(channelRect, Qt::NoClip);
        }

        painter.restore();
    }

    // Note: In original, CDDS_ITEMPOSTPAINT for TBCD_CHANNEL resets clip region.
    // Since we used painter.save/restore, clip is restored automatically.
}