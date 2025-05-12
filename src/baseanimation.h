#pragma once

#include <qpropertyanimation.h>
#include "bean/datadef.h"

LITE_NAMESPACE_BEGIN
class BaseAnimation : public QObject
{
public:
    static QPropertyAnimation* createAnimation(QWidget* widget, const QByteArray& property, const QVariantAnimation::KeyValues& value, int msec = 250, QEasingCurve::Type easingType = QEasingCurve::Linear);

    static QPropertyAnimation* createSlidePopupAnimationEnter(QWidget* widget, Qt::AnchorPoint anchorPoint, const QByteArray& property = "geometry", int msec = 250, QEasingCurve::Type easingType = QEasingCurve::OutQuad);
    static QPropertyAnimation* createSlidePopupAnimationClose(QWidget* widget, Qt::AnchorPoint anchorPoint, const QByteArray& property = "geometry", int msec = 250, QEasingCurve::Type easingType = QEasingCurve::OutQuad);

    static QPropertyAnimation* createPopupZoomAnimationEnter(QWidget* widget, const QByteArray& property = "geometry", int msec = 250, QEasingCurve::Type easingType = QEasingCurve::OutBack);
    static QPropertyAnimation* createPopupZoomAnimationClose(QWidget* widget, const QByteArray& property = "geometry", int msec = 250, QEasingCurve::Type easingType = QEasingCurve::OutBack);

    static QPropertyAnimation* createOpacityAnimationEnter(QWidget* widget, const QByteArray& property = "windowOpacity", int msec = 250, QEasingCurve::Type easingType = QEasingCurve::Linear);
    static QPropertyAnimation* createOpacityAnimationClose(QWidget* widget, const QByteArray& property = "windowOpacity", int msec = 250, QEasingCurve::Type easingType = QEasingCurve::Linear);

    enum AnimationType
    {
        None,
        Custom,
        Slide,
        Zoom,
        Opacity,
    };

    explicit BaseAnimation(QWidget* w, bool enterAnimation, QObject* parent = nullptr);

    void createBaseAnimation();

    bool isClose() const;

public:
    QWidget* widget = nullptr;
    QByteArray property;
    QVariantAnimation::KeyValues values;
    int msec = 250;
    QEasingCurve::Type type = QEasingCurve::OutBack;
    Qt::AnchorPoint anchor = Qt::AnchorBottom;

    AnimationType animationType = None;

private:
    bool isClosing = false;
    bool isEnterAnimation = true;
};

LITE_NAMESPACE_END