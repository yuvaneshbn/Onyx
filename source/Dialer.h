#ifndef DIALER_H
#define DIALER_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QSlider>
#include <QToolTip>
#include <QTimer>
#include <QList>
#include <QFont>
#include <QIcon>
#include <QPoint>
#include <QPointF>
#include <QResizeEvent>
#include <QShowEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QSize>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QStyle>
#include <QApplication>
#include "ButtonDialer.h"
#include "LevelsSliderCtrl.h"
#include "ButtonEx.h"
#include "ButtonBottom.h"

// Enum for actions (from original)
enum DialerActions {
    ACTION_CALL,
    ACTION_VIDEO_CALL,
    ACTION_MESSAGE
};

class CBitmapButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CBitmapButton(QWidget *parent = nullptr) : QPushButton(parent) {}
};

// Forward declarations
class MainDlg;
struct Shortcut;

// ------------------------------------------------------------------
// Dialer class – Qt translation of MFC Dialer

class Dialer : public QDialog
{
    Q_OBJECT

public:
    explicit Dialer(QWidget* parent = nullptr);
    ~Dialer() override;

    // Public methods (identical to original)
    void SetName(const QString& str = QString());
    void UpdateVoicemailButton(bool hasMail);
    void DTMF(const QString& digits, bool force = false);
    void DTMFDelayed(const QString& digits, int delay);
    void SetDTMF(const QString& digits);
    void Input(const QString& digits, bool disableDTMF = false);
    void DialedClear();
    void DialedLoad();
    void DialedSave(QComboBox* combobox);
    void DialedAdd(const QString& number);
    void SetNumber(const QString& number, int callsCount = -1);
    void UpdateCallButton(bool force = false, int callsCount = -1);
    void Action(DialerActions action);
    void Clear(bool update = true);
    void TimerVuMeter();

    void SetCheckDND(bool checked);
    void SetCheckREC(bool checked);
    void EnableButtonCONF(bool enabled);

    // Presence / shortcuts
    void RebuildShortcutsRestart();
    void RebuildShortcuts(bool init = false);
    void PresenceSubscribe();
    void PresenceReset();
    void PresenceReceived(const QString& buddyNumber, int image, bool ringing, bool fromUsersDirectory = false);
    void OnTimerShortcutsBlink();
    void RebuildButtons(bool init = false);

    void AutoMove(QWidget* widget, int left, int top, int right, int bottom);
    void AutoUnmove(QWidget* widget);

    // UI members (kept public for compatibility)
    QList<QPushButton*> shortcutButtons;
    QList<Shortcut> shortcuts;    // defined elsewhere

    // Child widgets (the controls)
    QPushButton* m_ButtonDialer1;
    QPushButton* m_ButtonDialer2;
    QPushButton* m_ButtonDialer3;
    QPushButton* m_ButtonDialer4;
    QPushButton* m_ButtonDialer5;
    QPushButton* m_ButtonDialer6;
    QPushButton* m_ButtonDialer7;
    QPushButton* m_ButtonDialer8;
    QPushButton* m_ButtonDialer9;
    QPushButton* m_ButtonDialer0;
    QPushButton* m_ButtonDialerStar;
    QPushButton* m_ButtonDialerGrate;
    QPushButton* m_ButtonDialerDelete;
    QPushButton* m_ButtonDialerPlus;
    QPushButton* m_ButtonDialerClear;
    QPushButton* m_ButtonDialerRedial;

    CLevelsSliderCtrl* m_SliderCtrlInput;
    CLevelsSliderCtrl* m_SliderCtrlOutput;

    QPushButton* m_ButtonMinusInput;
    QPushButton* m_ButtonMinusOutput;
    QPushButton* m_ButtonPlusInput;
    QPushButton* m_ButtonPlusOutput;

    CButtonBottom* m_ButtonDND;
    CButtonBottom* m_ButtonFWD;
    CButtonBottom* m_ButtonAA;
    CButtonBottom* m_ButtonAC;
    CButtonBottom* m_ButtonRec;
    CButtonBottom* m_ButtonConf;

