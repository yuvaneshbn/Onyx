#include "ShortcutsDlg.h"
#include "mainDlg.h"
#include "Dialer.h"
#include "settings.h"
#include "langpack.h"

#include <QScrollArea>

// Static arrays for action types (original kept as CString)
static QString defaultActionItems[] = {
    ONYX_SHORTCUT_CALL,
#ifdef _GLOBAL_VIDEO
    ONYX_SHORTCUT_VIDEOCALL,
#endif
    ONYX_SHORTCUT_MESSAGE,
    ONYX_SHORTCUT_DTMF,
    ONYX_SHORTCUT_TRANSFER,
    ONYX_SHORTCUT_ATTENDED_TRANSFER,
    ONYX_SHORTCUT_CONFERENCE,
    ONYX_SHORTCUT_RUNBATCH,
    ONYX_SHORTCUT_CALL_URL,
    ONYX_SHORTCUT_POP_URL,
};
static QString defaultActionValues[] = {
    "Call",
#ifdef _GLOBAL_VIDEO
    "Video Call",
#endif
    "Message",
    "DTMF",
    "Call Transfer",
    "Attended Transfer",
    "Conference",
    "Run Batch",
    "Call URL",
    "Pop URL",
};

ShortcutsDlg::ShortcutsDlg(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi();
    translateUi();
    initDialog();
}

ShortcutsDlg::~ShortcutsDlg() {}

void ShortcutsDlg::setupUi()
{
    setWindowTitle("Shortcuts");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Enable / Bottom checkboxes
    QHBoxLayout *topLayout = new QHBoxLayout;
    m_enableShortcuts = new QCheckBox(Translate("Enable shortcuts"));
    m_shortcutsBottom = new QCheckBox(Translate("Show shortcuts at bottom"));
    topLayout->addWidget(m_enableShortcuts);
    topLayout->addWidget(m_shortcutsBottom);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // Scrollable area for shortcut rows
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget;
    QGridLayout *gridLayout = new QGridLayout(scrollWidget);

    // Column headers
    gridLayout->addWidget(new QLabel(Translate("Label")), 0, 0);
    gridLayout->addWidget(new QLabel(Translate("Number")), 0, 1);
    m_number2Label = new QLabel(Translate("Number 2"));
    gridLayout->addWidget(m_number2Label, 0, 2);
    gridLayout->addWidget(new QLabel(Translate("Type")), 0, 3);
    gridLayout->addWidget(new QLabel(Translate("Toggle")), 0, 4);
    gridLayout->addWidget(new QLabel(Translate("BLF")), 0, 5);

    // Populate rows
    m_rows.resize(_GLOBAL_SHORTCUTS_QTY);
    for (int i = 0; i < _GLOBAL_SHORTCUTS_QTY; ++i) {
        ShortcutRow &row = m_rows[i];
        row.labelEdit   = new QLineEdit;   row.labelEdit->setObjectName(QString("label%1").arg(i));
        row.numberEdit  = new QLineEdit;   row.numberEdit->setObjectName(QString("number%1").arg(i));
        row.number2Edit = new QLineEdit;   row.number2Edit->setObjectName(QString("number2%1").arg(i));
        row.typeCombo   = new QComboBox;   row.typeCombo->setObjectName(QString("type%1").arg(i));
        row.toggleCheck = new QCheckBox;   row.toggleCheck->setObjectName(QString("toggle%1").arg(i));
        row.presenceCheck = new QCheckBox; row.presenceCheck->setObjectName(QString("presence%1").arg(i));

        int rowNum = i + 1;  // grid row 0 = headers
        gridLayout->addWidget(row.labelEdit,   rowNum, 0);
        gridLayout->addWidget(row.numberEdit,  rowNum, 1);
        gridLayout->addWidget(row.number2Edit, rowNum, 2);
        gridLayout->addWidget(row.typeCombo,   rowNum, 3);
        gridLayout->addWidget(row.toggleCheck, rowNum, 4);
        gridLayout->addWidget(row.presenceCheck, rowNum, 5);

        // Fill combo with items
        for (int j = 0; j < sizeof(defaultActionItems)/sizeof(defaultActionItems[0]); ++j) {
            QString str = defaultActionValues[j];
            if (defaultActionItems[j] == "runBatch"
                || defaultActionItems[j] == "callURL"
                || defaultActionItems[j] == "popURL") {
                str += "*";
            }
            row.typeCombo->addItem(Translate(str));
        }

        // Connect toggle button
        connect(row.toggleCheck, &QCheckBox::toggled, this, [this, i](bool checked) {
            onBnClickedToggle(i, checked);
        });
    }

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    // Help links (SyslinkToggle, SyslinkBLF) – can be separate buttons or labels
    QHBoxLayout *helpLayout = new QHBoxLayout;
    QPushButton *helpToggleBtn = new QPushButton(Translate("?"));
    QPushButton *helpBLFBtn = new QPushButton(Translate("?"));
    connect(helpToggleBtn, &QPushButton::clicked, this, &ShortcutsDlg::onHelpToggle);
    connect(helpBLFBtn, &QPushButton::clicked, this, &ShortcutsDlg::onHelpBLF);
    helpLayout->addWidget(new QLabel(Translate("Toggle help:")));
    helpLayout->addWidget(helpToggleBtn);
    helpLayout->addWidget(new QLabel(Translate("BLF help:")));
    helpLayout->addWidget(helpBLFBtn);
    helpLayout->addStretch();
    mainLayout->addLayout(helpLayout);

    // OK / Cancel buttons
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    m_okBtn = new QPushButton(Translate("OK"));
    m_cancelBtn = new QPushButton(Translate("Cancel"));
    connect(m_okBtn, &QPushButton::clicked, this, &ShortcutsDlg::onBnClickedOk);
    connect(m_cancelBtn, &QPushButton::clicked, this, &ShortcutsDlg::onBnClickedCancel);
    btnLayout->addWidget(m_okBtn);
    btnLayout->addWidget(m_cancelBtn);
    mainLayout->addLayout(btnLayout);

    // Initial visibility: hide Number2 column
    m_number2Label->setVisible(false);
    for (auto &row : m_rows) {
        row.number2Edit->setVisible(false);
    }
}

