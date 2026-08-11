#include "StdAfx.h"
#include "Transfer.h"
#include "MessagesDlg.h"
#include "mainDlg.h"
#include "langpack.h"
#include "settings.h"

Transfer::Transfer(QWidget *parent)
    : QDialog(parent)
{
    // Replicates: Create(IDD, pParent) and the dialog resource layout
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    // Prevent Qt from restoring an invalid tiny geometry saved from a previous MFC session
    setMinimumSize(280, 320);
    setAttribute(Qt::WA_DeleteOnClose, false); // we control deletion via done() override

    // --- Build UI exactly as IDD_TRANSFER ---
    m_comboNumber = new QComboBox(this);
    m_comboNumber->setEditable(true);
    m_comboNumber->setObjectName(QStringLiteral("IDC_NUMBER"));

    m_btnOk = new QPushButton(QStringLiteral("OK"), this);
    m_btnOk->setObjectName(QStringLiteral("IDOK"));
    m_btnCancel = new QPushButton(QStringLiteral("Cancel"), this);
    m_btnCancel->setObjectName(QStringLiteral("IDCANCEL"));
    m_btnAttended = new QPushButton(QStringLiteral("Attended"), this);
    m_btnAttended->setObjectName(QStringLiteral("IDC_TRANSFER_ATTENDED"));
    m_btnBlind = new QPushButton(QStringLiteral("Blind"), this);
    m_btnBlind->setObjectName(QStringLiteral("IDC_TRANSFER_BLIND"));

    m_btnKey1 = new QPushButton(QStringLiteral("1"), this);
    m_btnKey2 = new QPushButton(QStringLiteral("2"), this);
    m_btnKey3 = new QPushButton(QStringLiteral("3"), this);
    m_btnKey4 = new QPushButton(QStringLiteral("4"), this);
    m_btnKey5 = new QPushButton(QStringLiteral("5"), this);
    m_btnKey6 = new QPushButton(QStringLiteral("6"), this);
    m_btnKey7 = new QPushButton(QStringLiteral("7"), this);
    m_btnKey8 = new QPushButton(QStringLiteral("8"), this);
    m_btnKey9 = new QPushButton(QStringLiteral("9"), this);
    m_btnKeyStar = new QPushButton(QStringLiteral("*"), this);
    m_btnKey0 = new QPushButton(QStringLiteral("0"), this);
    m_btnKeyGrate = new QPushButton(QStringLiteral("#"), this);

    // Layout: arrange similar to original dialog (simple grid)
    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(m_comboNumber, 0, 0, 1, 3);
    grid->addWidget(m_btnKey1, 1, 0);
    grid->addWidget(m_btnKey2, 1, 1);
    grid->addWidget(m_btnKey3, 1, 2);
    grid->addWidget(m_btnKey4, 2, 0);
    grid->addWidget(m_btnKey5, 2, 1);
    grid->addWidget(m_btnKey6, 2, 2);
    grid->addWidget(m_btnKey7, 3, 0);
    grid->addWidget(m_btnKey8, 3, 1);
    grid->addWidget(m_btnKey9, 3, 2);
    grid->addWidget(m_btnKeyStar, 4, 0);
    grid->addWidget(m_btnKey0, 4, 1);
    grid->addWidget(m_btnKeyGrate, 4, 2);
    grid->addWidget(m_btnAttended, 5, 0);
    grid->addWidget(m_btnBlind, 5, 1);
    grid->addWidget(m_btnOk, 6, 0);
    grid->addWidget(m_btnCancel, 6, 1);

    // Connect all buttons to their slots (replaces BEGIN_MESSAGE_MAP)
    connect(m_btnOk, &QPushButton::clicked, this, &Transfer::OnBnClickedOk);
    connect(m_btnCancel, &QPushButton::clicked, this, &Transfer::OnBnClickedCancel);
    connect(m_btnAttended, &QPushButton::clicked, this, &Transfer::OnBnClickedAttended);
    connect(m_btnBlind, &QPushButton::clicked, this, &Transfer::OnBnClickedBlind);
    connect(m_btnKey1, &QPushButton::clicked, this, &Transfer::OnBnClickedKey1);
    connect(m_btnKey2, &QPushButton::clicked, this, &Transfer::OnBnClickedKey2);
    connect(m_btnKey3, &QPushButton::clicked, this, &Transfer::OnBnClickedKey3);
    connect(m_btnKey4, &QPushButton::clicked, this, &Transfer::OnBnClickedKey4);
    connect(m_btnKey5, &QPushButton::clicked, this, &Transfer::OnBnClickedKey5);
    connect(m_btnKey6, &QPushButton::clicked, this, &Transfer::OnBnClickedKey6);
    connect(m_btnKey7, &QPushButton::clicked, this, &Transfer::OnBnClickedKey7);
    connect(m_btnKey8, &QPushButton::clicked, this, &Transfer::OnBnClickedKey8);
    connect(m_btnKey9, &QPushButton::clicked, this, &Transfer::OnBnClickedKey9);
    connect(m_btnKeyStar, &QPushButton::clicked, this, &Transfer::OnBnClickedKeyStar);
    connect(m_btnKey0, &QPushButton::clicked, this, &Transfer::OnBnClickedKey0);
    connect(m_btnKeyGrate, &QPushButton::clicked, this, &Transfer::OnBnClickedKeyGrate);

    // Initialise action/callId with safe defaults
    action = ONYX_ACTION_TRANSFER;
    callId = PJSUA_INVALID_ID;
}

