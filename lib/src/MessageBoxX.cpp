/*
Module : MessageBoxX.cpp (Qt port)
Purpose: Qt implementation of a MessageBox wrapper class, ported from MFC.
Original MFC version by PJ Naughter.
Qt port: all MFC dependencies replaced with Qt equivalents.

Copyright (c) 2003-2020 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)
All rights reserved.
*/

#include "MessageBoxX.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QTextEdit>
#include <QTextBrowser>
#include <QIcon>
#include <QApplication>
#include <QTimer>
#include <QWindow>
#include <QMessageBox>
#include <QDebug>
#include <QStyle>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QAbstractButton>
#include <QList>
#include <QMap>

// Constants (matching original defines)
constexpr int MB_TYPEMASK = 0x0000000F;
constexpr int MB_ICONMASK = 0x000000F0;
constexpr int MB_OK = 0x00000000;
constexpr int MB_OKCANCEL = 0x00000001;
constexpr int MB_ABORTRETRYIGNORE = 0x00000002;
constexpr int MB_YESNOCANCEL = 0x00000003;
constexpr int MB_YESNO = 0x00000004;
constexpr int MB_RETRYCANCEL = 0x00000005;
constexpr int MB_CANCELTRYCONTINUE = 0x00000006;
constexpr int MB_ICONHAND = 0x00000010;
constexpr int MB_ICONQUESTION = 0x00000020;
constexpr int MB_ICONEXCLAMATION = 0x00000030;
constexpr int MB_ICONASTERISK = 0x00000040;
constexpr int MB_USERICON = 0x00000080;

