// AddDlg.h
#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

// Forward declaration of Contact (assumed defined elsewhere)
struct Contact;
class QShowEvent;

class AddDlg : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString contactName READ contactName WRITE setContactName NOTIFY contactNameChanged)
    Q_PROPERTY(QString contactNumber READ contactNumber WRITE setContactNumber NOTIFY contactNumberChanged)

public:
    explicit AddDlg(QWidget *parent = nullptr);
    ~AddDlg() override;

    void Load(Contact *pContact);

    QString contactName() const;
    void setContactName(const QString &name);

    QString contactNumber() const;
    void setContactNumber(const QString &number);

signals:
    void contactNameChanged(const QString &);
    void contactNumberChanged(const QString &);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onFirstLastNameChanged();
    void onPhoneNumberChanged();

private:
    void setupUi();
    void updateNameFromParts();

    QLineEdit *m_nameEdit = nullptr;
    QLineEdit *m_numberEdit = nullptr;
    QLineEdit *m_firstNameEdit = nullptr;
    QLineEdit *m_lastNameEdit = nullptr;
    QLineEdit *m_phoneEdit = nullptr;
    QLineEdit *m_mobileEdit = nullptr;
    QLineEdit *m_emailEdit = nullptr;
    QLineEdit *m_addressEdit = nullptr;
    QLineEdit *m_cityEdit = nullptr;
    QLineEdit *m_stateEdit = nullptr;
    QLineEdit *m_zipEdit = nullptr;
    QLineEdit *m_commentEdit = nullptr;
    QCheckBox *m_presenceCheck = nullptr;
    QCheckBox *m_starredCheck = nullptr;

    QPushButton *m_okButton = nullptr;
    QPushButton *m_cancelButton = nullptr;

    // Original values used to detect changes for smart update
    QString m_originalNumber;
    QString m_originalFirstName;
    QString m_originalLastName;
    QString m_originalPhone;

    bool m_loading = false;
};
