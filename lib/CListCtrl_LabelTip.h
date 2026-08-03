#pragma once

#include "CListCtrl_ToolTip.h"
#include <QFontMetrics>
#include <QHeaderView>

class CListCtrl_LabelTip : public CListCtrl_ToolTip
{
    Q_OBJECT

public:
    explicit CListCtrl_LabelTip(QWidget *parent = nullptr)
        : CListCtrl_ToolTip(parent)
    {
        // Equivalent of PreSubclassWindow: enable label tip mode
        // (No direct Qt style, we override GetToolTipText instead)
        setMouseTracking(true);   // ensure mouse move events are captured for tooltip
    }

protected:
    // Override GetToolTipText to show tooltip only for partially visible text
    QString GetToolTipText(int nRow, int nCol) override
    {
        if (nRow == -1 || nCol == -1)
            return QString();

        QTableWidgetItem *item = this->item(nRow, nCol);
        if (!item)
            return QString();

        QString cellText = item->text();
        if (cellText.isEmpty())
            return QString();

        // Check if the text is fully visible within the column width
        QFontMetrics fm(item->font());
        int textWidth = fm.horizontalAdvance(cellText);
        int colWidth = columnWidth(nCol);

        // If the text fits, do not show tooltip (mimics LVS_EX_LABELTIP)
        if (textWidth <= colWidth)
            return QString();

        return cellText;
    }
};