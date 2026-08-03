#include "RinginDlg.h"
#include "langpack.h"
#include "mainDlg.h"
#include "settings.h"
#include "Markup.h"

#include <QApplication>
#include <QScreen>
#include <QWindow>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QStyle>
#include <QStyleOption>
#include <QMessageBox>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <random>

// External globals (as in original)
extern CmainDlg* mainDlg;
extern AccountSettings accountSettings;
extern LangPack langPack;

// Helper function to enumerate monitors (Qt version)
static QList<QScreen*> getScreens() {
    return QGuiApplication::screens();
}

// We'll simulate the MyInfoEnumProc logic: get all non-mirroring screens.
static QList<QScreen*> getNonMirrorScreens() {
    QList<QScreen*> screens;
    for (QScreen *screen : QGuiApplication::screens()) {
        // Qt doesn't provide mirroring flag directly; we assume all screens are valid.
        // To mimic the original's DISPLAY_DEVICE_MIRRORING_DRIVER check, we might skip if the screen geometry is same as primary? 
        // Since we cannot easily detect, we'll just use all screens.
        screens.append(screen);
    }
    return screens;
}

RinginDlg::RinginDlg(QWidget *pParent /*=nullptr*/)
    : QDialog(pParent)
    , call_id(PJSUA_INVALID_ID)
    , answered(false)
    , remoteHasVideo(false)
{
    setObjectName("RinginDlg");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Incoming Call"));

    // Create UI elements
    m_pCallerNameLabel = new QLabel(this);
    m_pCallerNameLabel->setObjectName(QString::number(IDC_CALLER_NAME));
    m_pCallerNameLabel->setAlignment(Qt::AlignCenter);
    m_pCallerAddrLabel = new QLabel(this);
    m_pCallerAddrLabel->setObjectName(QString::number(IDC_CALLER_ADDR));
    m_pCallerAddrLabel->setAlignment(Qt::AlignCenter);
    m_pRinginTextLabel = new QLabel(this);
    m_pRinginTextLabel->setObjectName(QString::number(IDC_RINGIN_TEXT));
    m_pRinginTextLabel->setAlignment(Qt::AlignCenter);

    m_pAnswerBtn = new QPushButton(tr("Answer"), this);
    m_pAnswerBtn->setObjectName(QString::number(IDC_ANSWER));
    m_pDeclineBtn = new QPushButton(tr("Decline"), this);
    m_pDeclineBtn->setObjectName(QString::number(IDC_DECLINE));
    m_pIgnoreBtn = new QPushButton(tr("Ignore"), this);
    m_pIgnoreBtn->setObjectName(QString::number(IDC_IGNORE));
    m_pVideoBtn = new QPushButton(tr("Video"), this);
    m_pVideoBtn->setObjectName(QString::number(IDC_VIDEO));
    m_pTransferBtn = new QPushButton(this);
    m_pTransferBtn->setObjectName(QString::number(IDC_TRANSFER));
    m_pTransferBtn->setIcon(QIcon(":/icons/button_forward.ico")); // place holder

    // Layout: simple vertical box with labels and buttons
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_pCallerNameLabel);
    mainLayout->addWidget(m_pCallerAddrLabel);
    mainLayout->addWidget(m_pRinginTextLabel);
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_pAnswerBtn);
    btnLayout->addWidget(m_pDeclineBtn);
    btnLayout->addWidget(m_pIgnoreBtn);
    btnLayout->addWidget(m_pVideoBtn);
    btnLayout->addWidget(m_pTransferBtn);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    // Connect signals
    connect(m_pAnswerBtn, &QPushButton::clicked, this, &RinginDlg::onAnswerClicked);
    connect(m_pDeclineBtn, &QPushButton::clicked, this, &RinginDlg::onDeclineClicked);
    connect(m_pVideoBtn, &QPushButton::clicked, this, &RinginDlg::onVideoClicked);
    connect(m_pTransferBtn, &QPushButton::clicked, this, &RinginDlg::onTransferClicked);

    // Setup fonts
    QFont defaultFont = this->font();
    // For caller name: bold, size 20 (approximate)
    m_font = defaultFont;
    m_font.setPointSize(20);
    m_font.setBold(true);
    m_pCallerNameLabel->setFont(m_font);

    // For ignore button: smaller font
    m_font_ignore = defaultFont;
    m_font_ignore.setPointSize(10);
    m_pIgnoreBtn->setFont(m_font_ignore);

    // Hide video button if video disabled