Transfer::~Transfer(void)
{
    // Qt handles child widgets; nothing extra needed
}

void Transfer::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    // ON_WM_CREATE equivalent: RTL layout
    if (langPack.rtl) {
        setLayoutDirection(Qt::RightToLeft);
    }

    // ON_INITDIALOG equivalent
    // TranslateDialog(this) – must exist in project
    TranslateDialog(this);

    // Create the scaled font (16pt based on current DPI)
    QFont font = this->font();
    int dpiY = qMax(96, (int)logicalDpiY()); // guard: never 0
    int pixelHeight = qMax(10, (int)MulDiv(16, dpiY, 96));
    font.setPixelSize(pixelHeight);
    m_font = font;

    // Apply font to all controls
    m_comboNumber->setFont(m_font);
    m_btnKey1->setFont(m_font);
    m_btnKey2->setFont(m_font);
    m_btnKey3->setFont(m_font);
    m_btnKey4->setFont(m_font);
    m_btnKey5->setFont(m_font);
    m_btnKey6->setFont(m_font);
    m_btnKey7->setFont(m_font);
    m_btnKey8->setFont(m_font);
    m_btnKey9->setFont(m_font);
    m_btnKey0->setFont(m_font);
    m_btnKeyStar->setFont(m_font);
    m_btnKeyGrate->setFont(m_font);

    // (Optional) Make the window visible; modeless behaviour is automatic
}

void Transfer::closeEvent(QCloseEvent *event)
{
    // ON_CLOSE handler
    if (mainDlg->transferDlg) {
        // DestroyWindow equivalent: just accepting close deletes later
        event->accept();
    }
}

void Transfer::done(int r)
{
    // ON_DESTROY equivalent: clear global pointer and combo data
    mainDlg->transferDlg = NULL;
    ClearDropdown();
    QDialog::done(r);

    // PostNcDestroy equivalent: delete this
    deleteLater();
}

void Transfer::LoadFromContacts(Contact *selectedContact)
{
    ClearDropdown();
    int selectedIndex = -1;
    bool hasStarred = false;

    // First pass: check if any starred contacts exist
    for (auto it = mainDlg->pageContacts->contacts().begin();
         it != mainDlg->pageContacts->contacts().end(); ++it) {
        if ((*it)->starred) {
            hasStarred = true;
            break;
        }
    }

    // Second pass: fill combo
    int i = 0;
    for (auto it = mainDlg->pageContacts->contacts().begin();
         it != mainDlg->pageContacts->contacts().end(); ++it) {
        Contact* contact = *it;
        if (!hasStarred || contact->starred) {
            if (selectedContact == contact) {
                selectedIndex = i;
            }
            m_comboNumber->addItem(contact->name, QVariant::fromValue(QString(contact->number)));
            i++;
        }
    }

    if (selectedIndex != -1) {
        m_comboNumber->setCurrentIndex(selectedIndex);
    }
}

