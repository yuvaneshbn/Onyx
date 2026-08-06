// Calls.cpp
#include "Calls.h"
#include "AddDlg.h"
#include "global.h"
#include "settings.h"
#include "langpack.h"
#include "mainDlg.h"
#include "CSVFile.h"
#include "Markup.h"

#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSet>
#include <QStringConverter>
#include <QXmlStreamWriter>
#include <QFile>
#include <QFont>
#include <QScreen>
#include <QGuiApplication>

// Column indices
enum {
    COL_NAME = 0,
    COL_NUMBER,
    COL_TIME,
    COL_DURATION,
    COL_INFO,
    COL_COUNT
};

// ---------------------------------------------------------------------------
Calls::Calls(QWidget *parent)
    : BaseDialog(parent)
    , m_table(nullptr)
    , m_filterEdit(nullptr)
    , m_timer(nullptr)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() | Qt::Window); // if used as a standalone window?
    initUi();
    setupTable();
    setupFilter();
    setupTimers();
    populateIcons();
    // defer loading until showEvent to match MFC's OnInitDialog
}

Calls::~Calls()
{
    for (auto pCall : m_calls) delete pCall;
}

void Calls::initUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    auto *filterLayout = new QHBoxLayout;
    m_filterEdit = new QLineEdit(this);
    m_filterEdit->setPlaceholderText(Translate("Filter..."));
    filterLayout->addWidget(m_filterEdit);
    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);

    m_table = new QTableWidget(0, COL_COUNT, this);
    m_table->setHorizontalHeaderLabels({
        Translate("Name"),
        Translate("Number"),
        Translate("Time"),
        Translate("Duration"),
        Translate("Info")
    });
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->verticalHeader()->hide();
    m_table->setSortingEnabled(false); // we do manual sort
    m_table->horizontalHeader()->setSectionsMovable(false);
    mainLayout->addWidget(m_table);

    // Configure column widths from settings
    QHeaderView *header = m_table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setStretchLastSection(true);
    int widths[] = { accountSettings.callsWidth0, accountSettings.callsWidth1, accountSettings.callsWidth2,
                     accountSettings.callsWidth3, accountSettings.callsWidth4 };
    for (int i = 0; i < COL_COUNT; ++i)
        m_table->setColumnWidth(i, widths[i] > 0 ? widths[i] : (i == 0 ? 160 : (i == 1 ? 100 : (i == 2 ? 135 : (i == 3 ? 70 : 120)))));

    connect(header, &QHeaderView::sectionResized, this, &Calls::onHeaderResized);
    connect(m_table, &QTableWidget::cellDoubleClicked, this, &Calls::onTableDoubleClicked);
    connect(m_filterEdit, &QLineEdit::textChanged, this, &Calls::onFilterTextChanged);

    // DPI-aware font (like original)
    QFont tableFont("Microsoft Sans Serif", 8);
    int dpiY = QGuiApplication::primaryScreen()->logicalDotsPerInchY();
    tableFont.setPixelSize(qRound(12.0 * dpiY / 96.0));
    m_table->setFont(tableFont);
    m_filterEdit->setFont(tableFont);
}

void Calls::populateIcons()
{
    // Add icons in the same order as original image list indices
    m_iconList.clear();
    m_iconList.append(QIcon(":/icons/call_out.ico").pixmap(16, 16));   // IDI_CALL_OUT
    m_iconList.append(QIcon(":/icons/call_in.ico").pixmap(16, 16));    // IDI_CALL_IN
    m_iconList.append(QIcon(":/icons/call_miss.ico").pixmap(16, 16));  // IDI_CALL_MISS
    m_iconList.append(QIcon(":/icons/call_miss1.ico").pixmap(16, 16));// IDI_CALL_MISS_1
}

void Calls::setupTimers()
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Calls::ReloadTime);
    m_timer->start(300000); // 5 minutes
}

void Calls::setupTable()
{
}

void Calls::setupFilter()
{
}

