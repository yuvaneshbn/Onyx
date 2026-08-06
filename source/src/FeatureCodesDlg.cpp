// FeatureCodesDlg.cpp
#include "FeatureCodesDlg.h"
#include "mainDlg.h"          // Qt-adapted mainDlg
#include "SettingsDlg.h"
#include "langpack.h"         // Qt-adapted langpack
#include "settings.h"         // Qt-adapted accountSettings
#include "global.h"           // Qt-adapted global defines

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QApplication>
#include <QCursor>

FeatureCodesDlg::FeatureCodesDlg(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    retranslateUi();

    // Position near cursor
    QPoint cursorPos = QCursor::pos();
    move(cursorPos.x() - 250, cursorPos.y() + 10);

    // RTL support
    if (langPack.rtl) {
        setLayoutDirection(Qt::RightToLeft);
    }

    // Connect buttons
    connect(findChild<QDialogButtonBox*>("buttonBox"), &QDialogButtonBox::accepted,
            this, &FeatureCodesDlg::onOkClicked);
    connect(findChild<QDialogButtonBox*>("buttonBox"), &QDialogButtonBox::rejected,
            this, &FeatureCodesDlg::onCancelClicked);
}

FeatureCodesDlg::~FeatureCodesDlg()
{
}

void FeatureCodesDlg::setupUi()
{
    setWindowTitle(tr("Feature Codes"));
    setObjectName("FeatureCodesDlg");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout;

    // CP
    m_editCP = new QLineEdit(this);
    m_checkCP = new QCheckBox(this);
    formLayout->addRow(tr("Call Pickup:"), m_editCP);
    formLayout->addRow(tr("Enable"), m_checkCP);

    // BT
    m_editBT = new QLineEdit(this);
    m_checkBT = new QCheckBox(this);
    formLayout->addRow(tr("Blind Transfer:"), m_editBT);
    formLayout->addRow(tr("Enable"), m_checkBT);

    // AT
    m_editAT = new QLineEdit(this);
    m_checkAT = new QCheckBox(this);
    formLayout->addRow(tr("Attended Transfer:"), m_editAT);
    formLayout->addRow(tr("Enable"), m_checkAT);

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName("buttonBox");
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void FeatureCodesDlg::retranslateUi()
{
    // This function would be called by a global translation system
    m_editCP->setText(accountSettings.featureCodeCP);
    m_checkCP->setChecked(accountSettings.enableFeatureCodeCP);
    m_editBT->setText(accountSettings.featureCodeBT);
    m_checkBT->setChecked(accountSettings.enableFeatureCodeBT);
    m_editAT->setText(accountSettings.featureCodeAT);
    m_checkAT->setChecked(accountSettings.enableFeatureCodeAT);
}

void FeatureCodesDlg::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    // Translation of dialog title could be done here if needed
}

void FeatureCodesDlg::closeEvent(QCloseEvent *event)
{
    // Notify mainDlg that this dialog is closing
    if (mainDlg && mainDlg->settingsDlg) {
        mainDlg->settingsDlg->featureCodesDlg = nullptr;
    }
    // Use deleteLater to safely clean up
    deleteLater();
    event->accept();
}

void FeatureCodesDlg::onOkClicked()
{
    accountSettings.featureCodeCP = m_editCP->text();
    accountSettings.enableFeatureCodeCP = m_checkCP->isChecked();
    accountSettings.featureCodeBT = m_editBT->text();
    accountSettings.enableFeatureCodeBT = m_checkBT->isChecked();
    accountSettings.featureCodeAT = m_editAT->text();
    accountSettings.enableFeatureCodeAT = m_checkAT->isChecked();
    accountSettings.SettingsSave();
    close();
}

void FeatureCodesDlg::onCancelClicked()
{
    close();
}
