// AAOptionsDlg.h
#pragma once

#include <QDialog>
#include <QString>

class QLineEdit;
class QComboBox;
class QPushButton;
class QShowEvent;

class AAOptionsDlg : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(int autoAnswerDelay READ autoAnswerDelay WRITE setAutoAnswerDelay NOTIFY autoAnswerDelayChanged)
    Q_PROPERTY(QString autoAnswerNumber READ autoAnswerNumber WRITE setAutoAnswerNumber NOTIFY autoAnswerNumberChanged)
    Q_PROPERTY(QString autoAnswerCalls READ autoAnswerCalls WRITE setAutoAnswerCalls NOTIFY autoAnswerCallsChanged)

public:
    // Corresponds to the internal values "" (No), "hold" (Hold), "all" (All Calls)
    enum CallsOption {
        NoCalls = 0,
        HoldCalls,
        AllCalls
    };
    Q_ENUM(CallsOption)
    explicit AAOptionsDlg(QWidget *parent = nullptr);
    ~AAOptionsDlg() override;
    int autoAnswerDelay() const;
    QString autoAnswerNumber() const;
    QString autoAnswerCalls() const;
    // Convenience methods using the enum
    CallsOption autoAnswerCallsOption() const;
    void setAutoAnswerCallsOption(CallsOption option);

public slots:
    void setAutoAnswerDelay(int delay);
    void setAutoAnswerNumber(const QString &number);
    void setAutoAnswerCalls(const QString &calls);

signals:
    void autoAnswerDelayChanged(int delay);
    void autoAnswerNumberChanged(const QString &number);
    void autoAnswerCallsChanged(const QString &calls);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void setupUi();
    void loadSettings();
    void saveSettings();
    void onAccept();
    void updateComboFromCalls(const QString &calls);
    QLineEdit *m_delayEdit = nullptr;
    QLineEdit *m_numberEdit = nullptr;
    QComboBox *m_callsCombo = nullptr;
    QPushButton *m_okButton = nullptr;
    QPushButton *m_cancelButton = nullptr;
    int m_autoAnswerDelay = 0;
    QString m_autoAnswerNumber;
    QString m_autoAnswerCalls;   // internal value: "", "hold", "all"
};
