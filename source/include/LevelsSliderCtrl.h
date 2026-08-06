// LevelsSliderCtrl.h
#pragma once

#include <QSlider>
#include <QStyleOptionSlider>
#include <QPainter>
#include <QRect>

class CLevelsSliderCtrl : public QSlider
{
    Q_OBJECT
    Q_PROPERTY(bool IsActive READ isActive WRITE setActive)
    Q_PROPERTY(int selMin READ selMin WRITE setSelMin)
    Q_PROPERTY(int selMax READ selMax WRITE setSelMax)

public:
    explicit CLevelsSliderCtrl(QWidget *parent = nullptr)
        : QSlider(parent)
        , m_isActive(false)
        , m_selMin(0)
        , m_selMax(0)
    {
    }

    explicit CLevelsSliderCtrl(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QSlider(orientation, parent)
        , m_isActive(false)
        , m_selMin(0)
        , m_selMax(0)
    {
    }

    bool isActive() const { return m_isActive; }
    void setActive(bool active) {
        if (m_isActive != active) {
            m_isActive = active;
            update();
        }
    }

    int selMin() const { return m_selMin; }
    void setSelMin(int val) {
        if (m_selMin != val) {
            m_selMin = val;
            update();
        }
    }

    int selMax() const { return m_selMax; }
    void setSelMax(int val) {
        if (m_selMax != val) {
            m_selMax = val;
            update();
        }
    }

    void setSelection(int min, int max) {
        if (m_selMin != min || m_selMax != max) {
            m_selMin = min;
            m_selMax = max;
            update();
        }
    }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isActive;
    int m_selMin;
    int m_selMax;
};