// ---------------------------------------------------------------------------
// MFC overrides – now showEvent
// ---------------------------------------------------------------------------
void Calls::showEvent(QShowEvent *event)
{
    if (!event->spontaneous()) {
        // First time show: load calls (like OnInitDialog)
        CallsLoad();
        // Auto move children (not needed with layout, but original used AutoMove)
        // We can skip because we use layout stretching.
    }
    BaseDialog::showEvent(event);
}

void Calls::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    // Not used – we use QTimer.
}

// ---------------------------------------------------------------------------
// Table column resize persistence
// ---------------------------------------------------------------------------
void Calls::onHeaderResized(int logicalIndex, int, int newSize)
{
    switch (logicalIndex) {
    case 0: accountSettings.callsWidth0 = newSize; break;
    case 1: accountSettings.callsWidth1 = newSize; break;
    case 2: accountSettings.callsWidth2 = newSize; break;
    case 3: accountSettings.callsWidth3 = newSize; break;
    case 4: accountSettings.callsWidth4 = newSize; break;
    }
    scheduleSettingsSave();
}

void Calls::scheduleSettingsSave()
{
    if (mainDlg) mainDlg->AccountSettingsPendingSave();
}

// ---------------------------------------------------------------------------
// Filtering
// ---------------------------------------------------------------------------
void Calls::onFilterTextChanged(const QString &text)
{
    // Original: clear and reload when filter changes.
    // We'll rebuild the table from m_calls, applying filter.
    m_table->setRowCount(0);
    for (auto pCall : m_calls) {
        if (!isFiltered(pCall))
            insertCall(pCall);
    }
}

bool Calls::isFiltered(Call *pCall)
{
    QString filter = m_filterEdit->text().trimmed();
    if (filter.isEmpty())
        return false;
    if (!pCall)
        return true;
    QString name = pCall->name.toLower();
    QString number = pCall->number.toLower();
    return !(name.contains(filter) || number.contains(filter));
}

void Calls::filterReset()
{
    m_filterEdit->clear();   // will trigger textChanged
}

// ---------------------------------------------------------------------------
// Table double click / default action
// ---------------------------------------------------------------------------
void Calls::onTableDoubleClicked(int row, int /*column*/)
{
    defaultItemAction(row);
}

void Calls::defaultItemAction(int row)
{
    if (accountSettings.defaultAction.isEmpty()) {
        messageDlgOpen(accountSettings.singleMode);
    } else if (accountSettings.defaultAction == "call") {
        onMenuCall();
#ifdef _GLOBAL_VIDEO
    } else if (accountSettings.defaultAction == "video") {
        onMenuCallVideo();
#endif
    } else {
        onMenuChat();
    }
}

void Calls::messageDlgOpen(bool isCall, bool hasVideo)
{
    int row = m_table->currentRow();
    if (row < 0) return;
    Call *pCall = m_calls[row];
    if (isCall) {
        mainDlg->MakeCall(pCall->number, hasVideo, false, pCall->type != ONYX_CALL_OUT, pCall->name);
    } else {
        mainDlg->MessagesOpen(pCall->number, false, pCall->type != ONYX_CALL_OUT, pCall->name);
    }
}

// ---------------------------------------------------------------------------
// Button actions
// ---------------------------------------------------------------------------
void Calls::onOkClicked()
{
    int row = m_table->currentRow();
    if (row >= 0)
        defaultItemAction(row);
}

void Calls::onCancelClicked()
{
    mainDlg->hide(); // or close? Original showed SW_HIDE
}

void Calls::onMenuCall()
{
    messageDlgOpen(true);
}

#ifdef _GLOBAL_VIDEO
void Calls::onMenuCallVideo()
{
    messageDlgOpen(true, true);
}
#endif

void Calls::onMenuChat()
{
    if (!accountSettings.disableMessaging)
        messageDlgOpen(false);
}

