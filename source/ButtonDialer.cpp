// ButtonDialer.cpp
#include "ButtonDialer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QStyleOptionButton>
#include <QStyle>
#include <QMouseEvent>

// ---------------------------------------------------------------------------
// Constructor – initialises the keypad mapping and UI defaults
// ---------------------------------------------------------------------------
ButtonDialer::ButtonDialer(QWidget *parent)
    : QPushButton(parent)
{
    // Same mapping as original CButtonDialer constructor
    m_map[QStringLiteral("1")] = QString();
    m_map[QStringLiteral("2")] = QStringLiteral("ABC");
    m_map[QStringLiteral("3")] = QStringLiteral("DEF");
    m_map[QStringLiteral("4")] = QStringLiteral("GHI");
    m_map[QStringLiteral("5")] = QStringLiteral("JKL");
    m_map[QStringLiteral("6")] = QStringLiteral("MNO");
    m_map[QStringLiteral("7")] = QStringLiteral("PQRS");
    m_map[QStringLiteral("8")] = QStringLiteral("TUV");
    m_map[QStringLiteral("9")] = QStringLiteral("WXYZ");
    m_map[QStringLiteral("0")] = QString();
    m_map[QStringLiteral("*")] = QString();
    m_map[QStringLiteral("#")] = QString();

    setMouseTracking(true);               // needed for mouseMoveEvent
    setAttribute(Qt::WA_Hover);           // enable hover events
    initFonts();
}

ButtonDialer::ButtonDialer(const QString &text, QWidget *parent)
    : ButtonDialer(parent)
{
    setText(text);
}

ButtonDialer::~ButtonDialer() = default;

// ---------------------------------------------------------------------------
// Font creation (replaces PreSubclassWindow logic)
// ---------------------------------------------------------------------------
void ButtonDialer::initFonts()
{
    // Base font similar to Microsoft Sans Serif, height 12 * dpi / 96
    QFont baseFont(QStringLiteral("Microsoft Sans Serif"), 8);
    baseFont.setStyleHint(QFont::SansSerif);

    // Compute DPI (use screen's logical DPI)
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
        m_dpiY = static_cast<int>(screen->logicalDotsPerInchY());
    else
        m_dpiY = 96;

    // Desired point size for letters: original used lfHeight = 12 * dpiY / 96.
    // In Qt, we can set pixel size directly: font.setPixelSize(12 * m_dpiY / 96);
    int letterPixelSize = qRound(12.0 * m_dpiY / 96.0);
    m_fontLetters = baseFont;
    m_fontLetters.setPixelSize(letterPixelSize);
}

// ---------------------------------------------------------------------------
// Resize event – placeholder for future adjustments (as in original)
// ---------------------------------------------------------------------------
void ButtonDialer::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    // Nothing special needed now; original OnSize is empty
}

// ---------------------------------------------------------------------------
// Mouse move – capture while inside, release when outside (to mimic original)
// ---------------------------------------------------------------------------
void ButtonDialer::mouseMoveEvent(QMouseEvent *event)
{
    QPushButton::mouseMoveEvent(event);

    QRect r = rect();
    if (r.contains(event->pos())) {
        // Inside – ensure we have capture (Qt handles press/down state; we just update visual)
        if (!underMouse()) // just a sanity check
            update();
    } else {
        // Outside – original releases capture
        // No direct capture in Qt, but we can release mouse grab if needed; visual update suffices
        update();
    }
}

// ---------------------------------------------------------------------------
// Enter/Leave events – used for hover state
// ---------------------------------------------------------------------------
void ButtonDialer::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    update();   // repaint to reflect hover
}

void ButtonDialer::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    update();   // repaint to reflect normal state
}

// ---------------------------------------------------------------------------
// Custom painting – replicates DrawItem behavior
// ---------------------------------------------------------------------------
void ButtonDialer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 1. Draw background (button face)
    QStyleOptionButton option;
    option.initFrom(this);

    // Map state to QStyle::State flags
    if (isDown() || (isCheckable() && isChecked()))
        option.state |= QStyle::State_Sunken;
    else if (underMouse())
        option.state |= QStyle::State_MouseOver;

    // Use native style to draw the button background (handles themes)
    style()->drawPrimitive(QStyle::PE_PanelButtonCommand, &option, &painter, this);

    // 2. Prepare for text drawing
    QRect r = rect().adjusted(0, 0, -1, -1);  // avoid border overlap
    // DPI‑adjusted margins (like original's MulDiv calls)
    int x12 = qRound(12.0 * m_dpiY / 96.0);
    int x14 = qRound(14.0 * m_dpiY / 96.0);
    int x4  = qRound(4.0  * m_dpiY / 96.0);

    QString strTemp = text();                 // the button's main label
    QString letters = m_map.value(strTemp);   // associated letters, if any

    // 3. Handle forced numeric mode differently
    if (m_forceNumeric || letters.isEmpty()) {
        // Draw only the digit, centred
        QColor textColor = m_forceNumeric ? QColor(80, 80, 80) : QColor(127, 127, 127);
        painter.setPen(textColor);
        painter.drawText(r, Qt::AlignCenter, strTemp);
    } else {
        // Draw digit on the left (with some left margin) and letters on the right
        QRect digitRect = r;
        digitRect.setLeft(r.left() + x14);
        painter.setPen(Qt::black);
        painter.drawText(digitRect, Qt::AlignLeft | Qt::AlignVCenter, strTemp);

        // Letters in smaller grey font
        QRect letterRect = r;
        letterRect.setLeft(digitRect.right() + x12);
        letterRect.setRight(r.right() - x4);
        painter.setFont(m_fontLetters);
        painter.setPen(QColor(127, 127, 127));
        painter.drawText(letterRect, Qt::AlignLeft | Qt::AlignVCenter, letters);
    }

    // 4. Draw focus rectangle if focused (original logic)
    if (hasFocus()) {
        QStyleOptionFocusRect focusOption;
        focusOption.initFrom(this);
        focusOption.backgroundColor = option.palette.color(QPalette::Button);
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &focusOption, &painter, this);
    }
}