void Transfer::ClearDropdown()
{
    // Qt automatically deletes QVariant data, just clear
    m_comboNumber->clear();
}

void Transfer::SetAction(onyx_action action, pjsua_call_id call_id)
{
    this->action = action;
    callId = call_id;
    bool buttons = false;

    if (action == ONYX_ACTION_TRANSFER || action == ONYX_ACTION_ATTENDED_TRANSFER || action == ONYX_ACTION_FORWARD) {
        if (action == ONYX_ACTION_TRANSFER || action == ONYX_ACTION_ATTENDED_TRANSFER) {
            if (accountSettings.enableFeatureCodeAT
                && !accountSettings.featureCodeAT.isEmpty())
            {
                buttons = true;
            }
        }
        if (buttons || action == ONYX_ACTION_FORWARD) {
            setWindowTitle(Translate(QStringLiteral("Call Transfer")));
        } else {
            if (action == ONYX_ACTION_ATTENDED_TRANSFER) {
                setWindowTitle(Translate(QStringLiteral("Attended Transfer")));
            } else {
                setWindowTitle(Translate(QStringLiteral("Blind Transfer")));
            }
        }
    }
    if (action == ONYX_ACTION_INVITE) {
        setWindowTitle(Translate(QStringLiteral("Invite to Conference")));
    }

    m_btnOk->setVisible(!buttons);
    m_btnAttended->setVisible(buttons);
    m_btnBlind->setVisible(buttons);
}

bool Transfer::Action(onyx_action action)
{
    QString number;
    int i = m_comboNumber->currentIndex();
    if (i == -1) {
        number = m_comboNumber->currentText().trimmed();
    } else {
        number = m_comboNumber->itemData(i).toString();
    }
    if (!number.isEmpty()) {
        mainDlg->messagesDlg->CallAction(action, number, callId);
        return true;
    }
    return false;
}

void Transfer::Input(const QString &digits)
{
    QLineEdit *lineEdit = m_comboNumber->lineEdit();
    if (lineEdit) {
        // Simulate inserting text at the end
        lineEdit->insert(digits);
    }
}

// --- Numeric keypad handlers ---
void Transfer::OnBnClickedOk()
{
    if (!m_btnOk->isVisible()) {
        return;
    }
    if (Action(action)) {
        close();  // equivalent to OnClose/DestroyWindow
    }
}

void Transfer::OnBnClickedCancel()
{
    close();
}

void Transfer::OnBnClickedAttended()
{
    if (Action(ONYX_ACTION_ATTENDED_TRANSFER)) {
        close();
    }
}

void Transfer::OnBnClickedBlind()
{
    if (Action(ONYX_ACTION_TRANSFER)) {
        close();
    }
}

void Transfer::OnBnClickedKey1()
{
    Input(QStringLiteral("1"));
}

void Transfer::OnBnClickedKey2()
{
    Input(QStringLiteral("2"));
}

void Transfer::OnBnClickedKey3()
{
    Input(QStringLiteral("3"));
}

void Transfer::OnBnClickedKey4()
{
    Input(QStringLiteral("4"));
}

void Transfer::OnBnClickedKey5()
{
    Input(QStringLiteral("5"));
}

void Transfer::OnBnClickedKey6()
{
    Input(QStringLiteral("6"));
}

void Transfer::OnBnClickedKey7()
{
    Input(QStringLiteral("7"));
}

void Transfer::OnBnClickedKey8()
{
    Input(QStringLiteral("8"));
}

void Transfer::OnBnClickedKey9()
{
    Input(QStringLiteral("9"));
}

void Transfer::OnBnClickedKeyStar()
{
    Input(QStringLiteral("*"));
}

void Transfer::OnBnClickedKey0()
{
    Input(QStringLiteral("0"));
}

void Transfer::OnBnClickedKeyGrate()
{
    Input(QStringLiteral("#"));
}
