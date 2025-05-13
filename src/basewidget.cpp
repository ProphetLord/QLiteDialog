#include "basewidget.h"
#include <QGraphicsDropShadowEffect>
#include <qevent.h>
#include <qpushbutton.h>

QLITEDIALOG_NAMESPACE_BEGIN
BaseWidget::BaseWidget(QWidget *parent)
    : QWidget(parent)
{
    QMetaObject::invokeMethod(this, &BaseWidget::setupBaseProperty, Qt::QueuedConnection);
    // QMetaObject::invokeMethod(this, "setupAnimationProperty", Qt::QueuedConnection, Q_ARG(bool, true));

    animation = new BaseAnimation(this, true, this);
    closeAnimation = new BaseAnimation(this, false, this);
}
void BaseWidget::setWidgetShadow(QWidget *widget, int radius, const QSize &offset, const QColor &color) {
    auto shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(radius);
    shadow->setOffset(offset.width(), offset.height());
    shadow->setColor(color);
    widget->setGraphicsEffect(shadow);
    widget->setWindowFlags(Qt::FramelessWindowHint);       // 无窗口边框
    widget->setAttribute(Qt::WA_TranslucentBackground);             // 背景透明
}

void BaseWidget::setupProperty(BasePropertyBuilder &builder) {
    builder.setReleaseOnClose(true).setFrameless(true).setTaskBar(true).setTopHint(true);
}
void BaseWidget::setupEnterAnimationProperty(AnimationPropertyBuilder &builder) {
    builder.setAnchor(Qt::AnchorBottom).setAnimationType(BaseAnimation::None);
}
void BaseWidget::setupCloseAnimationProperty(AnimationPropertyBuilder &builder) {
    builder.setAnchor(Qt::AnchorBottom).setAnimationType(BaseAnimation::None);
}

void BaseWidget::setMoveAreaWidget(QWidget *widget) {
    framelessHelper.setMoveWidget(widget);
    // 设置界面上的按钮autoDefault属性为false，避免按下回车键，导致触发按钮点击事件
    auto buttons = this->template findChildren<QPushButton *>();
    for (const auto &it: buttons) {
        it->setAutoDefault(false);
    }
}
void BaseWidget::mousePressEvent(QMouseEvent *event) {
    framelessHelper.pressEvent(event);
}
void BaseWidget::mouseReleaseEvent(QMouseEvent *event) {
    framelessHelper.releaseEvent(event);
}
void BaseWidget::mouseMoveEvent(QMouseEvent *event) {
    framelessHelper.moveEvent(event);
}
void BaseWidget::closeEvent(QCloseEvent *event) {
    setupAnimationProperty(false);
    if (closeAnimation->isClose()) {
        event->accept();
        QWidget::closeEvent(event);
    } else {
        event->ignore();
        closeAnimation->createBaseAnimation();
    }
}
void BaseWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    setupAnimationProperty(true);
    animation->createBaseAnimation();
}

void BaseWidget::setupBaseProperty() {
    BasePropertyBuilder builder;
    setupProperty(builder);
    auto widgetProperty = builder.build();
    this->setWindowModality(widgetProperty.isModal ? Qt::ApplicationModal : Qt::NonModal);
    this->setAttribute(Qt::WA_DeleteOnClose, widgetProperty.isReleaseOnClose);
    this->setAttribute(Qt::WA_TranslucentBackground, widgetProperty.isTranslucentBackground);
    widgetProperty.isTopHint ? this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint)
                             : this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
    widgetProperty.isTaskBar ? this->setWindowFlags(this->windowFlags() | Qt::Window)
                             : this->setWindowFlags(this->windowFlags() & ~Qt::Window);
    widgetProperty.isFrameless ? this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint)
                               : this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint);

    framelessHelper.setWindowResizeable(widgetProperty.isResizeable);
}
void BaseWidget::setupAnimationProperty(bool enter) {
    AnimationPropertyBuilder builder;
    if (enter) {
        setupEnterAnimationProperty(builder);
        auto animationProperty = builder.build();
        animation->setupAnimationProperty(animationProperty);
    } else {
        setupCloseAnimationProperty(builder);
        auto animationProperty = builder.build();
        closeAnimation->setupAnimationProperty(animationProperty);
    }
}
QLITEDIALOG_NAMESPACE_END