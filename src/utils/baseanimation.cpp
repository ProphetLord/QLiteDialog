#include "baseanimation.h"
#include "baseanimation_p.h"

#include <qwidget.h>
QLITEDIALOG_NAMESPACE_BEGIN
BaseAnimationPrivate::BaseAnimationPrivate(BaseAnimation *q)
    : q_ptr(q) {
}

void BaseAnimationPrivate::createBaseAnimation() {
    QPropertyAnimation *animation = nullptr;
    switch (animation_property.animationType) {
        case BaseAnimation::None:
            break;
        case BaseAnimation::Custom: {
            animation = BaseAnimation::createAnimation(widget, animation_property.animationProperty, animation_property.values, animation_property.msec, animation_property.type);
        } break;
        case BaseAnimation::Slide: {
            if (isEnterAnimation) {
                animation = BaseAnimation::createSlidePopupAnimationEnter(widget, animation_property.anchor, animation_property.animationProperty.isEmpty() ? "geometry" : animation_property.animationProperty, animation_property.msec, animation_property.type);
            } else {
                animation = BaseAnimation::createSlidePopupAnimationClose(widget, animation_property.anchor, animation_property.animationProperty.isEmpty() ? "geometry" : animation_property.animationProperty, animation_property.msec, animation_property.type);
            }
        } break;
        case BaseAnimation::Zoom: {
            if (isEnterAnimation) {
                animation = BaseAnimation::createPopupZoomAnimationEnter(widget, animation_property.animationProperty.isEmpty() ? "geometry" : animation_property.animationProperty, animation_property.msec, animation_property.type);
            } else {
                animation = BaseAnimation::createPopupZoomAnimationClose(widget, animation_property.animationProperty.isEmpty() ? "geometry" : animation_property.animationProperty, animation_property.msec, animation_property.type);
            }
        } break;
        case BaseAnimation::Opacity: {
            if (isEnterAnimation) {
                animation = BaseAnimation::createOpacityAnimationEnter(widget, animation_property.animationProperty.isEmpty() ? "windowOpacity" : animation_property.animationProperty, animation_property.msec, animation_property.type);
            } else {
                animation = BaseAnimation::createOpacityAnimationClose(widget, animation_property.animationProperty.isEmpty() ? "windowOpacity" : animation_property.animationProperty, animation_property.msec, animation_property.type);
            }
        } break;
    }

    if (animation) {
        connect(animation, &QPropertyAnimation::finished, this, [&] {
            if (widget && !isEnterAnimation) {
                isClosing = true;
                widget->close();
            }
        });
    }
}

QPropertyAnimation * BaseAnimation::createAnimation(QWidget *widget, const QByteArray &property, const QVariantAnimation::KeyValues &values, int msec, QEasingCurve::Type easingType) {
    auto animation = new QPropertyAnimation(widget, property);
    animation->setDuration(msec);
    animation->setKeyValues(values);
    animation->setEasingCurve(easingType);
    // 动画结束后完全显示大小
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    return animation;
}

