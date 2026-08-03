#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QShowEvent>

#include "global.h"

class ShortcutsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ShortcutsDlg(QWidget *parent = nullptr);
    ~ShortcutsDlg();

protected:
    // Stores pointers to the widgets of each shortcut row, indexed by shortcut index
    struct ShortcutRow {
        QLineEdit   *labelEdit   = nullptr;
        QLineEdit   *numberEdit  = nullptr;
        QLineEdit   *number2Edit = nullptr;
        QComboBox   *typeCombo   = nullptr;
        QCheckBox   *toggleCheck = nullptr;
        QCheckBox   *presenceCheck = nullptr;
    };
    QVector<ShortcutRow> m_rows;

    QCheckBox *m_enableShortcuts = nullptr;
    QCheckBox *m_shortcutsBottom = nullptr;
    QLabel    *m_number2Label    = nullptr;   // "Number 2" header

    QPushButton *m_okBtn = nullptr;
    QPushButton *m_cancelBtn = nullptr;

    int m_numberWidth  = -1;   // default width of number edit (set after show)
    int m_numberHeight = -1;

    void setupUi();
    void translateUi();
    void initDialog();          // replaces OnInitDialog
    void UpdateToggle(bool checked, int i);
    void showEvent(QShowEvent *event) override;

private slots:
    void onBnClickedToggle(int index, bool checked);
    void onHelpToggle();
    void onHelpBLF();
    void onBnClickedOk();
    void onBnClickedCancel();
    void onClose();
};