void Calls::onMenuAdd()
{
    mainDlg->pageContacts->OnMenuAdd();
    int row = m_table->currentRow();
    if (row >= 0) {
        Call *pCall = m_calls[row];
        Contact contact;
        contact.number = pCall->number;
        contact.name = pCall->name;
        mainDlg->pageContacts->addDlg->Load(&contact);
    }
}

void Calls::onMenuCopy()
{
    int row = m_table->currentRow();
    if (row >= 0) {
        Call *pCall = m_calls[row];
        mainDlg->CopyStringToClipboard(pCall->number);
    }
}

void Calls::onMenuDelete()
{
    QList<int> rows;
    for (auto item : m_table->selectedItems())
        rows.append(item->row());
    // Remove duplicates and sort descending to delete from bottom
    QSet<int> uniqueRows = QSet<int>(rows.begin(), rows.end());
    QList<int> sortedRows = uniqueRows.values();
    std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());
    for (int row : sortedRows)
        Delete(row);
}

void Calls::onMenuExport()
{
    QString filter = "CSV Files (*.csv);;XML Files (*.xml)";
    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(this, Translate("Export Calls"), "Calls.csv", filter, &selectedFilter);
    if (filename.isEmpty()) return;

    if (selectedFilter.contains("xml")) {
        if (!filename.endsWith(".xml")) filename += ".xml";
        exportToXml(filename);
    } else {
        if (!filename.endsWith(".csv")) filename += ".csv";
        exportToCsv(filename);
    }
}

void Calls::exportToCsv(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    // Header
    stream << "Type,Name,Number,Time,Duration,Info\n";
    for (auto pCall : m_calls) {
        QString typeStr = pCall->type == ONYX_CALL_OUT ? "out" :
                          (pCall->type == ONYX_CALL_IN ? "in" :
                          (pCall->type == ONYX_CALL_MISS ? "miss" : "else"));
        stream << typeStr << "," << pCall->name << "," << pCall->number << ","
               << pCall->time << "," << pCall->duration << "," << pCall->info << "\n";
    }
    file.close();
}