QPropertyAnimation* BaseAnimation::createSlidePopupAnimationEnter(QWidget *widget, Qt::AnchorPoint anchorPoint, const QByteArray &property, int msec, QEasingCurve::Type easingType) {
    QRect endRect = widget->geometry();
    QRect startRect = endRect;
    switch (anchorPoint) {
        case Qt::AnchorLeft: {
            startRect.moveLeft(endRect.left());// 从越过底部开始动画
        } break;
        case Qt::AnchorHorizontalCenter: {
            startRect.moveLeft(endRect.left() + (endRect.right() - endRect.left()) / 2);
        } break;
        case Qt::AnchorRight: {
            startRect.moveLeft(endRect.right());
        } break;
        case Qt::AnchorTop: {
            startRect.moveTop(endRect.top());
        } break;
        case Qt::AnchorVerticalCenter: {
            startRect.moveTop(endRect.top() + (endRect.bottom() - endRect.top()) / 2);
        } break;
        case Qt::AnchorBottom: {
            startRect.moveTop(endRect.bottom());
        } break;
    }
    return createAnimation(widget, property, {{0, startRect}, {1, endRect}}, msec, easingType);
}
QPropertyAnimation* BaseAnimation::createSlidePopupAnimationClose(QWidget *widget, Qt::AnchorPoint anchorPoint, const QByteArray &property, int msec, QEasingCurve::Type easingType) {
    QRect startRect = widget->geometry();
    QRect endRect = startRect;
    switch (anchorPoint) {
        case Qt::AnchorLeft: {
            endRect.moveLeft(startRect.left());// 从越过底部开始动画
        } break;
        case Qt::AnchorHorizontalCenter: {
            endRect.moveLeft(startRect.left() + (startRect.right() - startRect.left()) / 2);
        } break;
        case Qt::AnchorRight: {
            endRect.moveLeft(startRect.right());
        } break;
        case Qt::AnchorTop: {
            endRect.moveTop(startRect.top());
        } break;
        case Qt::AnchorVerticalCenter: {
            endRect.moveTop(startRect.top() + (startRect.bottom() - startRect.top()) / 2);
        } break;
        case Qt::AnchorBottom: {
            endRect.moveTop(startRect.bottom());
        } break;
    }
    return createAnimation(widget, property, {{0, startRect}, {1, endRect}}, msec, easingType);
}

QPropertyAnimation* BaseAnimation::createPopupZoomAnimationEnter(QWidget *widget, const QByteArray &property, int msec, QEasingCurve::Type easingType) {
    // 初始几何，缩放从极小矩形(比如中间点大小为0)
    QRect endRect = widget->geometry();
    QPoint center = endRect.center();
    // 从中心点开始，大小为0矩形
    QRect startRect(center.x(), center.y(), 0, 0);

    return createAnimation(widget, property, {{0, startRect}, {1, endRect}}, msec, easingType);
}
QPropertyAnimation* BaseAnimation::createPopupZoomAnimationClose(QWidget *widget, const QByteArray &property, int msec, QEasingCurve::Type easingType) {
    // 初始几何，缩放从极小矩形(比如中间点大小为0)
    QRect startRect = widget->geometry();
    QPoint center = startRect.center();
    // 从中心点开始，大小为0矩形
    QRect endRect(center.x(), center.y(), 0, 0);

    return createAnimation(widget, property, {{0, startRect}, {1, endRect}}, msec, easingType);
}

QPropertyAnimation* BaseAnimation::createOpacityAnimationEnter(QWidget *widget, const QByteArray &property, int msec, QEasingCurve::Type easingType) {
    widget->setWindowOpacity(0.0);
    return createAnimation(widget, property, {{0, 0.0}, {1, 1.0}}, msec, easingType);
}
QPropertyAnimation* BaseAnimation::createOpacityAnimationClose(QWidget *widget, const QByteArray &property, int msec, QEasingCurve::Type easingType) {
    widget->setWindowOpacity(0.0);
    return createAnimation(widget, property, {{0, 1.0}, {1, 0.0}}, msec, easingType);
}

BaseAnimation::BaseAnimation(QWidget *w, bool enterAnimation, QObject *parent)
    : QObject(parent), d_ptr(new BaseAnimationPrivate(this)) {
    d_ptr->widget = w;
    d_ptr->isEnterAnimation = enterAnimation;
}
void BaseAnimation::setupAnimationProperty(const AnimationProperty &animation_property) {
    d_ptr->animation_property = animation_property;
}
void BaseAnimation::createBaseAnimation() {
    d_ptr->createBaseAnimation();
}

bool BaseAnimation::isClose() const {
    return d_ptr->animation_property.animationType == None ? true : d_ptr->isClosing;
}
QLITEDIALOG_NAMESPACE_END