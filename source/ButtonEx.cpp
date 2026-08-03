// ButtonEx.cpp
#include "ButtonEx.h"

#include <QPainter>
#include <QStyleOptionButton>

ButtonEx::ButtonEx(QWidget *parent)
    : QPushButton(parent)
    , m_FaceColor(255, 255, 255)    // default white
    , m_TextColor(0, 0, 0)          // default black
{
    // Flat no‑border style (mimics CMFCButton::BUTTONSTYLE_NOBORDERS)
    setFlat(true);
    setStyleSheet("ButtonEx { border: none; }");
}

ButtonEx::ButtonEx(const QString &text, QWidget *parent)
    : ButtonEx(parent)
{
    setText(text);
}

ButtonEx::~ButtonEx() = default;

QColor ButtonEx::faceColor() const
{
    return m_FaceColor;
}

void ButtonEx::setFaceColor(const QColor &color)
{
    m_FaceColor = color;
    update();
}

QColor ButtonEx::textColor() const
{
    return m_TextColor;
}

void ButtonEx::setTextColor(const QColor &color)
{
    m_TextColor = color;
    update();
}

void ButtonEx::setEnabled(bool enabled)
{
    if (enabled) {
        // Restore original colours
        m_FaceColor = m_FaceColor; // they are already stored
        m_TextColor = m_TextColor;
    } else {
        // Disabled state colours (like original EnableWindow)
        // Original: SetTextColor(RGB(0,0,0)) and SetFaceColor(RGB(222,222,222))
        m_FaceColor = QColor(222, 222, 222);
        m_TextColor = QColor(0, 0, 0);
    }
    // Base class call; will also trigger a repaint
    QPushButton::setEnabled(enabled);
    update();
}

void ButtonEx::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    // Fill background with current face colour
    painter.setBrush(m_FaceColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    // Draw text with current text colour
    painter.setPen(m_TextColor);
    painter.drawText(rect(), Qt::AlignCenter, text());

    // Optionally draw focus rect if focused
    if (hasFocus()) {
        QStyleOptionFocusRect opt;
        opt.initFrom(this);
        opt.backgroundColor = m_FaceColor;
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, &painter, this);
    }
}

void ButtonEx::mouseMoveEvent(QMouseEvent *event)
{
    // Calls base class – identical to original
    QPushButton::mouseMoveEvent(event);
}
