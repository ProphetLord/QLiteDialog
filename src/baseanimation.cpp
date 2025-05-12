#include "baseanimation.h"

#include <qwidget.h>
LITE_NAMESPACE_BEGIN
QPropertyAnimation* BaseAnimation::createAnimation(QWidget *widget, const QByteArray &property, const QVariantAnimation::KeyValues &values, int msec, QEasingCurve::Type easingType) {
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
    : QObject(parent), widget(w), isEnterAnimation(enterAnimation)
{
}

void BaseAnimation::createBaseAnimation() {
    QPropertyAnimation *animation = nullptr;
    switch (animationType) {
        case None:
            break;
        case Custom: {
            animation = createAnimation(widget, property, values, msec, type);
        } break;
        case Slide: {
            if (isEnterAnimation) {
                animation = createSlidePopupAnimationEnter(widget, anchor, property.isEmpty() ? "geometry" : property, msec, type);
            } else {
                animation = createSlidePopupAnimationClose(widget, anchor, property.isEmpty() ? "geometry" : property, msec, type);
            }
        } break;
        case Zoom: {
            if (isEnterAnimation) {
                animation = createPopupZoomAnimationEnter(widget, property.isEmpty() ? "geometry" : property, msec, type);
            } else {
                animation = createPopupZoomAnimationClose(widget, property.isEmpty() ? "geometry" : property, msec, type);
            }
        } break;
        case Opacity: {
            if (isEnterAnimation) {
                animation = createOpacityAnimationEnter(widget, property.isEmpty() ? "windowOpacity" : property, msec, type);
            } else {
                animation = createOpacityAnimationClose(widget, property.isEmpty() ? "windowOpacity" : property, msec, type);
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
bool BaseAnimation::isClose() const {
    return animationType == None ? true : isClosing;
}
LITE_NAMESPACE_END