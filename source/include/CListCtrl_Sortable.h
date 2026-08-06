// CListCtrl_Sortable.h
#pragma once

#include <QTableWidget>
#include <QHeaderView>
#include <QShowEvent>

// Replaces CListCtrl_LabelTip with QTableWidget.
// The original sorting behaviour is preserved via header click handling.
class CListCtrl_Sortable : public QTableWidget
{
    Q_OBJECT
public:
    explicit CListCtrl_Sortable(QWidget *parent = nullptr);
    virtual ~CListCtrl_Sortable() {}

    // Original public interface
    int GetSortColumn() const { return m_SortCol; }
    bool IsAscending() const { return m_Ascending; }
    void ResetSortOrder();
    void SetSortColumn(int columnIndex, bool ascending);
    void SetSortArrow(int col, bool ascending);
    int GetColumnData(int col) const;   // kept for compatibility

    // Pure virtual – must be implemented in derived class
    virtual bool SortColumn(int columnIndex, bool ascending) = 0;

protected:
    bool m_Ascending;
    int  m_SortCol;

    // Called once when the widget is shown (like PreSubclassWindow)
    void showEvent(QShowEvent *event) override;
    void InitControl();

private slots:
    void OnHeaderClicked(int section);
};