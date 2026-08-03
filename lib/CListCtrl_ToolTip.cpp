#include "stdafx.h"
#include "CListCtrl_ToolTip.h"
#include <QToolTip>
#include <QHelpEvent>
#include <QTableWidgetItem>

// ------------------------------------------------------------
// CellHitTest: finds the cell at a given viewport point
// ------------------------------------------------------------
void CListCtrl_ToolTip::CellHitTest(const QPoint &pt, int &nRow, int &nCol) const
{
    nRow = -1;
    nCol = -1;

    QTableWidgetItem *item = itemAt(pt);
    if (item) {
        nRow = item->row();
        nCol = item->column();
    }
}

// ------------------------------------------------------------
// OnToolNeedText: shows the tooltip if the cell has a valid tooltip text
// ------------------------------------------------------------
bool CListCtrl_ToolTip::OnToolNeedText(uint id, QHelpEvent *event)
{
    Q_UNUSED(id);

    // Convert event position to viewport coordinates
    QPoint viewportPos = viewport()->mapFromGlobal(event->globalPos());
    int nRow, nCol;
    CellHitTest(viewportPos, nRow, nCol);

    QString tooltip = GetToolTipText(nRow, nCol);
    if (tooltip.isEmpty())
        return false;

    QToolTip::showText(event->globalPos(), tooltip, this);
    return true;
}

// ------------------------------------------------------------
// ShowToolTip: returns true if a valid cell is under the point
// ------------------------------------------------------------
bool CListCtrl_ToolTip::ShowToolTip(const QPoint &pt) const
{
    int nRow, nCol;
    CellHitTest(pt, nRow, nCol);
    return (nRow != -1 && nCol != -1);
}

// ------------------------------------------------------------
// GetToolTipText: returns the cell's text (can be overridden)
// ------------------------------------------------------------
QString CListCtrl_ToolTip::GetToolTipText(int nRow, int nCol)
{
    if (nRow != -1 && nCol != -1) {
        QTableWidgetItem *item = this->item(nRow, nCol);
        if (item)
            return item->text();
    }
    return QString();
}

// ------------------------------------------------------------
// viewportEvent: intercepts ToolTip events to call OnToolNeedText
// ------------------------------------------------------------
bool CListCtrl_ToolTip::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        if (OnToolNeedText(0, helpEvent))
            return true;   // event handled
    }
    return QTableWidget::viewportEvent(event);
}