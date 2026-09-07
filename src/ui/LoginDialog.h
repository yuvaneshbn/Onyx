#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

namespace Onyx {

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);

    QString username() const;
    QString statusMessage() const;
    QString avatarColor() const;
    bool rememberMe() const;

private slots:
    void onColorSelected(int index);
    void onAccept();

private:
    void setupUi();
    void loadSavedSettings();
    void saveSettings();

    QLineEdit* m_usernameEdit = nullptr;
    QComboBox* m_statusCombo = nullptr;
    QComboBox* m_colorCombo = nullptr;
    QCheckBox* m_rememberCheck = nullptr;
    QLabel* m_avatarPreview = nullptr;
    QPushButton* m_continueBtn = nullptr;
};

} // namespace Onyx

#endif // LOGIN_DIALOG_H
