// ButtonDialer.h
#pragma once

#include <QPushButton>
#include <QMap>
#include <QFont>
#include <QEnterEvent>

class ButtonDialer : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool forceNumeric READ forceNumeric WRITE setForceNumeric)

public:
    explicit ButtonDialer(QWidget *parent = nullptr);
    explicit ButtonDialer(const QString &text, QWidget *parent = nullptr);
    ~ButtonDialer() override;

    // Public flag (mirroring original)
    bool forceNumeric() const { return m_forceNumeric; }
    void setForceNumeric(bool enable) { m_forceNumeric = enable; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void initFonts();

    QFont m_fontLetters;                  // Smaller font for letters
    QMap<QString, QString> m_map;         // Digit → letters mapping
    bool m_forceNumeric = false;
    int m_dpiY = 96;                      // DPI scaling (default 96)
};

using CButtonDialer = ButtonDialer;
