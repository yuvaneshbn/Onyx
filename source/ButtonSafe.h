// ButtonSafe.h
#pragma once

#include <QPushButton>

class ButtonSafe : public QPushButton
{
    Q_OBJECT

public:
    explicit ButtonSafe(QWidget *parent = nullptr);
    // No special members needed beyond base class

protected:
    void paintEvent(QPaintEvent *event) override;
};