// CListCtrl_SortItemsEx.cpp
#include "CListCtrl_SortItemsEx.h"
#include "Calls.h"
#include "Contacts.h"
#include "global.h"        // for Call, Contact structs
#include "settings.h"
#include "mainDlg.h"
#include <algorithm>

CListCtrl_SortItemsEx::CListCtrl_SortItemsEx(QWidget *parent)
    : CListCtrl_Sortable(parent)
{
}

bool CListCtrl_SortItemsEx::SortColumn(int columnIndex, bool ascending)
{
    if (rowCount() == 0)
        return true;

    // Identify the type of data stored in UserRole (Call* or Contact*).
    void *samplePtr = nullptr;
    if (QTableWidgetItem *item0 = item(0, 0))
        samplePtr = item0->data(Qt::UserRole).value<void*>();

    if (!samplePtr)
        return true;

    // We'll sort the rows by collecting all row data and then re‑inserting.
    QList<QList<QTableWidgetItem*>> allRows;
    for (int r = 0; r < rowCount(); ++r) {
        QList<QTableWidgetItem*> rowItems;
        for (int c = 0; c < columnCount(); ++c)
            rowItems.append(takeItem(r, c));
        allRows.append(rowItems);
    }

    // Sort indices based on the custom comparison.
    QList<int> indices;
    for (int i = 0; i < allRows.size(); ++i)
        indices.append(i);

    // Comparison lambda – mimics original PARAMSORT
    auto compare = [&](int a, int b) -> bool {
        void *ptr1 = allRows[a].value(0) ? allRows[a][0]->data(Qt::UserRole).value<void*>() : nullptr;
        void *ptr2 = allRows[b].value(0) ? allRows[b][0]->data(Qt::UserRole).value<void*>() : nullptr;
        if (!ptr1 || !ptr2) return false;

        // Calls special handling (time column 2)
        Call *call1 = dynamic_cast<Call*>(static_cast<Call*>(ptr1));
        Call *call2 = dynamic_cast<Call*>(static_cast<Call*>(ptr2));
        if (call1 && call2 && mainDlg && mainDlg->pageCalls && this == mainDlg->pageCalls->table()) {
            if (columnIndex == 2) {   // Time
                if (ascending)
                    return call1->time < call2->time;
                else
                    return call1->time > call2->time;
            }
        }

        // Contacts special handling: starred first
        Contact *contact1 = dynamic_cast<Contact*>(static_cast<Contact*>(ptr1));
        Contact *contact2 = dynamic_cast<Contact*>(static_cast<Contact*>(ptr2));
        if (contact1 && contact2) {
            if (contact1->starred != contact2->starred)
                return contact1->starred > contact2->starred;   // starred before non‑starred
        }

        // Generic text comparison
        QString left  = (allRows[a].value(columnIndex) ? allRows[a][columnIndex]->text() : QString());
        QString right = (allRows[b].value(columnIndex) ? allRows[b][columnIndex]->text() : QString());
        int cmp = QString::compare(left, right, Qt::CaseInsensitive);
        if (cmp != 0)
            return ascending ? (cmp < 0) : (cmp > 0);

        // Tie breaker: compare first column
        if (columnIndex > 0) {
            left  = (allRows[a].value(0) ? allRows[a][0]->text() : QString());
            right = (allRows[b].value(0) ? allRows[b][0]->text() : QString());
            cmp = QString::compare(left, right, Qt::CaseInsensitive);
            return cmp < 0;   // always ascending for tiebreaker
        }
        return false;
    };

    std::stable_sort(indices.begin(), indices.end(), compare);

    // Rebuild the table in sorted order
    setRowCount(0);
    for (int idx : indices) {
        int newRow = rowCount();
        insertRow(newRow);
        for (int c = 0; c < columnCount(); ++c) {
            if (c < allRows[idx].size())
                setItem(newRow, c, allRows[idx][c]);
        }
    }

    // Update sort arrow
    SetSortArrow(columnIndex, ascending);
    return true;
}