#ifdef _GLOBAL_VIDEO
    if (accountSettings.disableVideo) {
        m_pVideoBtn->setVisible(false);
    }
#endif

    // Ignore button initially disabled and possibly hidden
    m_pIgnoreBtn->setEnabled(false);
    if (accountSettings.noIgnoreCall) {
        m_pIgnoreBtn->setVisible(false);
    }

    // Remove border from name and address labels (like ModifyStyleEx)
    m_pCallerNameLabel->setStyleSheet("border: none;");
    m_pCallerAddrLabel->setStyleSheet("border: none;");

    // Set up timer for initialization (simulate OnTimer)
    m_pInitTimer = new QTimer(this);
    m_pInitTimer->setSingleShot(true);
    connect(m_pInitTimer, &QTimer::timeout, this, [this]() {
        // Nothing needed, just to mimic the timer
    });

    // Set initial size
    resize(400, 300);
}

RinginDlg::~RinginDlg()
{
}

void RinginDlg::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    // Equivalent of OnInitDialog and OnCreate
    // RTL handling
    if (langPack.rtl) {
        setLayoutDirection(Qt::RightToLeft);
    }

    // AutoMove logic is ignored; we rely on layout.

    // Set window position
    int x, y;
    if (accountSettings.randomAnswerBox) {
        QRect ringinRect = geometry();
        QList<QScreen*> screens = getNonMirrorScreens();
        if (!screens.isEmpty()) {
            // Shuffle and pick first (like random_shuffle)
            std::shuffle(screens.begin(), screens.end(), std::mt19937{std::random_device{}()});
            QScreen *screen = screens.first();
            QRect screenRect = screen->availableGeometry();
            x = screenRect.left() + (screenRect.width() - ringinRect.width()) * (rand() / (RAND_MAX + 1.0));
            y = screenRect.top() + (screenRect.height() - ringinRect.height()) * (rand() / (RAND_MAX + 1.0));
        } else {
            QRect primaryRect = QGuiApplication::primaryScreen()->availableGeometry();
            x = primaryRect.left() + (primaryRect.width() - ringinRect.width()) * (rand() / (RAND_MAX + 1.0));
            y = primaryRect.top() + (primaryRect.height() - ringinRect.height()) * (rand() / (RAND_MAX + 1.0));
        }
    } else {
        if (mainDlg && !mainDlg->ringinDlgs.isEmpty()) {
            // Get last dialog's position
            RinginDlg *lastDlg = mainDlg->ringinDlgs.last();
            QRect rect = lastDlg->geometry();
            x = rect.left() + 22;
            y = rect.top() + 22;
        } else {
            if (accountSettings.ringinX != 0 || accountSettings.ringinY != 0) {
                QRect screenRect;
                if (accountSettings.multiMonitor) {
                    // Use union of all screens? In original, Onyx::GetScreenRect might get combined.
                    // We'll use primary screen for simplicity.
                    screenRect = QGuiApplication::primaryScreen()->availableGeometry();
                } else {
                    screenRect = QGuiApplication::primaryScreen()->availableGeometry();
                }
                QRect rect = geometry();
                int maxLeft = screenRect.right() - rect.width();
                x = (accountSettings.ringinX > maxLeft) ? maxLeft : (accountSettings.ringinX < screenRect.left() ? screenRect.left() : accountSettings.ringinX);
                int maxTop = screenRect.bottom() - rect.height();
                y = (accountSettings.ringinY > maxTop) ? maxTop : (accountSettings.ringinY < screenRect.top() ? screenRect.top() : accountSettings.ringinY);
            } else {
                QRect ringinRect = geometry();
                QRect primaryRect = QGuiApplication::primaryScreen()->availableGeometry();
                x = (primaryRect.width() - ringinRect.width()) / 2;
                y = (primaryRect.height() - ringinRect.height()) / 2;
            }
        }
    }

    // Set window position and topmost flags
    if (accountSettings.bringToFrontOnIncoming || accountSettings.alwaysOnTop) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    move(x, y);

    // Bring to front if needed
    if (accountSettings.bringToFrontOnIncoming) {
        if (mainDlg && mainDlg->isVisible()) {
            if (mainDlg->isMinimized()) {
                mainDlg->showNormal();
            } else {
                mainDlg->hide();
                mainDlg->showMinimized();
                mainDlg->showNormal();
            }
        }
        showNormal();
        raise();
        activateWindow();
    } else {
        if (mainDlg && mainDlg->isVisible()) {
            showNormal();
        }
    }

    // Start timer (like OnShowWindow)
    m_pInitTimer->start(1000);
}

