#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSettings>
#include <QMessageBox>
#include <QPainter>

namespace Onyx {

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Welcome to Onyx");
    setFixedSize(440, 480);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    
    setupUi();
    loadSavedSettings();
}

void LoginDialog::setupUi() {
    setStyleSheet(
        "QDialog {"
        "  background-color: #0F172A;"
        "  color: #F8FAFC;"
        "}"
        "QLabel {"
        "  color: #E2E8F0;"
        "  font-size: 13px;"
        "}"
        "QLineEdit, QComboBox {"
        "  background-color: #1E293B;"
        "  color: #F8FAFC;"
        "  border: 1px solid #334155;"
        "  border-radius: 8px;"
        "  padding: 8px 12px;"
        "  font-size: 13px;"
        "}"
        "QLineEdit:focus, QComboBox:focus {"
        "  border: 1px solid #0078D4;"
        "}"
        "QComboBox::drop-down {"
        "  border: none;"
        "  padding-right: 8px;"
        "}"
        "QCheckBox {"
        "  color: #94A3B8;"
        "  font-size: 12px;"
        "}"
        "QPushButton#continueBtn {"
        "  background-color: #0078D4;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 10px 16px;"
        "  font-weight: bold;"
        "  font-size: 14px;"
        "}"
        "QPushButton#continueBtn:hover {"
        "  background-color: #106EBE;"
        "}"
        "QPushButton#continueBtn:pressed {"
        "  background-color: #005A9E;"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 28, 32, 28);
    mainLayout->setSpacing(18);

    // Header with App Title
    QVBoxLayout* headerLayout = new QVBoxLayout();
    QLabel* brandLabel = new QLabel("ONYX", this);
    brandLabel->setAlignment(Qt::AlignCenter);
    brandLabel->setStyleSheet("font-size: 26px; font-weight: 800; color: #0078D4; letter-spacing: 2px;");

    QLabel* subtitleLabel = new QLabel("Secure LAN Voice, Video & Group Conferencing", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 12px; color: #94A3B8;");

    headerLayout->addWidget(brandLabel);
    headerLayout->addWidget(subtitleLabel);
    mainLayout->addLayout(headerLayout);

    // Avatar preview
    m_avatarPreview = new QLabel(this);
    m_avatarPreview->setFixedSize(72, 72);
    m_avatarPreview->setAlignment(Qt::AlignCenter);
    m_avatarPreview->setStyleSheet(
        "background-color: #0078D4; color: white; border-radius: 36px; "
        "font-size: 26px; font-weight: bold; border: 3px solid #38BDF8;"
    );
    m_avatarPreview->setText("O");

    QHBoxLayout* avLayout = new QHBoxLayout();
    avLayout->addStretch();
    avLayout->addWidget(m_avatarPreview);
    avLayout->addStretch();
    mainLayout->addLayout(avLayout);

    // Form fields
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignLeft);

    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("Enter your display name (e.g. Scott)");
    formLayout->addRow("Display Name:", m_usernameEdit);

    m_statusCombo = new QComboBox(this);
    m_statusCombo->addItems({"Available", "In a call", "In a meeting", "Busy", "Working remotely", "Away"});
    formLayout->addRow("Status:", m_statusCombo);

    m_colorCombo = new QComboBox(this);
    m_colorCombo->addItem("Onyx Blue", "#0078D4");
    m_colorCombo->addItem("Emerald Green", "#10B981");
    m_colorCombo->addItem("Amethyst Purple", "#8B5CF6");
    m_colorCombo->addItem("Crimson Red", "#EF4444");
    m_colorCombo->addItem("Amber Orange", "#F59E0B");
    m_colorCombo->addItem("Cyan Teal", "#06B6D4");
    formLayout->addRow("Avatar Color:", m_colorCombo);

    mainLayout->addLayout(formLayout);

    // Remember me check
    m_rememberCheck = new QCheckBox("Remember my profile on this device", this);
    m_rememberCheck->setChecked(true);
    mainLayout->addWidget(m_rememberCheck);

    // Continue button
    m_continueBtn = new QPushButton("Join Onyx LAN", this);
    m_continueBtn->setObjectName("continueBtn");
    mainLayout->addWidget(m_continueBtn);

    connect(m_usernameEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        QString initial = text.trimmed().isEmpty() ? "O" : text.trimmed().left(1).toUpper();
        m_avatarPreview->setText(initial);
    });

    connect(m_colorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoginDialog::onColorSelected);
    connect(m_continueBtn, &QPushButton::clicked, this, &LoginDialog::onAccept);
}

void LoginDialog::onColorSelected(int index) {
    QString color = m_colorCombo->itemData(index).toString();
    m_avatarPreview->setStyleSheet(
        QString("background-color: %1; color: white; border-radius: 36px; "
                "font-size: 26px; font-weight: bold; border: 3px solid #38BDF8;").arg(color)
    );
}

void LoginDialog::loadSavedSettings() {
    QSettings settings("Onyx", "OnyxChat");
    QString savedName = settings.value("username", "").toString();
    QString savedStatus = settings.value("statusMessage", "Available").toString();
    QString savedColor = settings.value("avatarColor", "#0078D4").toString();
    bool remember = settings.value("rememberMe", true).toBool();

    if (!savedName.isEmpty()) {
        m_usernameEdit->setText(savedName);
    }
    int sIdx = m_statusCombo->findText(savedStatus);
    if (sIdx >= 0) m_statusCombo->setCurrentIndex(sIdx);

    int cIdx = m_colorCombo->findData(savedColor);
    if (cIdx >= 0) m_colorCombo->setCurrentIndex(cIdx);

    m_rememberCheck->setChecked(remember);
}

void LoginDialog::saveSettings() {
    QSettings settings("Onyx", "OnyxChat");
    if (m_rememberCheck->isChecked()) {
        settings.setValue("username", username());
        settings.setValue("statusMessage", statusMessage());
        settings.setValue("avatarColor", avatarColor());
        settings.setValue("rememberMe", true);
    } else {
        settings.setValue("rememberMe", false);
    }
}

void LoginDialog::onAccept() {
    if (m_usernameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Display Name Required", "Please enter a display name to join the LAN network.");
        m_usernameEdit->setFocus();
        return;
    }

    saveSettings();
    accept();
}

QString LoginDialog::username() const {
    return m_usernameEdit->text().trimmed();
}

QString LoginDialog::statusMessage() const {
    return m_statusCombo->currentText();
}

QString LoginDialog::avatarColor() const {
    return m_colorCombo->currentData().toString();
}

bool LoginDialog::rememberMe() const {
    return m_rememberCheck->isChecked();
}

} // namespace Onyx
