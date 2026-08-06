// AddDlg.cpp
#include "AddDlg.h"
#include "mainDlg.h"
#include "langpack.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

AddDlg::AddDlg(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    if (langPack.rtl)
        setLayoutDirection(Qt::RightToLeft);
    setupUi();
}

AddDlg::~AddDlg() = default;

void AddDlg::setupUi()
{
    setWindowTitle(Translate(QStringLiteral("Add / Edit Contact")));

    auto *mainLayout = new QVBoxLayout(this);

    auto *formLayout = new QGridLayout;
    int row = 0;

    // Name
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Name:")), this), row, 0);
    m_nameEdit = new QLineEdit(this);
    formLayout->addWidget(m_nameEdit, row++, 1);

    // Number
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Number:")), this), row, 0);
    m_numberEdit = new QLineEdit(this);
    formLayout->addWidget(m_numberEdit, row++, 1);

    // First name
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("First Name:")), this), row, 0);
    m_firstNameEdit = new QLineEdit(this);
    connect(m_firstNameEdit, &QLineEdit::textEdited, this, &AddDlg::onFirstLastNameChanged);
    formLayout->addWidget(m_firstNameEdit, row++, 1);

    // Last name
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Last Name:")), this), row, 0);
    m_lastNameEdit = new QLineEdit(this);
    connect(m_lastNameEdit, &QLineEdit::textEdited, this, &AddDlg::onFirstLastNameChanged);
    formLayout->addWidget(m_lastNameEdit, row++, 1);

    // Phone
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Phone:")), this), row, 0);
    m_phoneEdit = new QLineEdit(this);
    connect(m_phoneEdit, &QLineEdit::textEdited, this, &AddDlg::onPhoneNumberChanged);
    formLayout->addWidget(m_phoneEdit, row++, 1);

    // Mobile
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Mobile:")), this), row, 0);
    m_mobileEdit = new QLineEdit(this);
    formLayout->addWidget(m_mobileEdit, row++, 1);

    // Email
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Email:")), this), row, 0);
    m_emailEdit = new QLineEdit(this);
    formLayout->addWidget(m_emailEdit, row++, 1);

    // Address
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Address:")), this), row, 0);
    m_addressEdit = new QLineEdit(this);
    formLayout->addWidget(m_addressEdit, row++, 1);

    // City
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("City:")), this), row, 0);
    m_cityEdit = new QLineEdit(this);
    formLayout->addWidget(m_cityEdit, row++, 1);

    // State
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("State:")), this), row, 0);
    m_stateEdit = new QLineEdit(this);
    formLayout->addWidget(m_stateEdit, row++, 1);

    // ZIP
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("ZIP:")), this), row, 0);
    m_zipEdit = new QLineEdit(this);
    formLayout->addWidget(m_zipEdit, row++, 1);

    // Comment
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Comment:")), this), row, 0);
    m_commentEdit = new QLineEdit(this);
    formLayout->addWidget(m_commentEdit, row++, 1);

    // Presence
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Presence:")), this), row, 0);
    m_presenceCheck = new QCheckBox(this);
    formLayout->addWidget(m_presenceCheck, row++, 1);

    // Starred
    formLayout->addWidget(new QLabel(Translate(QStringLiteral("Starred:")), this), row, 0);
    m_starredCheck = new QCheckBox(this);
    formLayout->addWidget(m_starredCheck, row++, 1);

    mainLayout->addLayout(formLayout);

    // Buttons
    auto *buttonLayout = new QHBoxLayout;
    m_okButton = new QPushButton(Translate(QStringLiteral("OK")), this);
    m_cancelButton = new QPushButton(Translate(QStringLiteral("Cancel")), this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &AddDlg::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &AddDlg::onCancelClicked);
}

void AddDlg::Load(Contact *pContact)
{
    m_loading = true;

    m_originalNumber = pContact->number;
    m_originalFirstName = pContact->firstname;
    m_originalLastName = pContact->lastname;
    m_originalPhone = pContact->phone;

    m_nameEdit->setText(pContact->name);
    m_numberEdit->setText(pContact->number);
    m_firstNameEdit->setText(pContact->firstname);
    m_lastNameEdit->setText(pContact->lastname);
    m_phoneEdit->setText(pContact->phone);
    m_mobileEdit->setText(pContact->mobile);
    m_emailEdit->setText(pContact->email);
    m_addressEdit->setText(pContact->address);
    m_cityEdit->setText(pContact->city);
    m_stateEdit->setText(pContact->state);
    m_zipEdit->setText(pContact->zip);
    m_commentEdit->setText(pContact->comment);
    m_presenceCheck->setChecked(pContact->presence);
    m_starredCheck->setChecked(pContact->starred);

    m_nameEdit->setFocus();

    m_loading = false;
}

