#pragma once

#include <QTableWidget>
#include <QPoint>
#include <QString>
#include <QHelpEvent>

class CListCtrl_ToolTip : public QTableWidget
{
    Q_OBJECT

public:
    explicit CListCtrl_ToolTip(QWidget *parent = nullptr) : QTableWidget(parent) {}

    // Original method signatures mapped to Qt
    bool OnToolNeedText(uint id, QHelpEvent *event);
    void CellHitTest(const QPoint &pt, int &nRow, int &nCol) const;
    bool ShowToolTip(const QPoint &pt) const;
    virtual QString GetToolTipText(int nRow, int nCol);

protected:
    // Replaces TTN_NEEDTEXT handling
    bool viewportEvent(QEvent *event) override;
};