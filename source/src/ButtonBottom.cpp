// ButtonBottom.cpp
// ButtonBottom.cpp
#include "ButtonBottom.h"
#include <QPainter>
#include <QStyleOptionButton>

ButtonBottom::ButtonBottom(QWidget *parent)
    : QPushButton(parent)
    , m_clrButton(45, 45, 48)            // Dark grey matching dark theme
    , m_clrButtonHover(60, 60, 65)      // Hover highlight
    , m_clrButtonPushed(92, 145, 219)   // Medium blue when pressed
    , m_clrButtonText(240, 240, 240)    // Light text
    , m_clrButtonTextPushed(255, 255, 255) // White text
{
    // Mimic flat style: no border, transparent
    setFlat(true);
    setStyleSheet("ButtonBottom { border: none; }");
    setFocusPolicy(Qt::TabFocus);
    setAttribute(Qt::WA_Hover, true); // needed for enter/leave events
}

ButtonBottom::ButtonBottom(const QString &text, QWidget *parent)
    : ButtonBottom(parent)
{
    setText(text);
}

ButtonBottom::~ButtonBottom() = default;

void ButtonBottom::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // Determine current state colors (like original DrawItem)
    QColor faceColor;
    QColor textColor;

    if (isCheckable() && isChecked())
    {
        faceColor = m_clrButtonPushed;
        textColor = m_clrButtonTextPushed;
    }
    else if (isDown())
    {
        faceColor = m_clrButtonPushed;
        textColor = m_clrButtonTextPushed;
    }
    else if (m_bHover)
    {
        faceColor = m_clrButtonHover;
        textColor = m_clrButtonText;
    }
    else
    {
        faceColor = m_clrButton;
        textColor = m_clrButtonText;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill background with face color
    painter.setBrush(faceColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    // Draw text
    painter.setPen(textColor);
    painter.drawText(rect(), Qt::AlignCenter, text());

    // Optionally draw focus rect if needed (original m_bDrawFocus = FALSE, so we skip it)
    if (hasFocus())
    {
        QStyleOptionFocusRect opt;
        opt.initFrom(this);
        opt.backgroundColor = faceColor;
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, &painter, this);
    }
}

void ButtonBottom::focusOutEvent(QFocusEvent *event)
{
    // Mimic OnKillFocus: release any capture and reset hover/pushed states
    if (isDown())
    {
        // No direct releaseCapture equivalent, but we can reset the down state
        // (Qt internally handles this; we just visually update)
        setDown(false);
    }
    m_bHover = false;
    update(); // invalidate
    QPushButton::focusOutEvent(event);
}

void ButtonBottom::enterEvent(QEnterEvent *event)
{
    m_bHover = true;
    update();
    QPushButton::enterEvent(event);
}

void ButtonBottom::leaveEvent(QEvent *event)
{
    m_bHover = false;
    update();
    QPushButton::leaveEvent(event);
}

void ButtonBottom::mousePressEvent(QMouseEvent *event)
{
    // Let base class handle press, but force visual update after
    QPushButton::mousePressEvent(event);
    update();
}

void ButtonBottom::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
    update();
}
