#ifndef MESSAGEBOXX_H
#define MESSAGEBOXX_H

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QTextBrowser>
#include <QTimer>
#include <QList>
#include <QMap>
#include <QIcon>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QCloseEvent>

// Define to enable hyperlink support (like the original NO_CMESSAGEBOXX_HYPERLINK)
// If not defined, hyperlink features are disabled
#define CMESSAGEBOXX_HYPERLINK_SUPPORT

class CMessageBoxX : public QDialog
{
    Q_OBJECT

public:
    // Constructors
    CMessageBoxX(QWidget* parent = nullptr);
    CMessageBoxX(const QString& text, const QString& caption = QString(),
                 int type = 0, int helpId = 0, QWidget* parent = nullptr);
    CMessageBoxX(int resourceId, const QString& caption = QString(),
                 int type = 0, int helpId = -1, QWidget* parent = nullptr);

    // Main method
    int exec() override;

    // Accessors/Mutators
    void setCaption(const QString& caption);
    bool setCaption(int resourceId);
    void setCaption(int resourceId, const QString& arg1);
    void setCaption(int resourceId, const QString& arg1, const QString& arg2);

    void setText(const QString& text, int helpId = 0);
    bool setText(int resourceId, int helpId = -1);
    void setText(int resourceId, const QString& arg1, int helpId = -1);
    void setText(int resourceId, const QString& arg1, const QString& arg2, int helpId = -1);

    void setUserIcon(int resourceId, const QString& iconName = QString()); // icon from resource
    void setUserIcon(const QIcon& icon);

    void setType(int type);
    void setTimeout(int timeout, int timeoutButtonId = QDialogButtonBox::Cancel,
                    bool modifyCaption = true, bool resetOnActivity = false);
    void setDisabledButton(int timeout, int disabledButtonId = QDialogButtonBox::Ok,
                           bool modifyCaption = true);
    void setCheckBox(bool checked, const QString& text);
    void setCheckBoxState(bool checked);
    bool getCheckBoxState() const;

#ifdef CMESSAGEBOXX_HYPERLINK_SUPPORT
    void setHyperlink(bool enable, const QString& caption, const QString& url);
    bool getHyperlink() const;
#endif

    void setRightAlignButtons(bool rightAlign);
    void setSelectableText(bool selectable);
    bool getSelectableText() const;

    void setModifiedButtonCaption(int buttonIndex, const QString& newCaption);
    QString getModifiedButtonCaption(int buttonIndex) const;

    void setAnimatedText(int intervalMs, const QStringList& texts, bool loop);
    void setAnimatedCaption(int intervalMs, const QStringList& captions, bool loop);

signals:
    // Emitted when the dialog is about to close due to timeout
    void timeoutTriggered(int buttonId);

protected:
    // Virtual methods for customization (like original)
    virtual QString getCurrentTimerCaption();
    virtual QString getCurrentDisabledCaption();
    virtual bool installTimeoutDisabledTimer();
    virtual bool installAnimatedTextTimer();
    virtual bool installAnimatedCaptionTimer();
    virtual void doTimeoutDisabledButtonUpdate();
    virtual void doAnimatedTextUpdate();
    virtual void doAnimatedCaptionUpdate();
    virtual bool createCheckBox();
#ifdef CMESSAGEBOXX_HYPERLINK_SUPPORT
    virtual bool createHyperlink();
#endif
    virtual void rightAlignButtons();
    virtual void modifyButtonCaptions();
    virtual void onInitDone(); // analogous to OnInitDone
    bool eventFilter(QObject* obj, QEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onTimeoutTimer();
    void onAnimatedTextTimer();
    void onAnimatedCaptionTimer();
    void onUserActivity(); // for resetting timeout

private:
    // Helper methods
    QWidget* findTextControl() const;
    QWidget* findOurTextControl() const;
    QPushButton* getFirstButton() const;
    QRect getRightMostButtonRect() const;
    void updateCaption(const QString& newCaption);
    void setupLayout();

    // Member variables
    QString m_sCaption;
    QString m_sText;
    int m_nHelpIDPrompt = 0;
    QIcon m_userIcon;
    bool m_hasUserIcon = false;
    int m_nType = 0;
    int m_nTimerTimeout = 0;
    int m_nTimerButtonId = QDialogButtonBox::Cancel;
    int m_nTimerCountdown = 0;
    bool m_bModifyCaptionForTimerCountdown = true;
    bool m_bResetTimeoutOnUserActivity = false;
    int m_nDisabledTimeout = 0;
    int m_nDisabledButtonId = QDialogButtonBox::Ok;
    bool m_bModifyCaptionForDisabledCountdown = true;
    int m_nDisabledCountdown = 0;
    QTimer* m_pTimeoutTimer = nullptr;
    QTimer* m_pAnimatedTextTimer = nullptr;
    QTimer* m_pAnimatedCaptionTimer = nullptr;
    int m_dwTextAnimationInterval = 0;
    QStringList m_animatedTexts;
    bool m_bAnimatedTextLoop = false;
    int m_nCurrentAnimatedTextIndex = 0;
    int m_dwCaptionAnimationInterval = 0;
    QStringList m_animatedCaptions;
    bool m_bAnimatedCaptionLoop = false;
    int m_nCurrentAnimatedCaptionIndex = 0;
    bool m_bCheckBox = false;
    bool m_bCheckBoxState = false;
    QString m_sCheckBoxText;
    QCheckBox* m_pCheckBox = nullptr;
#ifdef CMESSAGEBOXX_HYPERLINK_SUPPORT
    bool m_bHyperlink = false;
    QString m_sHyperlinkCaption;
    QString m_sHyperlinkURL;
    QTextBrowser* m_pHyperlink = nullptr;
#endif
    bool m_bRightAlignButtons = false;
    bool m_bSelectableText = false;
    QTextEdit* m_pSelectableText = nullptr;
    QMap<int, QString> m_modifiedButtonCaptions; // index -> new caption

    // Main content widgets
    QLabel* m_pIconLabel = nullptr;
    QLabel* m_pTextLabel = nullptr;        // normal text
    QWidget* m_pTextWidget = nullptr;      // points to either m_pTextLabel or m_pSelectableText
    QDialogButtonBox* m_pButtonBox = nullptr;
    QVBoxLayout* m_pMainLayout = nullptr;

    // Constants (matching original IDs)
    enum { MESSAGEBOXX_EDIT_ID = 101, MESSAGEBOXX_CHECKBOX_ID = 102, MESSAGEBOXX_HYPERLINK_ID = 103 };
};

#endif // MESSAGEBOXX_H
