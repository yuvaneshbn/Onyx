// ButtonSafe.cpp
#include "ButtonSafe.h"

ButtonSafe::ButtonSafe(QWidget *parent)
    : QPushButton(parent)
{
}

void ButtonSafe::paintEvent(QPaintEvent *event)
{
    // Original CButtonSafe::DrawItem added a safety null check on the device context.
    // In Qt, the painter is always valid. We simply delegate to the base class paint.
    QPushButton::paintEvent(event);
}