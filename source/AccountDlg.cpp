// AccountDlg.cpp
#include "AccountDlg.h"
#include "mainDlg.h"
#include "langpack.h"

#include <QGridLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QWindow>
#include <QStringList>
#include <QHostAddress>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>

static const QStringList transportItems = { QStringLiteral("udp"), QStringLiteral("tcp"), QString(), QStringLiteral("tls") };
static const QStringList transportValues = { QStringLiteral("UDP"), QStringLiteral("TCP"), QStringLiteral("UDP+TCP"), QStringLiteral("TLS") };

AccountDlg::AccountDlg(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    if (langPack.rtl)
        setLayoutDirection(Qt::RightToLeft);
    setupUi();
}

AccountDlg::~AccountDlg()
{
    if (mainDlg)
        mainDlg->accountDlg = nullptr;
}

void AccountDlg::setupUi()
{
    setWindowTitle(Translate(_T("Account Settings")));
    auto *mainLayout = new QVBoxLayout(this);

    // Account label
    auto *labelRow = new QHBoxLayout;
    m_labelEdit = new QLineEdit(this);
    labelRow->addWidget(new QLabel(Translate(_T("Account name:")), this));
    labelRow->addWidget(m_labelEdit);
    mainLayout->addLayout(labelRow);

    // SIP Server
    m_sipServerHelp = new QLabel(QStringLiteral("<a href=\"sipServer\">%1</a>").arg(Translate(_T("SIP server"))), this);
    m_sipServerHelp->setOpenExternalLinks(false);
    connect(m_sipServerHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_serverEdit = new QLineEdit(this);
    auto *serverRow = new QHBoxLayout;
    serverRow->addWidget(m_sipServerHelp);
    serverRow->addWidget(m_serverEdit);
    mainLayout->addLayout(serverRow);

    // SIP Proxy
    m_sipProxyHelp = new QLabel(QStringLiteral("<a href=\"sipProxy\">%1</a>").arg(Translate(_T("SIP proxy"))), this);
    m_sipProxyHelp->setOpenExternalLinks(false);
    connect(m_sipProxyHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_proxyEdit = new QLineEdit(this);
    auto *proxyRow = new QHBoxLayout;
    proxyRow->addWidget(m_sipProxyHelp);
    proxyRow->addWidget(m_proxyEdit);
    mainLayout->addLayout(proxyRow);

    // Domain
    m_domainHelp = new QLabel(QStringLiteral("<a href=\"domain\">%1</a>").arg(Translate(_T("Domain"))), this);
    m_domainHelp->setOpenExternalLinks(false);
    connect(m_domainHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_domainEdit = new QLineEdit(this);
    auto *domainRow = new QHBoxLayout;
    domainRow->addWidget(m_domainHelp);
    domainRow->addWidget(m_domainEdit);
    mainLayout->addLayout(domainRow);

    // Username
    m_usernameHelp = new QLabel(QStringLiteral("<a href=\"username\">%1</a>").arg(Translate(_T("Username"))), this);
    m_usernameHelp->setOpenExternalLinks(false);
    connect(m_usernameHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_usernameEdit = new QLineEdit(this);
    auto *userRow = new QHBoxLayout;
    userRow->addWidget(m_usernameHelp);
    userRow->addWidget(m_usernameEdit);
    mainLayout->addLayout(userRow);

    // Auth ID
    m_authIDHelp = new QLabel(QStringLiteral("<a href=\"authID\">%1</a>").arg(Translate(_T("Auth. ID"))), this);
    m_authIDHelp->setOpenExternalLinks(false);
    connect(m_authIDHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_authIDEdit = new QLineEdit(this);
    auto *authRow = new QHBoxLayout;
    authRow->addWidget(m_authIDHelp);
    authRow->addWidget(m_authIDEdit);
    mainLayout->addLayout(authRow);

    // Password
    m_passwordHelp = new QLabel(QStringLiteral("<a href=\"password\">%1</a>").arg(Translate(_T("Password"))), this);
    m_passwordHelp->setOpenExternalLinks(false);
    connect(m_passwordHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    connect(m_passwordEdit, &QLineEdit::textChanged, this, &AccountDlg::onPasswordChanged);
    m_displayPasswordLink = new QLabel(QStringLiteral("<a href=\"display\">%1</a>").arg(Translate(_T("display password"))), this);
    m_displayPasswordLink->setOpenExternalLinks(false);
    connect(m_displayPasswordLink, &QLabel::linkActivated, this, &AccountDlg::onDisplayPasswordClicked);
    auto *passRow = new QHBoxLayout;
    passRow->addWidget(m_passwordHelp);
    passRow->addWidget(m_passwordEdit);
    passRow->addWidget(m_displayPasswordLink);
    mainLayout->addLayout(passRow);

    // Display name
    m_nameHelp = new QLabel(QStringLiteral("<a href=\"name\">%1</a>").arg(Translate(_T("Name"))), this);
    m_nameHelp->setOpenExternalLinks(false);
    connect(m_nameHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_displayNameEdit = new QLineEdit(this);
    auto *nameRow = new QHBoxLayout;
    nameRow->addWidget(m_nameHelp);
    nameRow->addWidget(m_displayNameEdit);
    mainLayout->addLayout(nameRow);

    // Dialing prefix
    m_dialingPrefixHelp = new QLabel(QStringLiteral("<a href=\"dialingPrefix\">%1</a>").arg(Translate(_T("Dialing prefix"))), this);
    m_dialingPrefixHelp->setOpenExternalLinks(false);
    connect(m_dialingPrefixHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_dialingPrefixEdit = new QLineEdit(this);
    auto *prefixRow = new QHBoxLayout;
    prefixRow->addWidget(m_dialingPrefixHelp);
    prefixRow->addWidget(m_dialingPrefixEdit);
    mainLayout->addLayout(prefixRow);

    // Dial plan
    m_dialPlanHelp = new QLabel(QStringLiteral("<a href=\"dialPlan\">%1</a>").arg(Translate(_T("Dial plan"))), this);
    m_dialPlanHelp->setOpenExternalLinks(false);
    connect(m_dialPlanHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_dialPlanEdit = new QLineEdit(this);
    auto *planRow = new QHBoxLayout;
    planRow->addWidget(m_dialPlanHelp);
    planRow->addWidget(m_dialPlanEdit);
    mainLayout->addLayout(planRow);

    // Hide CID
    m_hideCIDHelp = new QLabel(QStringLiteral("<a href=\"hideCID\">%1</a>").arg(Translate(_T("Hide Caller ID"))), this);
    m_hideCIDHelp->setOpenExternalLinks(false);
    connect(m_hideCIDHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_hideCIDCheck = new QCheckBox(this);
    auto *hideRow = new QHBoxLayout;
    hideRow->addWidget(m_hideCIDHelp);
    hideRow->addWidget(m_hideCIDCheck);
    mainLayout->addLayout(hideRow);

    // Voicemail
    m_voicemailHelp = new QLabel(QStringLiteral("<a href=\"voicemail\">%1</a>").arg(Translate(_T("Voicemail number"))), this);
    m_voicemailHelp->setOpenExternalLinks(false);
    connect(m_voicemailHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_voicemailEdit = new QLineEdit(this);
    auto *vmRow = new QHBoxLayout;
    vmRow->addWidget(m_voicemailHelp);
    vmRow->addWidget(m_voicemailEdit);
    mainLayout->addLayout(vmRow);

    // Encryption (SRTP)
    m_encryptionHelp = new QLabel(QStringLiteral("<a href=\"encryption\">%1</a>").arg(Translate(_T("Encryption"))), this);
    m_encryptionHelp->setOpenExternalLinks(false);
    connect(m_encryptionHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_srtpCombo = new QComboBox(this);
    m_srtpCombo->addItem(Translate(_T("Disabled")));
    m_srtpCombo->addItem(QStringLiteral("%1 SRTP (RTP/AVP)").arg(Translate(_T("Optional"))));
    m_srtpCombo->addItem(QStringLiteral("%1 SRTP (RTP/SAVP)").arg(Translate(_T("Mandatory"))));
    m_srtpCombo->addItem(QStringLiteral("DTLS-SRTP/SRTP"));
    m_srtpCombo->addItem(QStringLiteral("DTLS-SRTP"));
    auto *srtpRow = new QHBoxLayout;
    srtpRow->addWidget(m_encryptionHelp);
    srtpRow->addWidget(m_srtpCombo);
    mainLayout->addLayout(srtpRow);

    // Transport
    m_transportHelp = new QLabel(QStringLiteral("<a href=\"transport\">%1</a>").arg(Translate(_T("Transport"))), this);
    m_transportHelp->setOpenExternalLinks(false);
    connect(m_transportHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_transportCombo = new QComboBox(this);
    for (const auto &val : transportValues)
        m_transportCombo->addItem(Translate(val));
    m_transportCombo->setCurrentIndex(0);
    auto *transpRow = new QHBoxLayout;
    transpRow->addWidget(m_transportHelp);
    transpRow->addWidget(m_transportCombo);
    mainLayout->addLayout(transpRow);

    // Public address
    m_publicAddressHelp = new QLabel(QStringLiteral("<a href=\"publicAddress\">%1</a>").arg(Translate(_T("Public address"))), this);
    m_publicAddressHelp->setOpenExternalLinks(false);
    connect(m_publicAddressHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_publicAddrCombo = new QComboBox(this);
    m_publicAddrCombo->setEditable(true);
    m_publicAddrCombo->addItem(Translate(_T("Auto")));
    populatePublicAddresses();
    auto *pubRow = new QHBoxLayout;
    pubRow->addWidget(m_publicAddressHelp);
    pubRow->addWidget(m_publicAddrCombo);
    mainLayout->addLayout(pubRow);

    // Register refresh
    auto *refreshRow = new QHBoxLayout;
    refreshRow->addWidget(new QLabel(Translate(_T("Register refresh:")), this));
    m_registerRefreshEdit = new QLineEdit(this);
    refreshRow->addWidget(m_registerRefreshEdit);
    mainLayout->addLayout(refreshRow);

    // Keep alive
    auto *keepAliveRow = new QHBoxLayout;
    keepAliveRow->addWidget(new QLabel(Translate(_T("Keep alive:")), this));
    m_keepAliveEdit = new QLineEdit(this);
    keepAliveRow->addWidget(m_keepAliveEdit);
    mainLayout->addLayout(keepAliveRow);

    // Publish presence
    m_publishPresenceHelp = new QLabel(QStringLiteral("<a href=\"publishPresence\">%1</a>").arg(Translate(_T("Publish presence"))), this);
    m_publishPresenceHelp->setOpenExternalLinks(false);
    connect(m_publishPresenceHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_publishCheck = new QCheckBox(this);
    auto *pubPresRow = new QHBoxLayout;
    pubPresRow->addWidget(m_publishPresenceHelp);
    pubPresRow->addWidget(m_publishCheck);
    mainLayout->addLayout(pubPresRow);

    // Allow rewrite
    m_rewriteHelp = new QLabel(QStringLiteral("<a href=\"allowRewrite\">%1</a>").arg(Translate(_T("Allow rewrite"))), this);
    m_rewriteHelp->setOpenExternalLinks(false);
    connect(m_rewriteHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_rewriteCheck = new QCheckBox(this);
    auto *rewriteRow = new QHBoxLayout;
    rewriteRow->addWidget(m_rewriteHelp);
    rewriteRow->addWidget(m_rewriteCheck);
    mainLayout->addLayout(rewriteRow);

    // ICE
    m_iceHelp = new QLabel(QStringLiteral("<a href=\"ice\">%1</a>").arg(Translate(_T("ICE"))), this);
    m_iceHelp->setOpenExternalLinks(false);
    connect(m_iceHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_iceCheck = new QCheckBox(this);
    auto *iceRow = new QHBoxLayout;
    iceRow->addWidget(m_iceHelp);
    iceRow->addWidget(m_iceCheck);
    mainLayout->addLayout(iceRow);

    // Session timer
    m_sessionTimerHelp = new QLabel(QStringLiteral("<a href=\"sessionTimer\">%1</a>").arg(Translate(_T("Session timers"))), this);
    m_sessionTimerHelp->setOpenExternalLinks(false);
    connect(m_sessionTimerHelp, &QLabel::linkActivated, this, &AccountDlg::onHelpLinkActivated);
    m_sessionTimerCheck = new QCheckBox(this);
    auto *stRow = new QHBoxLayout;
    stRow->addWidget(m_sessionTimerHelp);
    stRow->addWidget(m_sessionTimerCheck);
    mainLayout->addLayout(stRow);

    // Delete account link
    m_deleteAccountLink = new QLabel(QStringLiteral("<a href=\"delete\">%1</a>").arg(Translate(_T("Delete Account"))), this);
    m_deleteAccountLink->setOpenExternalLinks(false);
    connect(m_deleteAccountLink, &QLabel::linkActivated, this, &AccountDlg::onDeleteAccountClicked);
    mainLayout->addWidget(m_deleteAccountLink);

    // OK / Cancel
    auto *buttonLayout = new QHBoxLayout;
    m_okButton = new QPushButton(Translate(_T("OK")), this);
    m_cancelButton = new QPushButton(Translate(_T("Cancel")), this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &AccountDlg::onAccept);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::close);
}

void AccountDlg::populatePublicAddresses()
{
    // Add local IP addresses
    const QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (const auto &addr : addresses) {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol && addr != QHostAddress::LocalHost)
            m_publicAddrCombo->addItem(addr.toString());
    }
}

void AccountDlg::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    if (m_accountId <= 1) {
        m_serverEdit->setEnabled(m_accountId != 0);
        m_publishCheck->setEnabled(m_accountId != 0);
        m_registerRefreshEdit->setEnabled(m_accountId != 0);
        m_keepAliveEdit->setEnabled(m_accountId != 0);
        m_rewriteCheck->setEnabled(m_accountId != 0);
    }
}

void AccountDlg::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
    QDialog::mousePressEvent(event);
}

void AccountDlg::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
    QDialog::mouseMoveEvent(event);
}

void AccountDlg::Load(int id)
{
    bool found = accountSettings.AccountLoad(id, &m_Account);
    m_accountId = found ? id : -1;
    if (m_accountId && accountSettings.accountId == m_accountId && !accountSettings.account.rememberPassword) {
        m_Account.username = accountSettings.account.username;
        m_Account.password = accountSettings.account.password;
        m_Account.rememberPassword = false;
    }
    loadAccountData();
    bool isEdit = (m_accountId > 0 && (!m_Account.username.isEmpty() || m_accountId > 1));
    m_deleteAccountLink->setVisible(isEdit);
}

void AccountDlg::loadAccountData()
{
    m_labelEdit->setText(m_Account.label);
    m_serverEdit->setText(m_Account.server);
    m_proxyEdit->setText(m_Account.proxy);
    m_domainEdit->setText(m_Account.domain);
    m_authIDEdit->setText(m_Account.authID);
    m_usernameEdit->setText(m_Account.username);
    m_passwordEdit->setText(m_Account.password);
    m_displayNameEdit->setText(m_Account.displayName);
    m_dialingPrefixEdit->setText(m_Account.dialingPrefix);
    m_dialPlanEdit->setText(m_Account.dialPlan);
    m_hideCIDCheck->setChecked(m_Account.hideCID);
    m_voicemailEdit->setText(m_Account.voicemailNumber);

    int srtpIdx = 0;
    if (m_Account.srtp == QStringLiteral("optional")) srtpIdx = 1;
    else if (m_Account.srtp == QStringLiteral("mandatory")) srtpIdx = 2;
    else if (m_Account.srtp == QStringLiteral("dtls-sdes")) srtpIdx = 3;
    else if (m_Account.srtp == QStringLiteral("dtls")) srtpIdx = 4;
    m_srtpCombo->setCurrentIndex(srtpIdx);

    int transpIdx = transportItems.indexOf(m_Account.transport);
    if (transpIdx < 0) transpIdx = 0;
    m_transportCombo->setCurrentIndex(transpIdx);

    // Public address combo may be disabled; set text
    if (m_publicAddrCombo->isEnabled()) {
        QString pubAddr = get_public_addr(&m_Account); // assuming function exists
        if (!pubAddr.isEmpty())
            m_publicAddrCombo->setCurrentText(pubAddr);
        else
            m_publicAddrCombo->setCurrentIndex(0); // "Auto"
    }

    m_registerRefreshEdit->setText(QString::number(m_Account.registerRefresh));
    m_keepAliveEdit->setText(QString::number(m_Account.keepAlive));
    m_publishCheck->setChecked(m_Account.publish);
    m_rewriteCheck->setChecked(m_Account.allowRewrite);
    m_iceCheck->setChecked(m_Account.ice);
    m_sessionTimerCheck->setChecked(m_Account.disableSessionTimer);

    if (m_accountId == -1 || m_Account.password.isEmpty()) {
        m_displayPasswordLink->show();
        m_passwordEdit->setEchoMode(QLineEdit::Password);
    } else {
        m_displayPasswordLink->hide();
    }
}

void AccountDlg::saveAccountData()
{
    m_Account.label = m_labelEdit->text().trimmed();
    m_Account.server = m_serverEdit->text().trimmed();
    m_Account.proxy = m_proxyEdit->text().trimmed();
    m_Account.domain = m_domainEdit->text().trimmed();
    m_Account.authID = m_authIDEdit->text().trimmed();
    m_Account.username = m_usernameEdit->text().trimmed();
    m_Account.password = m_passwordEdit->text().trimmed();
    m_Account.displayName = m_displayNameEdit->text().trimmed();
    m_Account.dialingPrefix = m_dialingPrefixEdit->text().trimmed();
    m_Account.dialPlan = m_dialPlanEdit->text().trimmed();
    m_Account.hideCID = m_hideCIDCheck->isChecked();
    m_Account.voicemailNumber = m_voicemailEdit->text().trimmed();

    switch (m_srtpCombo->currentIndex()) {
    case 1: m_Account.srtp = QStringLiteral("optional"); break;
    case 2: m_Account.srtp = QStringLiteral("mandatory"); break;
    case 3: m_Account.srtp = QStringLiteral("dtls-sdes"); break;
    case 4: m_Account.srtp = QStringLiteral("dtls"); break;
    default: m_Account.srtp.clear();
    }

    int tIdx = m_transportCombo->currentIndex();
    m_Account.transport = transportItems.value(tIdx);

    if (m_publicAddrCombo->isEnabled()) {
        QString pub = m_publicAddrCombo->currentText();
        if (pub == Translate(_T("Auto")))
            pub.clear();
        m_Account.publicAddr = pub;
    }

    m_Account.rememberPassword = true;
    m_Account.registerRefresh = m_registerRefreshEdit->text().toInt();
    if (m_Account.registerRefresh <= 0)
        m_Account.registerRefresh = PJSUA_REG_INTERVAL;
    m_Account.keepAlive = m_keepAliveEdit->text().toInt();
    if (m_Account.keepAlive < 0)
        m_Account.keepAlive = 15;

    m_Account.publish = m_publishCheck->isChecked();
    m_Account.allowRewrite = m_rewriteCheck->isChecked();
    m_Account.ice = m_iceCheck->isChecked();
    m_Account.disableSessionTimer = m_sessionTimerCheck->isChecked();
}

void AccountDlg::onAccept()
{
    if (m_Account.domain.isEmpty() || m_Account.username.isEmpty()) {
        if (m_accountId != 0) {
            QMessageBox::information(this, QString(),
                Translate(_T("Please fill out at least the required fields marked with *."))
                + QStringLiteral(" ") + Translate(_T("Ask your SIP provider how to configure the account correctly.")));
            return;
        }
    }

    saveAccountData();
    hide();
    if (mainDlg)
        mainDlg->accountDlg = nullptr;

    // new account assignment
    if (m_accountId == -1) {
        Account dummy;
        int i = 1;
        while (accountSettings.AccountLoad(i, &dummy)) i++;
        m_accountId = i;
    }

    accountSettings.AccountSave(m_accountId, &m_Account);

    if (m_accountId) {
        mainDlg->PJAccountDelete(true);
        accountSettings.accountId = m_accountId;
        accountSettings.account = m_Account;
        accountSettings.AccountLoad(accountSettings.accountId, &accountSettings.account);
        if (!m_Account.rememberPassword) {
            accountSettings.account.username = m_Account.username;
            accountSettings.account.password = m_Account.password;
            accountSettings.account.rememberPassword = false;
        }
        mainDlg->OnAccountChanged();
        mainDlg->InitUI();
        accountSettings.SettingsSave();
        mainDlg->PJAccountAdd();
    } else {
        mainDlg->PJAccountDeleteLocal();
        accountSettings.AccountLoad(0, &accountSettings.accountLocal);
        mainDlg->PJAccountAddLocal();
    }
    close();
}

void AccountDlg::onDisplayPasswordClicked()
{
    m_displayPasswordLink->hide();
    m_passwordEdit->setEchoMode(QLineEdit::Normal);
    m_passwordEdit->setFocus();
    m_passwordEdit->setCursorPosition(m_passwordEdit->text().length());
}

void AccountDlg::onDeleteAccountClicked()
{
    if (m_accountId > 0 &&
        QMessageBox::question(this, QString(), Translate(_T("Are you sure you want to delete?")),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        hide();
        if (mainDlg)
            mainDlg->accountDlg = nullptr;

        // shift accounts
        Account account;
        int i = m_accountId;
        while (accountSettings.AccountLoad(i + 1, &account)) {
            accountSettings.AccountSave(i, &account);
            i++;
        }
        accountSettings.AccountDelete(i);

        if (m_accountId && m_accountId == accountSettings.accountId) {
            mainDlg->PJAccountDelete(true);
            if (i > 1) {
                accountSettings.accountId = 1;
                accountSettings.AccountLoad(accountSettings.accountId, &accountSettings.account);
                mainDlg->OnAccountChanged();
                mainDlg->InitUI();
                mainDlg->PJAccountAdd();
            } else {
                accountSettings.accountId = 0;
                mainDlg->OnAccountChanged();
                mainDlg->InitUI();
            }
            accountSettings.SettingsSave();
        }
        close();
    }
}

void AccountDlg::onPasswordChanged(const QString &text)
{
    if (text.isEmpty()) {
        m_displayPasswordLink->show();
        m_passwordEdit->setEchoMode(QLineEdit::Password);
    }
}

void AccountDlg::onHelpLinkActivated(const QString &link)
{
    OpenHelp(link); // assume global function
}

int AccountDlg::accountId() const
{
    return m_accountId;
}

void AccountDlg::setAccountId(int id)
{
    if (m_accountId != id) {
        m_accountId = id;
        emit accountIdChanged(id);
    }
}
