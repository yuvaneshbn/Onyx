// CListCtrl_Sortable.cpp
#include "CListCtrl_Sortable.h"
#include "mainDlg.h"       // needed for resource IDs, if any

CListCtrl_Sortable::CListCtrl_Sortable(QWidget *parent)
    : QTableWidget(parent)
    , m_Ascending(false)
    , m_SortCol(-1)
{
    // Connect header click signal (will be set after header is available)
    connect(horizontalHeader(), &QHeaderView::sectionClicked,
            this, &CListCtrl_Sortable::OnHeaderClicked);
}

void CListCtrl_Sortable::showEvent(QShowEvent *event)
{
    if (!event->spontaneous()) {
        // One‑time initialisation (like PreSubclassWindow)
        InitControl();
    }
    QTableWidget::showEvent(event);
}

void CListCtrl_Sortable::InitControl()
{
    // Mimic original styles:
    // LVS_EX_FULLROWSELECT → row selection
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    // LVS_EX_DOUBLEBUFFER – Qt does double buffering by default
    // LVS_EX_HEADERDRAGDROP
    horizontalHeader()->setSectionsMovable(true);

    // Enable sort indicator (replaces theme enable logic)
    horizontalHeader()->setSortIndicatorShown(true);

    // Apply initial sort column if any
    if (m_SortCol >= 0)
        horizontalHeader()->setSortIndicator(m_SortCol,
            m_Ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
}

void CListCtrl_Sortable::OnHeaderClicked(int section)
{
    // Original OnHeaderClick behaviour
    setFocus();   // ensure other controls lose focus

    if (m_SortCol == section) {
        m_Ascending = !m_Ascending;
    } else {
        m_SortCol = section;
        m_Ascending = true;   // default ascending for new column
    }

    if (SortColumn(m_SortCol, m_Ascending))
        SetSortArrow(m_SortCol, m_Ascending);
}

void CListCtrl_Sortable::SetSortArrow(int colIndex, bool ascending)
{
    // In Qt, the header already shows a sort indicator;
    // we just update it.
    if (colIndex >= 0 && colIndex < horizontalHeader()->count()) {
        horizontalHeader()->setSortIndicator(colIndex,
            ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
    } else {
        // Hide indicator if colIndex is out of range
        horizontalHeader()->setSortIndicatorShown(false);
    }
}

void CListCtrl_Sortable::ResetSortOrder()
{
    m_Ascending = true;
    m_SortCol = -1;
    SetSortArrow(m_SortCol, m_Ascending);
}

int CListCtrl_Sortable::GetColumnData(int col) const
{
    // Original returned the LVCOLUMN iSubItem, which in Qt corresponds
    // to the column index itself.
    return col;
}

void CListCtrl_Sortable::SetSortColumn(int columnIndex, bool ascending)
{
    m_SortCol = columnIndex;
    m_Ascending = ascending;
    if (SortColumn(m_SortCol, m_Ascending))
        SetSortArrow(m_SortCol, m_Ascending);
}