void ShortcutsDlg::translateUi()
{
    setWindowTitle(Translate("Shortcuts"));
    // All texts are already wrapped with Translate where needed
}

void ShortcutsDlg::initDialog()
{
    // Load checkbox states
    m_enableShortcuts->setChecked(accountSettings.enableShortcuts);
    m_shortcutsBottom->setChecked(accountSettings.shortcutsBottom);

    // Populate each row from the global shortcuts array
    for (int i = 0; i < _GLOBAL_SHORTCUTS_QTY; ++i) {
        ShortcutRow &row = m_rows[i];
        Shortcut shortcut;
        if (i < shortcuts.size()) {
            shortcut = shortcuts.at(i);
            row.labelEdit->setText(shortcut.label);
            row.numberEdit->setText(shortcut.number);
            row.number2Edit->setText(shortcut.number2);
        }
        // Set combo selection based on shortcut type
        int n = sizeof(defaultActionItems) / sizeof(defaultActionItems[0]);
        int sel = 0;
        for (int j = 0; j < n; ++j) {
            if (shortcut.type == defaultActionItems[j]) {
                sel = j;
                break;
            }
        }
        row.typeCombo->setCurrentIndex(sel);

        // Toggle and presence
        bool toggleChecked = !shortcut.number2.isEmpty();
        row.toggleCheck->setChecked(toggleChecked);
        row.presenceCheck->setChecked(shortcut.presence);

        if (toggleChecked) {
            UpdateToggle(true, i);
        }
    }
}

void ShortcutsDlg::onBnClickedToggle(int index, bool checked)
{
    UpdateToggle(checked, index);

    // Also show/hide the Number2 column label if any toggle is checked
    bool anyChecked = false;
    for (const auto &row : m_rows) {
        if (row.toggleCheck->isChecked()) {
            anyChecked = true;
            break;
        }
    }
    m_number2Label->setVisible(anyChecked);
}

void ShortcutsDlg::UpdateToggle(bool checked, int i)
{
    ShortcutRow &row = m_rows[i];
    if (checked) {
        // Make number edit half width, show number2
        row.numberEdit->setFixedWidth(m_numberWidth / 2);
        row.number2Edit->setVisible(true);
    } else {
        row.numberEdit->setFixedWidth(m_numberWidth);
        row.number2Edit->setVisible(false);
    }
}

void ShortcutsDlg::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    // Capture the initial size of the first number edit after layout
    if (m_numberWidth <= 0 && !m_rows.isEmpty()) {
        m_numberWidth = m_rows[0].numberEdit->width();
        m_numberHeight = m_rows[0].numberEdit->height();
        // Apply initial sizes based on toggle states
        for (int i = 0; i < m_rows.size(); ++i) {
            if (m_rows[i].toggleCheck->isChecked()) {
                m_rows[i].numberEdit->setFixedWidth(m_numberWidth / 2);
            } else {
                m_rows[i].numberEdit->setFixedWidth(m_numberWidth);
            }
        }
    }
}

void ShortcutsDlg::onClose()
{
    close();
}

void ShortcutsDlg::onBnClickedCancel()
{
    onClose();
}

void ShortcutsDlg::onBnClickedOk()
{
    hide();
    mainDlg->ShortcutsRemoveAll();

    for (int i = 0; i < _GLOBAL_SHORTCUTS_QTY; ++i) {
        ShortcutRow &row = m_rows[i];
        Shortcut shortcut;
        shortcut.label  = row.labelEdit->text();
        shortcut.number = row.numberEdit->text();
        shortcut.number2 = row.number2Edit->text();
        if (!row.toggleCheck->isChecked()) {
            shortcut.number2.clear();
        }
        shortcut.presence = row.presenceCheck->isChecked();

        int comboIdx = row.typeCombo->currentIndex();
        if (comboIdx >= 0 && !shortcut.label.isEmpty() &&
            (!shortcut.number.isEmpty() ||
             defaultActionItems[comboIdx] == ONYX_SHORTCUT_TRANSFER ||
             defaultActionItems[comboIdx] == ONYX_SHORTCUT_ATTENDED_TRANSFER)) {
            shortcut.type = defaultActionItems[comboIdx];
            shortcuts.append(shortcut);
        }
    }
    ShortcutsSave();

    accountSettings.enableShortcuts = m_enableShortcuts->isChecked();
    accountSettings.shortcutsBottom = m_shortcutsBottom->isChecked();

    mainDlg->pageDialer->RebuildShortcutsRestart();

    onClose();
}

void ShortcutsDlg::onHelpToggle()
{
    OpenHelp("toggle");
}

void ShortcutsDlg::onHelpBLF()
{
    OpenHelp("BLF");
}
