// Contacts.h - Qt version of the MFC Contacts dialog
#ifndef CONTACTS_H
#define CONTACTS_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QShowEvent>
#include <QCloseEvent>
#include <QKeyEvent>

// Forward declarations (defined elsewhere)
class AddDlg;
class Contact;
struct ContactWithFields;
class MainDlg;   // global main window pointer (extern MainDlg* mainDlg)

// ------------------------------------------------------------------
// Contacts dialog class
class Contacts : public QDialog
{
    Q_OBJECT

public:
    explicit Contacts(QWidget* parent = nullptr);
    ~Contacts() override;

    // Public methods (same as original)
    bool isFiltered(Contact* contact = nullptr) const;
    bool isFiltered(Contact* contact, const QString& filterText) const;
    bool isFiltered(const QString& filterText) const;    // helper
    void filterReset();
    void ContactsLoad();
    void ContactsSave();
    bool ContactAdd(Contact contact, bool save = true, bool load = false,
                    QStringList* fields = nullptr, const QString& oldNumber = QString(),
                    bool manual = false);
    void ContactsAdd(QList<ContactWithFields*>* contactsWithFields, bool directory = false);
    void ContactDelete(int i);
    void ContactDeleteRaw(Contact* contact);
    void PresenceReset(Contact* pContact = nullptr);
    void PresenceReceived(const QString& buddyNumber, int image, bool ringing,
                          const QString& info, bool fromUsersDirectory = false);
    void PresenceSubscribe();
    void SetCanditates();
    int DeleteCanditates();
    Contact* FindContact(const QString& number, bool subscribed = false);
    QString GetNameByNumber(const QString& number);
    bool Import(const QString& filename, QList<ContactWithFields*>& contactsWithFields);
    void DefaultItemAction(int i);          // used internally
    void defaultItemAction(Contact* contact, int row);
    void messageDlgOpen(bool isCall = false, bool hasVideo = false, int index = 0);
    void MessageDlgOpen(bool isCall = false, bool hasVideo = false, int index = 0);
    void OnMenuAdd() { onMenuAdd(); }
    const QList<Contact*>& contacts() const { return m_contacts; }

    // Sorting (replaces m_SortItemsExListCtrl)
    void sortColumn(int column, bool ascending);
    int currentSortColumn = 0;
    bool currentSortAscending = true;

    // UI member (kept as public for compatibility with original code)
    QTableWidget* m_list = nullptr;         // was CListCtrl_SortItemsEx
    AddDlg* addDlg = nullptr;               // was AddDlg* addDlg
    AddDlg* m_addDlg = nullptr;

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    // Qt slots for UI events (replaces message map entries)
    void onHeaderSectionResized(int logicalIndex, int oldSize, int newSize);
    void onFilterTextChanged(const QString& text);
    void onItemDoubleClicked(int row, int column);
    void onOkClicked();
    void onCancelClicked();
    void onContextMenuRequested(const QPoint& pos);

    // Menu actions (commands)
    void onMenuCallPickup();
    void onMenuCall();
    void onMenuCallPhone();
    void onMenuCallVideo();
    void onMenuCallMobile();
    void onMenuChat();
    void onMenuAdd();
    void onMenuEdit();
    void onMenuCopy();
    void onMenuDelete();
    void onMenuImport();
    void onMenuExport();

    void onTimerContactsBlink();            // timer callback

    void contactDelete(int row);
    void contactDeleteRaw(Contact* contact);
    int getSelectedRow() const;
    QList<int> getSelectedRows() const;
    void presenceUnsubscribeOne(Contact* pContact);

private:
    // UI widgets (replaces GetDlgItem calls)
    QLineEdit* m_filterEdit = nullptr;      // IDC_FILER_VALUE
    QPushButton* m_okButton = nullptr;      // IDOK
    QPushButton* m_cancelButton = nullptr;  // IDCANCEL

    // Data container (replaces CList<Contact*> contacts)
    QList<Contact*> m_contacts;

    // Timer for blinking (replaces static UINT_PTR blinkTimer)
    QTimer* blinkTimer = nullptr;
    bool blinkState = false;                // static bool blinkState

    // Helpers (private)
    void setupUi();
    void initDialog();
    void applyFilter(const QString& filterText);
    void appendContactToList(Contact* contact, bool subscribe = false);
    bool contactUpdate(int row, Contact* contact, Contact* newContact, QStringList* fields);
    void contactCreate(Contact* pContact, bool subscribe = false);
    Contact* getContactFromRow(int row) const;

    // Static helpers (originally global functions)
    static bool ContactPrepare(Contact* contact);
    static void ContactDecode(const QString& str, Contact& contact);
};

#endif // CONTACTS_H