    CButtonEx* m_ButtonCall;
    CButtonEx* m_ButtonEnd;

    CBitmapButton* m_ButtonVoicemail;
    CBitmapButton* m_ButtonVoicemailDisabled;

    QComboBox* m_comboNumber;   // was CComboBox IDC_NUMBER

    // Icons
    QIcon m_iconMuteOutput;
    QIcon m_iconMutedOutput;
    QIcon m_iconMuteInput;
    QIcon m_iconMutedInput;
    QIcon m_iconHold;
    QIcon m_iconResume;
    QIcon m_iconVideo;
    QIcon m_iconTransfer;
    QIcon m_iconMessage;

    bool m_isButtonVoicemailVisible;
    bool m_hasVoicemail;

protected:
    // Overrides
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    bool event(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
    // Slots for UI events (mapped from message map)
    void onOkClicked();
    void onCancelClicked();
    void onDNDClicked();
    void onFWDClicked();
    void onAAClicked();
    void onACClicked();
    void onConfClicked();
    void onRecClicked();
    void onVoicemailClicked();
    void onCallClicked();
    void onVideoCallClicked();
    void onDTMFClicked();
    void onMessageClicked();
    void onHoldClicked();
    void onTransferClicked();
    void onEndClicked();
    void onComboEditTextChanged(const QString& text);
    void onComboIndexChanged(int index);
    void onPlusInputClicked();
    void onMinusInputClicked();
    void onPlusOutputClicked();
    void onMinusOutputClicked();
    void onMuteOutputClicked();
    void onMuteInputClicked();
    void onKey1Clicked();
    void onKey2Clicked();
    void onKey3Clicked();
    void onKey4Clicked();
    void onKey5Clicked();
    void onKey6Clicked();
    void onKey7Clicked();
    void onKey8Clicked();
    void onKey9Clicked();
    void onKeyStarClicked();
    void onKey0Clicked();
    void onKeyGrateClicked();
    void onKeyPlusClicked();
    void onClearClicked();
    void onRedialClicked();
    void onDeleteClicked();
    void OnBnClickedRedial() { onRedialClicked(); }
    void OnBnClickedMuteInput() { onMuteInputClicked(); }
    void OnBnClickedMuteOutput() { onMuteOutputClicked(); }
    void OnBnClickedPlusInput() { onPlusInputClicked(); }
    void OnBnClickedMinusInput() { onMinusInputClicked(); }
    void OnBnClickedPlusOutput() { onPlusOutputClicked(); }
    void OnBnClickedMinusOutput() { onMinusOutputClicked(); }

    void onSliderOutputMoved(int value);
    void onSliderInputMoved(int value);

    void onShortcutClicked();

    void onTimerBlink();   // for shortcuts blinking
    void OnBnClickedCall() { onCallClicked(); }
    void OnTimerVUMeter() { TimerVuMeter(); }

private:
    // Private helpers
    void setupUi();
    void initDialog();
    void createButtons();
    void updateButtonStates();
    void applyAutoMove();
    void storeWindowSize();
    void restoreWindowSize();

    // Data
    QSize windowSize;
    bool delayedDTMF;
    bool muteOutput;
    bool muteInput;

    // Fonts
    QFont m_font;
    QFont m_font_number;
    QFont m_font_balance;
    QFont m_font_call;
    QFont m_font_shortcuts;

    // Timer
    QTimer* blinkTimer;

    // Cursor
    QCursor m_cursorHand;

    // AutoMove data: map widget -> (left, top, right, bottom percentages)
    struct AutoMoveData {
        QWidget* widget;
        int left;   // percentage of parent width
        int top;    // percentage of parent height
        int right;  // percentage of parent width
        int bottom; // percentage of parent height
    };
    QList<AutoMoveData> m_autoMoveList;

    // Helper to set font on many controls
    void setControlsFont(const QFont& font);

public:
    void MuteOutput(bool state = true);
    void MuteInput(bool state = true);

    // Original methods (kept as member functions)
};

#endif // DIALER_H
