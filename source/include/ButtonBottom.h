// ButtonBottom.h
#pragma once

#include <QPushButton>
#include <QColor>
#include <QEnterEvent>

class ButtonBottom : public QPushButton
{
    Q_OBJECT
public:
    explicit ButtonBottom(QWidget *parent = nullptr);
    explicit ButtonBottom(const QString &text, QWidget *parent = nullptr);
    ~ButtonBottom() override;

private:
    QColor m_clrButton;
    QColor m_clrButtonHover;
    QColor m_clrButtonPushed;
    QColor m_clrButtonText;
    QColor m_clrButtonTextPushed;

    // State flags
    bool m_bHover = false;

protected:
    void paintEvent(QPaintEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

using CButtonBottom = ButtonBottom;
