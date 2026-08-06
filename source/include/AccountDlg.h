// AccountDlg.h
#pragma once

#include <QDialog>
#include <QPoint>
#include <QString>
#include "settings.h"

class QLineEdit;
class QComboBox;
class QCheckBox;
class QLabel;
class QPushButton;
class QShowEvent;
class QMouseEvent;

class AccountDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AccountDlg(QWidget *parent = nullptr);
    ~AccountDlg() override;

    void Load(int id);
    int accountId() const;
    void setAccountId(int id);

signals:
    void accountIdChanged(int id);

protected:
    void showEvent(QShowEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onAccept();
    void onDeleteAccountClicked();
    void onDisplayPasswordClicked();
    void onPasswordChanged(const QString &text);
    void onHelpLinkActivated(const QString &link);

private:
    void setupUi();
    void populatePublicAddresses();
    void loadAccountData();
    void saveAccountData();

    QLineEdit *m_labelEdit = nullptr;
    QLineEdit *m_serverEdit = nullptr;
    QLineEdit *m_proxyEdit = nullptr;
    QLineEdit *m_domainEdit = nullptr;
    QLineEdit *m_authIDEdit = nullptr;
    QLineEdit *m_usernameEdit = nullptr;
    QLineEdit *m_passwordEdit = nullptr;
    QLineEdit *m_displayNameEdit = nullptr;
    QLineEdit *m_dialingPrefixEdit = nullptr;
    QLineEdit *m_dialPlanEdit = nullptr;
    QCheckBox *m_hideCIDCheck = nullptr;
    QLineEdit *m_voicemailEdit = nullptr;
    QComboBox *m_srtpCombo = nullptr;
    QComboBox *m_transportCombo = nullptr;
    QComboBox *m_publicAddrCombo = nullptr;
    QLineEdit *m_registerRefreshEdit = nullptr;
    QLineEdit *m_keepAliveEdit = nullptr;
    QCheckBox *m_publishCheck = nullptr;
    QCheckBox *m_rewriteCheck = nullptr;
    QCheckBox *m_iceCheck = nullptr;
    QCheckBox *m_sessionTimerCheck = nullptr;
    QLabel *m_displayPasswordLink = nullptr;
    QLabel *m_deleteAccountLink = nullptr;
    QLabel *m_sipServerHelp = nullptr;
    QLabel *m_sipProxyHelp = nullptr;
    QLabel *m_domainHelp = nullptr;
    QLabel *m_usernameHelp = nullptr;
    QLabel *m_authIDHelp = nullptr;
    QLabel *m_passwordHelp = nullptr;
    QLabel *m_nameHelp = nullptr;
    QLabel *m_dialingPrefixHelp = nullptr;
    QLabel *m_dialPlanHelp = nullptr;
    QLabel *m_hideCIDHelp = nullptr;
    QLabel *m_voicemailHelp = nullptr;
    QLabel *m_encryptionHelp = nullptr;
    QLabel *m_transportHelp = nullptr;
    QLabel *m_publicAddressHelp = nullptr;
    QLabel *m_publishPresenceHelp = nullptr;
    QLabel *m_rewriteHelp = nullptr;
    QLabel *m_iceHelp = nullptr;
    QLabel *m_sessionTimerHelp = nullptr;
    QPushButton *m_okButton = nullptr;
    QPushButton *m_cancelButton = nullptr;
    QPoint m_dragPosition;

    Account m_Account;
    int m_accountId = -1;
};