void Calls::exportToXml(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("calls");
    for (auto pCall : m_calls) {
        xml.writeStartElement("call");
        xml.writeAttribute("type", pCall->type == ONYX_CALL_OUT ? "out" :
                                   (pCall->type == ONYX_CALL_IN ? "in" :
                                   (pCall->type == ONYX_CALL_MISS ? "miss" : "else")));
        xml.writeAttribute("name", pCall->name);
        xml.writeAttribute("number", pCall->number);
        xml.writeAttribute("time", QString::number(pCall->time));
        xml.writeAttribute("duration", QString::number(pCall->duration));
        xml.writeAttribute("info", pCall->info);
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

// ---------------------------------------------------------------------------
// Context menu (original OnContextMenu)
// ---------------------------------------------------------------------------
void Calls::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    int selectedRow = m_table->currentRow();
    bool hasSelection = (selectedRow >= 0);

    // Determine if multiple selection (original checked a second selected item)
    QList<QTableWidgetItem*> sel = m_table->selectedItems();
    QSet<int> rows;
    for (auto item : sel) rows.insert(item->row());
    bool multiSelection = rows.size() > 1;

    QAction *callAction = menu.addAction(Translate("Call"), this, &Calls::onMenuCall);
    QAction *chatAction = menu.addAction(Translate("Chat"), this, &Calls::onMenuChat);
    QAction *copyAction = menu.addAction(Translate("Copy Number"), this, &Calls::onMenuCopy);
    QAction *deleteAction = menu.addAction(Translate("Delete"), this, &Calls::onMenuDelete);
    menu.addSeparator();
    QAction *addAction = menu.addAction(Translate("Add Contact"), this, &Calls::onMenuAdd);
    menu.addSeparator();
    QAction *exportAction = menu.addAction(Translate("Export"), this, &Calls::onMenuExport);

    // Enable/disable based on selection
    if (hasSelection) {
        if (multiSelection) {
            // multiple selection: disable single-item actions, enable delete
            callAction->setEnabled(false);
            chatAction->setEnabled(false);
            copyAction->setEnabled(false);
            deleteAction->setEnabled(true);
        } else {
            callAction->setEnabled(true);
            chatAction->setEnabled(true);
            copyAction->setEnabled(true);
            deleteAction->setEnabled(true);
        }
        // "Add" disabled if contact already exists for the selected call
        Call *pCall = m_calls[selectedRow];
        if (mainDlg->pageContacts->FindContact(pCall->number))
            addAction->setEnabled(false);
        else
            addAction->setEnabled(true);
    } else {
        // no selection
        callAction->setEnabled(false);
        chatAction->setEnabled(false);
        copyAction->setEnabled(false);
        deleteAction->setEnabled(false);
        addAction->setEnabled(false);
    }

#ifdef _GLOBAL_VIDEO
    QAction *videoAction = menu.addAction(Translate("Video Call"), this, &Calls::onMenuCallVideo);
    if (accountSettings.disableVideo) videoAction->setVisible(false);
    if (hasSelection && !multiSelection) videoAction->setEnabled(true);
    else videoAction->setEnabled(false);
#endif

    if (accountSettings.disableMessaging)
        chatAction->setVisible(false);

    menu.exec(event->globalPos());
}

// ---------------------------------------------------------------------------
// Add / Update calls
// ---------------------------------------------------------------------------
void Calls::Add(pj_str_t id, const QString &number, const QString &name, int type, call_user_data * /*user_data*/)
{
    SIPURI sipuri;
    Onyx::ParseSIPURI(number, &sipuri);

    QString numberLocal;
    if (sipuri.user.isEmpty()) {
        numberLocal = sipuri.domain + sipuri.parameters + sipuri.commands;
    } else {
        if (sipuri.parameters.isEmpty() && (get_account_domain() == sipuri.domain || sipuri.domain.isEmpty())) {
            numberLocal = sipuri.user + sipuri.commands;
        } else {
            numberLocal = sipuri.user + "@" + sipuri.domain + sipuri.parameters + sipuri.commands;
        }
        if (!accountSettings.account.dialingPrefix.isEmpty()) {
            if (numberLocal.startsWith(accountSettings.account.dialingPrefix))
                numberLocal = numberLocal.mid(accountSettings.account.dialingPrefix.length());
        }
    }

    QString callId = Onyx::PjToStr(&id);
    int index = Get(callId);
    if (index == -1) {
        ReloadTime();
        Call *pCall = new Call();
        pCall->id = callId;
        pCall->number = numberLocal;
        pCall->name = name;
        pCall->type = type;
        pCall->time = QDateTime::currentSecsSinceEpoch();
        pCall->duration = 0;
        pCall->key = getNextKey();
        m_calls.append(pCall);
        if (!isFiltered(pCall))
            insertCall(pCall);
        callSave(pCall);
        saveKey();
    } else {
        Call *pCall = m_calls[index];
        bool changed = false;
        if (pCall->number != numberLocal) {
            pCall->number = numberLocal;
            changed = true;
        }
        if (pCall->name != name) {
            pCall->name = name;
            changed = true;
        }
        if (pCall->type != type) {
            pCall->type = type;
            changed = true;
        }
        if (changed) {
            // update table row
            int row = -1;
            for (int r = 0; r < m_table->rowCount(); ++r) {
                if (m_table->item(r, 0)->data(Qt::UserRole).value<Call*>() == pCall) {
                    row = r;
                    break;
                }
            }
            if (row >= 0) {
                updateTableRow(row, pCall);
            }
            callSave(pCall);
        }
    }
}

void Calls::SetName(pj_str_t id, const QString &name)
{
    QString callId = Onyx::PjToStr(&id);
    int index = Get(callId);
    if (index >= 0) {
        Call *pCall = m_calls[index];
        pCall->name = name;
        int row = findRowForCall(pCall);
        if (row >= 0)
            m_table->item(row, COL_NAME)->setText(name);
        callSave(pCall);
    }
}

void Calls::SetDuration(pj_str_t id, int sec, int /*total*/)
{
    QString callId = Onyx::PjToStr(&id);
    int index = Get(callId);
    if (index >= 0) {
        Call *pCall = m_calls[index];
        pCall->duration = sec;
        int row = findRowForCall(pCall);
        if (row >= 0)
            m_table->item(row, COL_DURATION)->setText(Onyx::GetDuration(sec));
        callSave(pCall);
    }
}

void Calls::SetInfo(pj_str_t id, const QString &str)
{
    QString callId = Onyx::PjToStr(&id);
    int index = Get(callId);
    if (index >= 0) {
        Call *pCall = m_calls[index];
        pCall->info = str;
        int row = findRowForCall(pCall);
        if (row >= 0)
            m_table->item(row, COL_INFO)->setText(str);
        callSave(pCall);
    }
}

int Calls::Get(const QString &id)
{
    for (int i = 0; i < m_calls.size(); ++i) {
        if (m_calls[i]->id == id)
            return i;
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Insert a call into the table at the given position (sorted)
// ---------------------------------------------------------------------------
void Calls::insertCall(Call *pCall, int pos)
{
    int row;
    if (pos < 0) {
        // Append, then sort
        row = m_table->rowCount();
        m_table->insertRow(row);
    } else {
        row = pos;
        m_table->insertRow(row);
    }
    updateTableRow(row, pCall);
    // Store pointer in item for identification
    m_table->item(row, 0)->setData(Qt::UserRole, QVariant::fromValue(pCall));
}

void Calls::updateTableRow(int row, Call *pCall)
{
    // Icon
    QTableWidgetItem *item0 = new QTableWidgetItem();
    if (pCall->type >= 0 && pCall->type < m_iconList.size())
        item0->setIcon(QIcon(m_iconList[pCall->type]));
    item0->setText(pCall->name);
    m_table->setItem(row, COL_NAME, item0);

    m_table->setItem(row, COL_NUMBER, new QTableWidgetItem(pCall->number));
    m_table->setItem(row, COL_TIME, new QTableWidgetItem(FormatTime(pCall->time)));
    m_table->setItem(row, COL_DURATION, new QTableWidgetItem(Onyx::GetDuration(pCall->duration)));
    m_table->setItem(row, COL_INFO, new QTableWidgetItem(pCall->info));
}

int Calls::findRowForCall(Call *pCall) const
{
    if (!pCall) {
        return -1;
    }
    for (int row = 0; row < m_table->rowCount(); ++row) {
        QTableWidgetItem *item = m_table->item(row, COL_NAME);
        if (item && item->data(Qt::UserRole).value<Call*>() == pCall) {
            return row;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Delete
// ---------------------------------------------------------------------------
void Calls::Delete(int row)
{
    if (row < 0 || row >= m_table->rowCount()) return;
    Call *pCall = m_calls[row];
    pCall->number.clear();   // marks as deleted
    callSave(pCall);
    m_table->removeRow(row);
    delete pCall;
    m_calls.removeAt(row);
}

void Calls::DeleteAll()
{
    CallsClear();
    QSettings settings(accountSettings.iniFile, QSettings::IniFormat);
    settings.beginGroup("Calls");
    settings.remove("");
    settings.endGroup();
}

// ---------------------------------------------------------------------------
// Load / Clear
// ---------------------------------------------------------------------------
void Calls::CallsClear()
{
    m_table->setRowCount(0);
    for (auto pCall : m_calls) delete pCall;
    m_calls.clear();
}

void Calls::CallsLoad()
{
    CallsClear();
    QSettings settings(accountSettings.iniFile, QSettings::IniFormat);
    settings.beginGroup("Calls");
    QStringList keys = settings.childKeys();
    int lastKey = -1;
    int maxTime = 0;
    QList<Call*> loadedCalls;

    for (const QString &key : keys) {
        QString val = settings.value(key).toString();
        if (val == "null") continue;
        Call *pCall = new Call();
        callDecode(val, pCall);
        if (pCall->time > maxTime) {
            maxTime = pCall->time;
            lastKey = key.toInt();
        }
        if (!isFiltered(pCall)) {
            loadedCalls.append(pCall);
            pCall->key = key.toInt();
        } else {
            delete pCall;
        }
    }
    settings.endGroup();

    // Sort by time descending (original uses a sort list control)
    std::sort(loadedCalls.begin(), loadedCalls.end(), [](Call *a, Call *b) {
        return a->time > b->time; // descending
    });
    for (auto pCall : loadedCalls) {
        m_calls.append(pCall);
        insertCall(pCall);
    }
    m_lastKey = lastKey;   // continue from highest used key
}

// ---------------------------------------------------------------------------
// Encode / Decode call string
// ---------------------------------------------------------------------------
QString Calls::callEncode(Call *pCall)
{
    return QString("%1;%2;%3;%4;%5;%6")
        .arg(pCall->number, pCall->name)
        .arg(pCall->type)
        .arg(pCall->time)
        .arg(pCall->duration)
        .arg(pCall->info);
}

void Calls::callDecode(const QString &str, Call *pCall)
{
    QStringList parts = str.split(';');
    if (parts.size() < 6) {
        pCall->number = str;
        pCall->name = pCall->number;
        pCall->type = 0;
        pCall->time = 0;
        pCall->duration = 0;
        return;
    }
    pCall->number = parts[0];
    pCall->name = parts[1];
    pCall->type = parts[2].toInt();
    pCall->time = parts[3].toInt();
    pCall->duration = parts[4].toInt();
    pCall->info = parts[5];
}

// ---------------------------------------------------------------------------
// Key management
// ---------------------------------------------------------------------------
int Calls::getNextKey(bool noInc)
{
    if (m_lastKey == -1) {
        QSettings settings(accountSettings.iniFile, QSettings::IniFormat);
        m_lastKey = settings.value("Settings/callsLastKey", -1).toInt();
    }
    int key = m_lastKey;
    if (!noInc) {
        m_lastKey++;
        if (m_lastKey >= 1000) m_lastKey = 0;
    }
    return key;
}

void Calls::saveKey()
{
    QSettings settings(accountSettings.iniFile, QSettings::IniFormat);
    settings.setValue("Settings/callsLastKey", m_lastKey);
}

void Calls::callSave(Call *pCall)
{
    if (pCall->number.isEmpty()) {
        // Delete
        QSettings settings(accountSettings.iniFile, QSettings::IniFormat);
        settings.beginGroup("Calls");
        settings.remove(QString::number(pCall->key));
        settings.endGroup();
    } else {
        QString data = callEncode(pCall);
        QSettings settings(accountSettings.iniFile, QSettings::IniFormat);
        settings.beginGroup("Calls");
        settings.setValue(QString::number(pCall->key), data);
        settings.endGroup();
    }
}

// ---------------------------------------------------------------------------
// Time formatting
// ---------------------------------------------------------------------------
QString Calls::FormatTime(int time, const QDateTime *pTimeNow)
{
    QDateTime dt;
    if (pTimeNow)
        dt = *pTimeNow;
    else
        dt = QDateTime::currentDateTime();

    QDateTime callTime = QDateTime::fromSecsSinceEpoch(time);
    if (callTime.date() == dt.date())
        return callTime.toString("hh:mm:ss");
    else
        return callTime.toString("yyyy-MM-dd hh:mm:ss");
}

void Calls::ReloadTime()
{
    QDateTime now = QDateTime::currentDateTime();
    if (m_lastDay && m_lastDay != now.date().day()) {
        m_lastDay = now.date().day();
        for (int r = 0; r < m_table->rowCount(); ++r) {
            Call *pCall = m_calls[r];
            QTableWidgetItem *item = m_table->item(r, COL_TIME);
            if (item)
                item->setText(FormatTime(pCall->time, &now));
        }
    }
}
