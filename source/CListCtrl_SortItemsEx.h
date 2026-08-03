// CListCtrl_SortItemsEx.h
#pragma once

#include "CListCtrl_Sortable.h"

// Concrete implementation that sorts Calls and Contacts data.
class CListCtrl_SortItemsEx : public CListCtrl_Sortable
{
    Q_OBJECT
public:
    explicit CListCtrl_SortItemsEx(QWidget *parent = nullptr);
    virtual bool SortColumn(int columnIndex, bool ascending) override;
};