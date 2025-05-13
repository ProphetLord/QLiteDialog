#pragma once

#include "../global.h"
#include <qpropertyanimation.h>

QLITEDIALOG_NAMESPACE_BEGIN

struct AnimationProperty;
class BaseAnimationPrivate;
class BaseAnimation : public QObject
{
    Q_DECLARE_PRIVATE(BaseAnimation)
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

    void setupAnimationProperty(const AnimationProperty& animation_property);

    void createBaseAnimation();

    bool isClose() const;

private:
    QScopedPointer<BaseAnimationPrivate> d_ptr;
};

QLITEDIALOG_NAMESPACE_END