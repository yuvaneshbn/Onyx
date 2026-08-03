// Calls.h
#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QTimer>
#include <QDateTime>
#include <QList>
#include <QPixmap>
#include <QFile>

#include "BaseDialog.h"
#include "global.h"          // for Call struct, etc.
#include "CSVFile.h"         // assumed ported or kept
#include "Markup.h"          // assumed ported or kept

class Calls : public BaseDialog
{
    Q_OBJECT
public:
    explicit Calls(QWidget *parent = nullptr);
    ~Calls();

    void TabFocusSet() override {}
    bool GotoTab(int i) override { return false; }
    void ProcessCommand(const QString &str) override {}

    int Get(const QString &id);
    void Add(pj_str_t id, const QString &number, const QString &name, int type, call_user_data *user_data);
    void SetName(pj_str_t id, const QString &name);
    void SetDuration(pj_str_t id, int sec, int total);
    void SetInfo(pj_str_t id, const QString &str);
    void Delete(int i);
    void DeleteAll();
    void UpdateCallButton();
    QTableWidget *table() const { return m_table; }

    void CallsLoad();
    void CallsClear();
    QString FormatTime(int time, const QDateTime *pTimeNow = nullptr);
    void ReloadTime();
    bool isFiltered(Call *pCall = nullptr);
    void filterReset();

    void OnCreated();

private:
    void initUi();
    void setupTable();
    void setupFilter();
    void setupTimers();
    void populateIcons();
    void callSave(Call *pCall);
    void callDecode(const QString &str, Call *pCall);
    QString callEncode(Call *pCall);
    void insertCall(Call *pCall, int pos = -1);
    void updateTableRow(int row, Call *pCall);
    int findRowForCall(Call *pCall) const;
    void messageDlgOpen(bool isCall = false, bool hasVideo = false);
    void defaultItemAction(int i);
    int getNextKey(bool noInc = false);
    void saveKey();

    void exportToCsv(const QString &filename);
    void exportToXml(const QString &filename);

    QTableWidget *m_table;
    QLineEdit *m_filterEdit;
    QTimer *m_timer;
    QList<QPixmap> m_iconList;   // replaces CImageList
    int m_lastDay = 0;
    int m_lastKey = -1;
    QList<Call*> m_calls;        // master list of calls
    int m_sortColumn = 2;        // default sort column (Time)
    Qt::SortOrder m_sortOrder = Qt::DescendingOrder;

    // Pending save
    void scheduleSettingsSave();

protected:
    void showEvent(QShowEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onTableDoubleClicked(int row, int column);
    void onHeaderResized(int logicalIndex, int oldSize, int newSize);
    void onFilterTextChanged(const QString &text);
    void onOkClicked();
    void onCancelClicked();
    void onMenuCall();
    void onMenuChat();
    void onMenuAdd();
    void onMenuCopy();
    void onMenuDelete();
    void onMenuExport();
#ifdef _GLOBAL_VIDEO
    void onMenuCallVideo();
#endif
};
