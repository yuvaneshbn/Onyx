// ButtonEx.h
#pragma once

#include <QPushButton>
#include <QColor>

class ButtonEx : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor faceColor READ faceColor WRITE setFaceColor)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)

public:
    explicit ButtonEx(QWidget *parent = nullptr);
    explicit ButtonEx(const QString &text, QWidget *parent = nullptr);
    ~ButtonEx() override;

    // Color accessors (mirror m_FaceColor / m_TextColor)
    QColor faceColor() const;
    void setFaceColor(const QColor &color);

    QColor textColor() const;
    void setTextColor(const QColor &color);

    // Change colors when enabled state changes.
    void setEnabled(bool enabled);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;   // kept for completeness

private:
    QColor m_FaceColor;    // default background
    QColor m_TextColor;    // default text color
};

using CButtonEx = ButtonEx;
