// AAOptionsDlg.cpp
#include "AAOptionsDlg.h"
#include "mainDlg.h"
#include "SettingsDlg.h"
#include "langpack.h"
#include "settings.h"

#include <QGridLayout>
#include <QLabel>
#include <QCursor>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QShowEvent>
#include <QStringList>

static const QStringList callsItems = { QString(), QStringLiteral("hold"), QStringLiteral("all") };
static const QStringList callsValues = { QStringLiteral("No"), QStringLiteral("Hold"), QStringLiteral("All Calls") };

AAOptionsDlg::AAOptionsDlg(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    if (langPack.rtl)
        setLayoutDirection(Qt::RightToLeft);
    setupUi();
    loadSettings();
}

AAOptionsDlg::~AAOptionsDlg()
{
    if (mainDlg && mainDlg->settingsDlg)
        mainDlg->settingsDlg->aaOptionsDlg = nullptr;
}

void AAOptionsDlg::setupUi()
{
    setWindowTitle(Translate(_T("Auto Answer Options")));
    auto *mainLayout = new QGridLayout(this);

    m_delayEdit = new QLineEdit(this);
    m_numberEdit = new QLineEdit(this);
    m_callsCombo = new QComboBox(this);

    for (const auto &val : callsValues)
        m_callsCombo->addItem(Translate(val));

    m_okButton = new QPushButton(Translate(_T("OK")), this);
    m_cancelButton = new QPushButton(Translate(_T("Cancel")), this);

    mainLayout->addWidget(new QLabel(Translate(_T("Delay (seconds):")), this), 0, 0);
    mainLayout->addWidget(m_delayEdit, 0, 1);
    mainLayout->addWidget(new QLabel(Translate(_T("Number:")), this), 1, 0);
    mainLayout->addWidget(m_numberEdit, 1, 1);
    mainLayout->addWidget(new QLabel(Translate(_T("Calls:")), this), 2, 0);
    mainLayout->addWidget(m_callsCombo, 2, 1);
    mainLayout->addWidget(m_okButton, 3, 0);
    mainLayout->addWidget(m_cancelButton, 3, 1);

    connect(m_okButton, &QPushButton::clicked, this, &AAOptionsDlg::onAccept);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::close);
}

void AAOptionsDlg::loadSettings()
{
    m_delayEdit->setText(QString::number(accountSettings.autoAnswerDelay));
    m_numberEdit->setText(accountSettings.autoAnswerNumber);

    bool found = false;
    for (int i = 0; i < callsItems.size(); ++i) {
        if (accountSettings.autoAnswerCalls == callsItems.at(i)) {
            m_callsCombo->setCurrentIndex(i);
            found = true;
            break;
        }
    }
    if (!found)
        m_callsCombo->setCurrentIndex(0);
}

void AAOptionsDlg::saveSettings()
{
    accountSettings.autoAnswerDelay = m_delayEdit->text().toInt();
    accountSettings.autoAnswerNumber = m_numberEdit->text().trimmed();
    accountSettings.autoAnswerCalls = callsItems.value(m_callsCombo->currentIndex());
    accountSettings.SettingsSave();
}

void AAOptionsDlg::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    QPoint cursorPos = QCursor::pos();
    move(cursorPos.x() - 250, cursorPos.y() + 10);
}

void AAOptionsDlg::onAccept()
{
    saveSettings();
    accept(); // triggers close if deleteOnClose is set
}

int AAOptionsDlg::autoAnswerDelay() const
{
    return accountSettings.autoAnswerDelay;
}

void AAOptionsDlg::setAutoAnswerDelay(int delay)
{
    if (accountSettings.autoAnswerDelay != delay) {
        accountSettings.autoAnswerDelay = delay;
        m_delayEdit->setText(QString::number(delay));
        emit autoAnswerDelayChanged(delay);
    }
}

QString AAOptionsDlg::autoAnswerNumber() const
{
    return accountSettings.autoAnswerNumber;
}

void AAOptionsDlg::setAutoAnswerNumber(const QString &number)
{
    if (accountSettings.autoAnswerNumber != number) {
        accountSettings.autoAnswerNumber = number;
        m_numberEdit->setText(number);
        emit autoAnswerNumberChanged(number);
    }
}

QString AAOptionsDlg::autoAnswerCalls() const
{
    return accountSettings.autoAnswerCalls;
}

void AAOptionsDlg::setAutoAnswerCalls(const QString &calls)
{
    if (accountSettings.autoAnswerCalls != calls) {
        accountSettings.autoAnswerCalls = calls;
        int idx = callsItems.indexOf(calls);
        if (idx >= 0)
            m_callsCombo->setCurrentIndex(idx);
        emit autoAnswerCallsChanged(calls);
    }
}
