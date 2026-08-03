// ClosableTabCtrl.h
#pragma once

#include <QTabBar>
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QStyleOptionTab>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

class ClosableTabCtrl : public QTabBar
{
    Q_OBJECT
    Q_PROPERTY(bool closable READ isClosable WRITE setClosable)

public:
    explicit ClosableTabCtrl(QWidget *parent = nullptr);
    virtual ~ClosableTabCtrl();

    // Original interface mappings
    void DeleteItem(int index);    // delegates to removeTab
    bool isClosable() const { return m_bCloseable; }
    void setClosable(bool closable);

    // Context menu helpers (if needed)
    int GetTabUnderContextMenu() const;
    bool SetDefaultContextMenuPos();

signals:
    void tabCloseRequested(int index);   // replaces UM_CLOSETAB
    void tabDoubleClicked(int index);    // replaces UM_DBLCLICKTAB
    bool tabQueryClose(int index);       // replaces UM_QUERYTAB (return true to disable close)

protected:
    // Qt event overrides
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void changeEvent(QEvent *event) override;  // for theme/system color changes

    // Custom drawing support
    virtual void DrawItem(QPainter &painter, int index, const QStyleOptionTab &option);

    // Internal helpers
    void InternalInit();
    void SetAllIcons();
    QRect GetCloseButtonRect(int index, const QRect &tabRect, bool selected, bool vistaThemeActive);
    int GetTabUnderPoint(const QPoint &point) const;
    bool IsVistaThemeActive() const;

private:
    bool m_bCloseable;
    QImage m_iconClose;        // active close icon
    QImage m_iconCloseDisabled; // inactive close icon
    QSize m_closeButtonSize;   // replaces IMAGEINFO dimensions
    QPoint m_ptCtxMenu;        // last context menu position
    int m_iTabClose;           // tab index selected for close (left button)
};