void AddDlg::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    // Any additional show-time logic can be placed here
}

void AddDlg::onOkClicked()
{
    Contact contact;

    contact.name = m_nameEdit->text().trimmed();
    contact.number = m_numberEdit->text().trimmed();
    contact.firstname = m_firstNameEdit->text().trimmed();
    contact.lastname = m_lastNameEdit->text().trimmed();
    contact.phone = m_phoneEdit->text().trimmed();
    contact.mobile = m_mobileEdit->text().trimmed();
    contact.email = m_emailEdit->text().trimmed();
    contact.address = m_addressEdit->text().trimmed();
    contact.city = m_cityEdit->text().trimmed();
    contact.state = m_stateEdit->text().trimmed();
    contact.zip = m_zipEdit->text().trimmed();
    contact.comment = m_commentEdit->text().trimmed();
    contact.presence = m_presenceCheck->isChecked();
    contact.starred = m_starredCheck->isChecked();

    if (contact.number.isEmpty()) {
        m_numberEdit->setFocus();
        return;
    }

    // Check duplicate if number changed
    if (m_originalNumber.isEmpty() || m_originalNumber != contact.number) {
        if (mainDlg->pageContacts->FindContact(contact.number)) {
            if (QMessageBox::question(this,
                    Translate(QStringLiteral("Contact already exists")),
                    Translate(QStringLiteral("Contact with the same number already exists. Do you want to overwrite?")),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No) != QMessageBox::Yes)
                return;
        }
    }

    // List of fields that changed (optional, for partial update)
    QStringList fields;
    fields << QStringLiteral("name") << QStringLiteral("number") << QStringLiteral("firstname")
           << QStringLiteral("lastname") << QStringLiteral("phone") << QStringLiteral("mobile")
           << QStringLiteral("email") << QStringLiteral("address") << QStringLiteral("city")
           << QStringLiteral("state") << QStringLiteral("zip") << QStringLiteral("comment")
           << QStringLiteral("presence") << QStringLiteral("starred");

    if (mainDlg->pageContacts->ContactAdd(contact, TRUE, FALSE, &fields, m_originalNumber, true)) {
        hide();
    }
}

void AddDlg::onCancelClicked()
{
    hide();
}

void AddDlg::onFirstLastNameChanged()
{
    if (m_loading)
        return;

    QString name = m_nameEdit->text().trimmed();
    QString firstName = m_firstNameEdit->text().trimmed();
    QString lastName = m_lastNameEdit->text().trimmed();

    // Logic ported from original: try to split name at first space if old first/last matched beginning
    int pos = -1;
    if (!m_originalFirstName.isEmpty()) {
        if (name.startsWith(m_originalFirstName)) {
            pos = m_originalFirstName.length();
        }
    } else if (!m_originalLastName.isEmpty()) {
        if (name.startsWith(m_originalLastName)) {
            pos = 0;
        }
    }
    if (pos == -1) {
        pos = name.indexOf(' ');
    }

    QString newFirstName, newLastName;
    if (pos != -1) {
        newFirstName = name.left(pos);
        newLastName = name.mid(pos).trimmed();
    } else {
        newFirstName = name;
        newLastName.clear();
    }

    // If the split matches the original parts, update name to reflect new first/last
    if ((newFirstName == m_originalFirstName && newLastName == m_originalLastName) ||
        (newLastName.isEmpty() && newFirstName == m_originalLastName)) {
        QString updatedName = firstName;
        if (!lastName.isEmpty()) {
            updatedName += QStringLiteral(" ") + lastName;
        }
        m_nameEdit->setText(updatedName.trimmed());
    }

    m_originalFirstName = firstName;
    m_originalLastName = lastName;
}

void AddDlg::onPhoneNumberChanged()
{
    if (m_loading)
        return;

    QString number = m_numberEdit->text().trimmed();
    QString phone = m_phoneEdit->text().trimmed();

    if (m_originalPhone == number) {
        m_numberEdit->setText(phone);
    }
    m_originalPhone = phone;
}

QString AddDlg::contactName() const
{
    return m_nameEdit ? m_nameEdit->text().trimmed() : QString();
}

void AddDlg::setContactName(const QString &name)
{
    if (m_nameEdit && m_nameEdit->text().trimmed() != name.trimmed()) {
        m_nameEdit->setText(name.trimmed());
        emit contactNameChanged(name.trimmed());
    }
}

QString AddDlg::contactNumber() const
{
    return m_numberEdit ? m_numberEdit->text().trimmed() : QString();
}

void AddDlg::setContactNumber(const QString &number)
{
    if (m_numberEdit && m_numberEdit->text().trimmed() != number.trimmed()) {
        m_numberEdit->setText(number.trimmed());
        emit contactNumberChanged(number.trimmed());
    }
}