void RinginDlg::closeEvent(QCloseEvent *event)
{
    if (accountSettings.noIgnoreCall) {
        onDeclineClicked();
    } else {
        Close(false);
    }
    event->accept();
}

void RinginDlg::timerEvent(QTimerEvent *event)
{
    // Not used; we use QTimer.
}

void RinginDlg::onAnswerClicked()
{
    CallAccept(false);
}

void RinginDlg::onVideoClicked()
{
    CallAccept(true);
}

void RinginDlg::onDeclineClicked()
{
    if (!answered) {
        pjsua_call_info call_info;
        pjsua_call_get_info(call_id, &call_info);
        call_user_data *user_data = (call_user_data *)pjsua_call_get_user_data(call_id);
        if (user_data) {
            user_data->CS.Lock();
            user_data->hangup = true;
            user_data->CS.Unlock();
        }
        onyx_call_busy(call_id);
        if (mainDlg) {
            mainDlg->callIdIncomingIgnore = Onyx::PjToStr(&call_info.call_id);
        }
    }
    Close();
}

void RinginDlg::onTransferClicked()
{
    if (mainDlg) {
        mainDlg->OpenTransferDlg(this, ONYX_ACTION_FORWARD, call_id);
    }
}

void RinginDlg::SetCallId(pjsua_call_id new_call_id)
{
    call_id = new_call_id;
}

void RinginDlg::OnAnswer()
{
    answered = true;
#ifdef _GLOBAL_VIDEO
    m_pVideoBtn->setEnabled(false);
#endif
    m_pAnswerBtn->setEnabled(false);
    m_pDeclineBtn->setEnabled(false);
}

void RinginDlg::CallAccept(bool hasVideo)
{
    if (!answered) {
        if (mainDlg) {
            mainDlg->onCallAnswer((WPARAM)call_id, (LPARAM)hasVideo);
        }
    }
}

void RinginDlg::Close(bool accept)
{
    if (mainDlg) {
        int count = mainDlg->ringinDlgs.size();
        for (int i = 0; i < count; ++i) {
            if (call_id == mainDlg->ringinDlgs[i]->call_id) {
                if (!accept) {
                    mainDlg->UpdateWindowText("-");
                }
                if (count == 1) {
                    mainDlg->PlayerStop();
                }
                mainDlg->ringinDlgs.removeAt(i);
                call_id = PJSUA_INVALID_ID;
                break;
            }
        }
    }
    if (call_id == PJSUA_INVALID_ID) {
        deleteLater();
    }
}

void RinginDlg::moveEvent(QMoveEvent *event)
{
    if (isVisible() && !isMaximized() && !isMinimized()) {
        QRect cRect = geometry();
        accountSettings.ringinX = cRect.left();
        accountSettings.ringinY = cRect.top();
        if (mainDlg) {
            mainDlg->AccountSettingsPendingSave();
        }
    }
    QDialog::moveEvent(event);
}
