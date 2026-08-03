// ClosableTabCtrl.cpp
#include "ClosableTabCtrl.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOptionTab>
#include <QStylePainter>
#include <QApplication>
#include <QPalette>

ClosableTabCtrl::ClosableTabCtrl(QWidget *parent)
    : QTabBar(parent)
    , m_bCloseable(true)
    , m_iTabClose(-1)
{
    setDrawBase(false);   // mimic owner-draw: we handle all painting
    InternalInit();
}

ClosableTabCtrl::~ClosableTabCtrl() = default;

void ClosableTabCtrl::setClosable(bool closable)
{
    if (m_bCloseable != closable) {
        m_bCloseable = closable;
        update();   // repaint to show/hide close buttons
    }
}

// Equivalent to DeleteItem (remove tab)
void ClosableTabCtrl::DeleteItem(int index)
{
    // Original workaround: scroll to first tab before removal
    if (count() > 1 && index >= 0 && index < count()) {
        setCurrentIndex(0);
    }
    removeTab(index);
}

// Equivalent to OnCreate & PreSubclassWindow combined
void ClosableTabCtrl::InternalInit()
{
    // Make tabs owner-drawn (Qt always allows custom painting)
    setTabsClosable(false);          // we draw our own close button
    setSelectionBehaviorOnRemove(QTabBar::SelectLeftTab);
    setMovable(false);
    // Enable mouse tracking for hover detection on close button
    setMouseTracking(true);
    // Load close icons
    SetAllIcons();
}

// Load close button icons (replaces m_ImgLstCloseButton and IDI_CLOSE/IDI_CLOSE_2)
void ClosableTabCtrl::SetAllIcons()
{
    if (!m_bCloseable)
        return;

    // Load from resource or file; assuming icons are available in Qt resources.
    // Adjust paths as needed. Original used 16x16 icons.
    m_iconClose = QIcon(":/icons/close.ico").pixmap(16, 16).toImage();
    m_iconCloseDisabled = QIcon(":/icons/close2.ico").pixmap(16, 16).toImage();
    m_closeButtonSize = m_iconClose.size();  // equivalent to ii.rcImage dimensions
    update();
}

// Equivalent to GetTabUnderPoint
int ClosableTabCtrl::GetTabUnderPoint(const QPoint &point) const
{
    for (int i = 0; i < count(); ++i) {
        QRect rc = tabRect(i);
        rc.adjust(-2, -2, 2, 2);   // inflate 2 pixels like original
        if (rc.contains(point))
            return i;
    }
    return -1;
}

// Context menu position helpers
int ClosableTabCtrl::GetTabUnderContextMenu() const
{
    if (m_ptCtxMenu.isNull())
        return -1;
    return GetTabUnderPoint(m_ptCtxMenu);
}

bool ClosableTabCtrl::SetDefaultContextMenuPos()
{
    int i = currentIndex();
    if (i < 0)
        return false;
    QRect rc = tabRect(i);
    rc.adjust(-2, -2, 2, 2);
    m_ptCtxMenu = QPoint(rc.left() + rc.width() / 2, rc.top() + rc.height() / 2);
    return true;
}

// Calculate close button rect – ported from original GetCloseButtonRect
QRect ClosableTabCtrl::GetCloseButtonRect(int index, const QRect &tabRect, bool selected, bool vistaThemeActive)
{
    QRect rcBtn;
    rcBtn.setTop(tabRect.top() + 2);
    rcBtn.setSize(m_closeButtonSize);
    rcBtn.moveRight(tabRect.right() - 2);
    if (vistaThemeActive)
        rcBtn.adjust(0, 0, -1, 0); // adjust width as original

    if (selected) {
        rcBtn.translate(-1, 0);
        if (vistaThemeActive) {
            int items = count();
            if (items > 1 && index == items - 1)
                rcBtn.translate(-2, 0);
        }
    } else {
        if (vistaThemeActive) {
            int items = count();
            if (items > 1 && index < items - 1)
                rcBtn.translate(2, 0);
        }
    }
    return rcBtn;
}

// Theming detection (simplified – Qt uses QStyle to determine if theme is active)
bool ClosableTabCtrl::IsVistaThemeActive() const
{
    // In Qt, we can check if the current style is a "native" style (e.g., Windows, Fusion)
    return style()->objectName().contains("windowsvista", Qt::CaseInsensitive) ||
           style()->objectName().contains("windows", Qt::CaseInsensitive);
}

// Painting – equivalent to DrawItem
void ClosableTabCtrl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStylePainter painter(this);
    QStyleOptionTab opt;
    
    // Draw base and tabs using the style (handles theming automatically)
    for (int i = 0; i < count(); ++i) {
        opt.initFrom(this);
        opt.state &= ~QStyle::State_Selected;
        if (i == currentIndex())
            opt.state |= QStyle::State_Selected;
        opt.rect = tabRect(i);
        // The base style will draw the tab shape and background
        style()->drawControl(QStyle::CE_TabBarTab, &opt, &painter, this);
        
        // Then call custom DrawItem to paint text, icon, and close button
        DrawItem(painter, i, opt);
    }
}