// ----------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------
CMessageBoxX::CMessageBoxX(QWidget* parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    // Create main layout
    m_pMainLayout = new QVBoxLayout(this);

    // Top area: icon + text (or selectable text)
    QHBoxLayout* topLayout = new QHBoxLayout();
    m_pIconLabel = new QLabel(this);
    m_pIconLabel->setFixedSize(48, 48);
    m_pIconLabel->setScaledContents(true);
    m_pIconLabel->hide();
    topLayout->addWidget(m_pIconLabel);

    // Text label (normal or selectable)
    m_pTextLabel = new QLabel(this);
    m_pTextLabel->setWordWrap(true);
    m_pTextLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    topLayout->addWidget(m_pTextLabel);
    m_pTextWidget = m_pTextLabel;

    m_pMainLayout->addLayout(topLayout);

    // Checkbox (hidden initially)
    m_pCheckBox = new QCheckBox(this);
    m_pCheckBox->hide();
    m_pMainLayout->addWidget(m_pCheckBox);

#ifdef CMESSAGEBOXX_HYPERLINK_SUPPORT
    // Hyperlink (hidden initially)
    m_pHyperlink = new QTextBrowser(this);
    m_pHyperlink->setOpenExternalLinks(true);
    m_pHyperlink->setMaximumHeight(30);
    m_pHyperlink->hide();
    m_pMainLayout->addWidget(m_pHyperlink);
#endif

    // Button box
    m_pButtonBox = new QDialogButtonBox(this);
    m_pMainLayout->addWidget(m_pButtonBox);

    connect(m_pButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_pButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Install event filter for user activity detection
    installEventFilter(this);
}

CMessageBoxX::CMessageBoxX(const QString& text, const QString& caption,
                           int type, int helpId, QWidget* parent)
    : CMessageBoxX(parent)
{
    setText(text, helpId);
    setCaption(caption);
    setType(type);
}

CMessageBoxX::CMessageBoxX(int resourceId, const QString& caption,
                           int type, int helpId, QWidget* parent)
    : CMessageBoxX(parent)
{
    setText(resourceId, helpId);
    setCaption(caption);
    setType(type);
}

// ----------------------------------------------------------------------
// Public methods
// ----------------------------------------------------------------------
void CMessageBoxX::setCaption(const QString& caption)
{
    if (!caption.isEmpty())
        m_sCaption = caption;
    else
        m_sCaption = QApplication::applicationName();
}

bool CMessageBoxX::setCaption(int resourceId)
{
    // In Qt, we don't have LoadString; this is a stub.
    // You can implement a resource map or use QApplication::translate.
    qDebug() << "CMessageBoxX::setCaption(int) not implemented in Qt port";
    return false;
}

void CMessageBoxX::setCaption(int resourceId, const QString& arg1)
{
    setCaption(arg1);
}

void CMessageBoxX::setCaption(int resourceId, const QString& arg1, const QString& arg2)
{
    setCaption(arg1 + " " + arg2);
}

void CMessageBoxX::setText(const QString& text, int helpId)
{
    m_sText = text;
    m_nHelpIDPrompt = helpId;
}

bool CMessageBoxX::setText(int resourceId, int helpId)
{
    qDebug() << "CMessageBoxX::setText(int) not implemented in Qt port";
    return false;
}

void CMessageBoxX::setText(int resourceId, const QString& arg1, int helpId)
{
    setText(arg1, helpId);
}

void CMessageBoxX::setText(int resourceId, const QString& arg1, const QString& arg2, int helpId)
{
    setText(arg1 + " " + arg2, helpId);
}

void CMessageBoxX::setUserIcon(int resourceId, const QString& iconName)
{
    QIcon icon;
    if (!iconName.isEmpty())
        icon = QIcon(iconName);
    else
        icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
    setUserIcon(icon);
}

void CMessageBoxX::setUserIcon(const QIcon& icon)
{
    m_userIcon = icon;
    m_hasUserIcon = true;
    m_pIconLabel->setPixmap(icon.pixmap(48, 48));
    m_pIconLabel->show();
}

void CMessageBoxX::setType(int type)
{
    m_nType = type;

    // Set icon based on type (if no user icon)
    if (!m_hasUserIcon)
    {
        QStyle::StandardPixmap sp = QStyle::SP_MessageBoxInformation;
        int iconPart = type & MB_ICONMASK;
        switch (iconPart)
        {
        case MB_ICONHAND:        sp = QStyle::SP_MessageBoxCritical; break;
        case MB_ICONQUESTION:    sp = QStyle::SP_MessageBoxQuestion; break;
        case MB_ICONEXCLAMATION: sp = QStyle::SP_MessageBoxWarning; break;
        case MB_ICONASTERISK:    sp = QStyle::SP_MessageBoxInformation; break;
        default: break;
        }
        QIcon icon = QApplication::style()->standardIcon(sp);
        if (!icon.isNull())
            setUserIcon(icon);
    }

    // Determine buttons based on type
    QDialogButtonBox::StandardButtons buttons = QDialogButtonBox::NoButton;
    int typeBase = type & MB_TYPEMASK;
    switch (typeBase)
    {
    case MB_OK:                buttons = QDialogButtonBox::Ok; break;
    case MB_OKCANCEL:          buttons = QDialogButtonBox::Ok | QDialogButtonBox::Cancel; break;
    case MB_ABORTRETRYIGNORE:  buttons = QDialogButtonBox::Abort | QDialogButtonBox::Retry | QDialogButtonBox::Ignore; break;
    case MB_YESNOCANCEL:       buttons = QDialogButtonBox::Yes | QDialogButtonBox::No | QDialogButtonBox::Cancel; break;
    case MB_YESNO:             buttons = QDialogButtonBox::Yes | QDialogButtonBox::No; break;
    case MB_RETRYCANCEL:       buttons = QDialogButtonBox::Retry | QDialogButtonBox::Cancel; break;
    case MB_CANCELTRYCONTINUE: buttons = QDialogButtonBox::Cancel | QDialogButtonBox::Retry | QDialogButtonBox::Ignore; break;
    default: break;
    }
    m_pButtonBox->setStandardButtons(buttons);
}

void CMessageBoxX::setTimeout(int timeout, int timeoutButtonId,
                              bool modifyCaption, bool resetOnActivity)
{
    m_nTimerTimeout = timeout;
    m_nTimerButtonId = timeoutButtonId;
    m_nTimerCountdown = timeout;
    m_bModifyCaptionForTimerCountdown = modifyCaption;
    m_bResetTimeoutOnUserActivity = resetOnActivity;
}

void CMessageBoxX::setDisabledButton(int timeout, int disabledButtonId, bool modifyCaption)
{
    m_nDisabledTimeout = timeout;
    m_nDisabledButtonId = disabledButtonId;
    m_nDisabledCountdown = timeout;
    m_bModifyCaptionForDisabledCountdown = modifyCaption;
}

void CMessageBoxX::setCheckBox(bool checked, const QString& text)
{
    m_bCheckBox = true;
    m_bCheckBoxState = checked;
    m_sCheckBoxText = text;
}

void CMessageBoxX::setCheckBoxState(bool checked)
{
    m_bCheckBoxState = checked;
}

bool CMessageBoxX::getCheckBoxState() const
{
    if (m_pCheckBox)
        return m_pCheckBox->isChecked();
    return m_bCheckBoxState;
}

#ifdef CMESSAGEBOXX_HYPERLINK_SUPPORT
void CMessageBoxX::setHyperlink(bool enable, const QString& caption, const QString& url)
{
    m_bHyperlink = enable;
    m_sHyperlinkCaption = caption;
    m_sHyperlinkURL = url;
}

bool CMessageBoxX::getHyperlink() const
{
    return m_bHyperlink;
}
#endif

void CMessageBoxX::setRightAlignButtons(bool rightAlign)
{
    m_bRightAlignButtons = rightAlign;
}

void CMessageBoxX::setSelectableText(bool selectable)
{
    m_bSelectableText = selectable;
}

bool CMessageBoxX::getSelectableText() const
{
    return m_bSelectableText;
}

void CMessageBoxX::setModifiedButtonCaption(int buttonIndex, const QString& newCaption)
{
    m_modifiedButtonCaptions[buttonIndex] = newCaption;
}

QString CMessageBoxX::getModifiedButtonCaption(int buttonIndex) const
{
    return m_modifiedButtonCaptions.value(buttonIndex);
}

void CMessageBoxX::setAnimatedText(int intervalMs, const QStringList& texts, bool loop)
{
    m_dwTextAnimationInterval = intervalMs;
    m_animatedTexts = texts;
    m_bAnimatedTextLoop = loop;
    m_nCurrentAnimatedTextIndex = 0;
    if (!texts.isEmpty())
        setText(texts.first());
}

void CMessageBoxX::setAnimatedCaption(int intervalMs, const QStringList& captions, bool loop)
{
    m_dwCaptionAnimationInterval = intervalMs;
    m_animatedCaptions = captions;
    m_bAnimatedCaptionLoop = loop;
    m_nCurrentAnimatedCaptionIndex = 0;
    if (!captions.isEmpty())
        setCaption(captions.first());
}

// ----------------------------------------------------------------------
// exec() - main entry
// ----------------------------------------------------------------------
int CMessageBoxX::exec()
{
    // If selectable text, replace label with a QTextEdit
    if (m_bSelectableText)
    {
        // Remove m_pTextLabel from layout, create m_pSelectableText
        QLayout* topLayout = m_pMainLayout->itemAt(0)->layout(); // top HBox
        if (topLayout)
        {
            topLayout->removeWidget(m_pTextLabel);
            delete m_pTextLabel;
            m_pSelectableText = new QTextEdit(this);
            m_pSelectableText->setReadOnly(true);
            m_pSelectableText->setPlainText(m_sText);
            topLayout->addWidget(m_pSelectableText);
            m_pTextWidget = m_pSelectableText;
        }
    }
    else
    {
        // Ensure label is used
        if (!m_pTextLabel)
        {
            // Recreate label if needed
            m_pTextLabel = new QLabel(this);
            m_pTextLabel->setWordWrap(true);
            m_pTextLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
            QLayout* topLayout = m_pMainLayout->itemAt(0)->layout();
            if (topLayout)
                topLayout->addWidget(m_pTextLabel);
            m_pTextWidget = m_pTextLabel;
        }
        m_pTextLabel->setText(m_sText);
    }

    // Create checkbox if needed
    if (m_bCheckBox)
        createCheckBox();

#ifdef CMESSAGEBOXX_HYPERLINK_SUPPORT
    if (m_bHyperlink)
        createHyperlink();
#endif

    // Modify button captions
    modifyButtonCaptions();

    // Right-align buttons
    if (m_bRightAlignButtons)
        rightAlignButtons();

    // Install timers
    if (m_nTimerTimeout || m_nDisabledTimeout)
        installTimeoutDisabledTimer();

    if (m_dwTextAnimationInterval && !m_animatedTexts.isEmpty())
        installAnimatedTextTimer();

    if (m_dwCaptionAnimationInterval && !m_animatedCaptions.isEmpty())
        installAnimatedCaptionTimer();

    // Disable button if needed
    if (m_nDisabledTimeout)
    {
        QAbstractButton* btn = m_pButtonBox->button(static_cast<QDialogButtonBox::StandardButton>(m_nDisabledButtonId));
        if (btn)
            btn->setEnabled(false);
    }

    // Call OnInitDone
    onInitDone();

    // Show dialog (modal)
    return QDialog::exec();
}

// ----------------------------------------------------------------------
// Protected virtual methods
// ----------------------------------------------------------------------
QString CMessageBoxX::getCurrentTimerCaption()
{
    if (m_nTimerTimeout && (m_nTimerCountdown <= 10) && m_bModifyCaptionForTimerCountdown)
        return m_sCaption + QString(" (Closing in %1 seconds)").arg(m_nTimerCountdown);
    else
        return m_sCaption;
}

QString CMessageBoxX::getCurrentDisabledCaption()
{
    if (m_nDisabledTimeout && m_bModifyCaptionForDisabledCountdown)
        return m_sCaption + QString(" (%1 seconds remaining)").arg(m_nDisabledCountdown);
    else
        return m_sCaption;
}

bool CMessageBoxX::installTimeoutDisabledTimer()
{
    if (!m_pTimeoutTimer)
    {
        m_pTimeoutTimer = new QTimer(this);
        connect(m_pTimeoutTimer, &QTimer::timeout, this, &CMessageBoxX::onTimeoutTimer);
    }
    m_pTimeoutTimer->start(1000);
    return true;
}

bool CMessageBoxX::installAnimatedTextTimer()
{
    if (!m_pAnimatedTextTimer)
    {
        m_pAnimatedTextTimer = new QTimer(this);
        connect(m_pAnimatedTextTimer, &QTimer::timeout, this, &CMessageBoxX::onAnimatedTextTimer);
    }
    m_pAnimatedTextTimer->start(m_dwTextAnimationInterval);
    return true;
}

bool CMessageBoxX::installAnimatedCaptionTimer()
{
    if (!m_pAnimatedCaptionTimer)
    {
        m_pAnimatedCaptionTimer = new QTimer(this);
        connect(m_pAnimatedCaptionTimer, &QTimer::timeout, this, &CMessageBoxX::onAnimatedCaptionTimer);
    }
    m_pAnimatedCaptionTimer->start(m_dwCaptionAnimationInterval);
    return true;
}

void CMessageBoxX::doTimeoutDisabledButtonUpdate()
{
    // Decrement counters
    if (m_nTimerTimeout)
        --m_nTimerCountdown;
    if (m_nDisabledTimeout)
        --m_nDisabledCountdown;

    // Re-enable disabled button if countdown reached zero
    if (m_nDisabledTimeout && m_nDisabledCountdown == 0)
    {
        QAbstractButton* btn = m_pButtonBox->button(static_cast<QDialogButtonBox::StandardButton>(m_nDisabledButtonId));
        if (btn)
            btn->setEnabled(true);

        // Reset caption if timer caption not active
        bool bUpdateCaption = (m_bModifyCaptionForDisabledCountdown &&
                               ((m_nTimerTimeout && (m_nTimerCountdown > 10)) || (!m_nTimerTimeout)));
        if (bUpdateCaption)
            setWindowTitle(m_sCaption);

        m_nDisabledTimeout = 0;
        m_nDisabledButtonId = 0;

        // Kill timer if no timeout pending
        if (m_nTimerTimeout == 0 && m_pTimeoutTimer)
            m_pTimeoutTimer->stop();
    }

    // Auto-close on timeout
    if (m_nTimerTimeout && m_nTimerCountdown == 0)
    {
        if (m_pTimeoutTimer)
            m_pTimeoutTimer->stop();

        // Emit signal and click the button
        emit timeoutTriggered(m_nTimerButtonId);
        QAbstractButton* btn = m_pButtonBox->button(static_cast<QDialogButtonBox::StandardButton>(m_nTimerButtonId));
        if (btn)
            btn->click();

        m_nTimerTimeout = 0;
        m_nTimerButtonId = 0;
    }
    else
    {
        // Update caption if needed
        QString newCaption = getCurrentTimerCaption();
        if (newCaption != windowTitle())
            setWindowTitle(newCaption);
        else
        {
            newCaption = getCurrentDisabledCaption();
            if (newCaption != windowTitle())
                setWindowTitle(newCaption);
        }
    }
}

void CMessageBoxX::doAnimatedTextUpdate()
{
    if (m_animatedTexts.isEmpty())
        return;

    ++m_nCurrentAnimatedTextIndex;
    if (m_nCurrentAnimatedTextIndex >= m_animatedTexts.size())
    {
        if (m_bAnimatedTextLoop)
            m_nCurrentAnimatedTextIndex = 0;
        else
        {
            if (m_pAnimatedTextTimer)
                m_pAnimatedTextTimer->stop();
            return;
        }
    }

    QWidget* textWidget = findOurTextControl();
    if (QLabel* lbl = qobject_cast<QLabel*>(textWidget))
        lbl->setText(m_animatedTexts[m_nCurrentAnimatedTextIndex]);
    else if (QTextEdit* edit = qobject_cast<QTextEdit*>(textWidget))
        edit->setPlainText(m_animatedTexts[m_nCurrentAnimatedTextIndex]);
}

void CMessageBoxX::doAnimatedCaptionUpdate()
{
    if (m_animatedCaptions.isEmpty())
        return;

    ++m_nCurrentAnimatedCaptionIndex;
    if (m_nCurrentAnimatedCaptionIndex >= m_animatedCaptions.size())
    {
        if (m_bAnimatedCaptionLoop)
            m_nCurrentAnimatedCaptionIndex = 0;
        else
        {
            if (m_pAnimatedCaptionTimer)
                m_pAnimatedCaptionTimer->stop();
            return;
        }
    }
    setWindowTitle(m_animatedCaptions[m_nCurrentAnimatedCaptionIndex]);
}

bool CMessageBoxX::createCheckBox()
{
    if (!m_pCheckBox)
        return false;
    m_pCheckBox->setText(m_sCheckBoxText);
    m_pCheckBox->setChecked(m_bCheckBoxState);
    m_pCheckBox->show();

    // Adjust layout: move buttons down if necessary (simulate original behavior)
    // In Qt, we can just let the layout handle it, but to mimic the original,
    // we might want to add spacing or adjust geometry. Since we are using layouts,
    // we don't need manual adjustments.
    // However, we might need to ensure the checkbox is above the buttons.
    // It already is in the layout.

    return true;
}

#ifdef CMESSAGEBOXX_HYPERLINK_SUPPORT
bool CMessageBoxX::createHyperlink()
{
    if (!m_pHyperlink)
        return false;
    m_pHyperlink->setHtml(QString("<a href=\"%1\">%2</a>").arg(m_sHyperlinkURL, m_sHyperlinkCaption));
    m_pHyperlink->show();
    return true;
}
#endif

void CMessageBoxX::rightAlignButtons()
{
    // Set button box to right-align buttons
    m_pButtonBox->setCenterButtons(false);
}

void CMessageBoxX::modifyButtonCaptions()
{
    // Iterate through buttons and modify captions based on index order
    QList<QAbstractButton*> buttons = m_pButtonBox->buttons();
    int index = 0;
    for (QAbstractButton* btn : buttons)
    {
        if (m_modifiedButtonCaptions.contains(index))
            btn->setText(m_modifiedButtonCaptions[index]);
        ++index;
    }
}

void CMessageBoxX::onInitDone()
{
    // Derived classes can override
}

// ----------------------------------------------------------------------
// Private slots
// ----------------------------------------------------------------------
void CMessageBoxX::onTimeoutTimer()
{
    doTimeoutDisabledButtonUpdate();
}

void CMessageBoxX::onAnimatedTextTimer()
{
    doAnimatedTextUpdate();
}

void CMessageBoxX::onAnimatedCaptionTimer()
{
    doAnimatedCaptionUpdate();
}

void CMessageBoxX::onUserActivity()
{
    if (m_nTimerTimeout && m_bResetTimeoutOnUserActivity)
    {
        if (m_bModifyCaptionForTimerCountdown && (m_nTimerCountdown <= 10))
            setWindowTitle(m_sCaption);
        m_nTimerCountdown = m_nTimerTimeout;
    }
}

// ----------------------------------------------------------------------
// Event filter for user activity reset
// ----------------------------------------------------------------------
bool CMessageBoxX::eventFilter(QObject* obj, QEvent* event)
{
    if (m_bResetTimeoutOnUserActivity && m_nTimerTimeout)
    {
        if (event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::KeyPress ||
            event->type() == QEvent::KeyRelease)
        {
            onUserActivity();
        }
    }
    return QDialog::eventFilter(obj, event);
}

// ----------------------------------------------------------------------
// Helper functions
// ----------------------------------------------------------------------
QWidget* CMessageBoxX::findTextControl() const
{
    // In our layout, the text control is either m_pTextLabel or m_pSelectableText
    return m_pTextWidget;
}

QWidget* CMessageBoxX::findOurTextControl() const
{
    if (m_bSelectableText && m_pSelectableText)
        return m_pSelectableText;
    return m_pTextLabel;
}

QPushButton* CMessageBoxX::getFirstButton() const
{
    QList<QAbstractButton*> btns = m_pButtonBox->buttons();
    if (!btns.isEmpty())
        return qobject_cast<QPushButton*>(btns.first());
    return nullptr;
}

QRect CMessageBoxX::getRightMostButtonRect() const
{
    QRect rect;
    QList<QAbstractButton*> btns = m_pButtonBox->buttons();
    int maxRight = -1;
    for (QAbstractButton* btn : btns)
    {
        QRect r = btn->geometry();
        if (r.right() > maxRight)
        {
            maxRight = r.right();
            rect = r;
        }
    }
    return rect;
}

// ----------------------------------------------------------------------
// Override close event to capture checkbox state (like OnDestroy)
// ----------------------------------------------------------------------
void CMessageBoxX::closeEvent(QCloseEvent* event)
{
    // Get the current check box state if we have one
    if (m_bCheckBox && m_pCheckBox)
        m_bCheckBoxState = m_pCheckBox->isChecked();

    // Kill the animation timers
    if (m_pAnimatedTextTimer)
    {
        m_pAnimatedTextTimer->stop();
        delete m_pAnimatedTextTimer;
        m_pAnimatedTextTimer = nullptr;
    }
    if (m_pAnimatedCaptionTimer)
    {
        m_pAnimatedCaptionTimer->stop();
        delete m_pAnimatedCaptionTimer;
        m_pAnimatedCaptionTimer = nullptr;
    }
    if (m_pTimeoutTimer)
    {
        m_pTimeoutTimer->stop();
        delete m_pTimeoutTimer;
        m_pTimeoutTimer = nullptr;
    }

    QDialog::closeEvent(event);
}