// Custom drawing for each tab – replicates original DrawItem logic
void ClosableTabCtrl::DrawItem(QPainter &painter, int index, const QStyleOptionTab &option)
{
    if (index < 0 || index >= count())
        return;

    QRect rc = option.rect;
    bool bSelected = (option.state & QStyle::State_Selected);
    bool bHotTracked = option.state & QStyle::State_MouseOver; // simplified

    // Clear background if needed (style already painted, but we may adjust)
    // The original did extra fill for non-themed; in Qt style handles that.

    // Draw tab icon (image from the list)
    QIcon itemIcon = this->tabIcon(index);
    if (!itemIcon.isNull()) {
        QRect iconRect = rc;
        iconRect.setLeft(rc.left() + (bSelected ? 8 : 4));
        iconRect.setTop(rc.top() + (bSelected ? 2 : 1));
        // size from original image list? we'll use 16x16
        itemIcon.paint(&painter, iconRect, Qt::AlignCenter);
        int iconWidth = 16;
        rc.setLeft(rc.left() + iconWidth + (bSelected ? 12 : 8));
    }

    // Close button
    bool bCloseable = m_bCloseable;
    if (bCloseable && !m_iconClose.isNull()) {
        bool vistaTheme = IsVistaThemeActive();
        QRect rcClose = GetCloseButtonRect(index, rc, bSelected, vistaTheme);
        
        // Draw close button with theming support (using style or plain image)
        // Original used theme API for Vista; we'll mimic by using style or simple draw
        if (vistaTheme) {
            QStyleOptionButton closeOpt;
            closeOpt.initFrom(this);
            closeOpt.rect = rcClose;
            closeOpt.state |= QStyle::State_Enabled;
            if (bSelected || bHotTracked)
                closeOpt.state |= QStyle::State_MouseOver; // or State_Selected for active?
            style()->drawPrimitive(QStyle::PE_IndicatorTabClose, &closeOpt, &painter, this);
        } else {
            // Fallback: draw the close icon directly
            if (bSelected || bHotTracked)
                painter.drawImage(rcClose.topLeft(), m_iconClose);
            else
                painter.drawImage(rcClose.topLeft(), m_iconCloseDisabled);
        }

        rc.setRight(rcClose.left() - 2);
        if (bSelected)
            rc.setLeft(rc.left() + (vistaTheme ? 4 : 2));
    }

    // Draw text
    painter.save();
    if (bSelected || bHotTracked)
        painter.setPen(palette().color(QPalette::ButtonText));
    else
        painter.setPen(palette().color(QPalette::WindowText));
    
    QRect textRect = rc;
    textRect.setTop(rc.top() + (bSelected ? 4 : 3));
    // Qt draws text centered automatically if we use drawText with alignment
    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextSingleLine, tabText(index));
    painter.restore();

    // Exclude clip (no longer needed in Qt)
}

// Mouse press events
void ClosableTabCtrl::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_bCloseable) {
        QPoint pt = event->pos();
        int tabIndex = GetTabUnderPoint(pt);
        if (tabIndex >= 0) {
            QRect rc = tabRect(tabIndex);
            rc.adjust(-2, -2, 2, 2);
            bool bSelected = (tabIndex == currentIndex());
            bool vistaTheme = IsVistaThemeActive();
            QRect rcClose = GetCloseButtonRect(tabIndex, rc, bSelected, vistaTheme);
            if (!vistaTheme) {
                rcClose.adjust(1, 1, -1, -1); // mimic visible area reduction
            }
            if (rcClose.contains(pt)) {
                m_iTabClose = tabIndex;
                event->accept();
                return;
            }
        }
    }
    QTabBar::mousePressEvent(event);
}

void ClosableTabCtrl::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_bCloseable) {
        if (event->button() == Qt::LeftButton && m_iTabClose != -1) {
            emit tabCloseRequested(m_iTabClose);
            m_iTabClose = -1;
            event->accept();
            return;
        }
        if (event->button() == Qt::MiddleButton) {
            int tabIndex = GetTabUnderPoint(event->pos());
            if (tabIndex >= 0) {
                emit tabCloseRequested(tabIndex);
                event->accept();
                return;
            }
        }
    }
    QTabBar::mouseReleaseEvent(event);
}

void ClosableTabCtrl::mouseDoubleClickEvent(QMouseEvent *event)
{
    int tabIndex = GetTabUnderPoint(event->pos());
    if (tabIndex >= 0) {
        emit tabDoubleClicked(tabIndex);
        event->accept();
        return;
    }
    QTabBar::mouseDoubleClickEvent(event);
}

// Theme change or system colors change
void ClosableTabCtrl::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::PaletteChange) {
        // Re-load icons if needed (original responded to OnSysColorChange)
        SetAllIcons();
    }
    QTabBar::changeEvent(event);
}
