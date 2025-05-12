#pragma once

#include "bean/datadef.h"
#include "baseanimation.h"
#include "windowframelesshelper.h"

#include <qevent.h>
#include <QGraphicsDropShadowEffect>

LITE_NAMESPACE_BEGIN

class QPushButton;
template<typename T>
class BaseDialog : public T {
public:
    virtual ~BaseDialog() = default;
    explicit BaseDialog(QWidget *parent = nullptr) : T(parent) {
        windowRelease(true);
        windowFramelessHint(true);
        windowTaskbar(true);
        windowTop(true);

        framelessHelper.initWidgetMember(this);
        animation = new BaseAnimation(this, true, this);
        closeAnimation = new BaseAnimation(this, false, this);
    }

    /**
     * @brief 窗口拖动开始
     * @note 函数功能全面说明
     *     - 实现窗口拖动开始的判定
     */
    void mousePressEvent(QMouseEvent *event) override {
        framelessHelper.pressEvent(event);
    }

    /**
     * @brief 窗口拖动
     * @note 函数功能全面说明
     *     - 实现窗口拖动过程中窗口位置的计算
     */
    void mouseMoveEvent(QMouseEvent *event) override {
        framelessHelper.moveEvent(event);
    }

    /**
     * @brief 窗口拖动结束
     * @note 函数功能全面说明
     *     - 实现窗口拖动结束的处理
     */
    void mouseReleaseEvent(QMouseEvent *event) override {
        framelessHelper.releaseEvent(event);
    }

    void showEvent(QShowEvent* event) override {
        T::showEvent(event);
        animation->createBaseAnimation();
    }

    void closeEvent(QCloseEvent* event) override {
        if (closeAnimation->isClose()) {
            event->accept();
            T::closeEvent(event);
        } else {
            event->ignore();
            closeAnimation->createBaseAnimation();
        }
    }

    /**
     * @brief 窗口是否允许拖动改变大小
     * @param resizeable
     */
    void setWindowResizeable(bool resizeable) {
        framelessHelper.setWindowResizeable(resizeable);
    }

    void setWidgetShadow(QWidget* widget, int radius, const QSize& offset, const QColor& color) {
        auto shadow = new QGraphicsDropShadowEffect(widget);
        shadow->setBlurRadius(radius);
        shadow->setOffset(offset.width(), offset.height());
        shadow->setColor(color);
        widget->setGraphicsEffect(shadow);
        widget->setWindowFlags(Qt::FramelessWindowHint);       // 无窗口边框
        widget->setAttribute(Qt::WA_TranslucentBackground);             // 背景透明
    }

protected:
    // 设置窗口模态
    void windowModal(bool modal) {
        this->setWindowModality(modal ? Qt::ApplicationModal : Qt::NonModal);
    }
    // 设置窗口关闭时自动释放
    void windowRelease(bool close) {
        this->setAttribute(Qt::WA_DeleteOnClose, close);
    }
    // 设置窗口置顶
    void windowTop(bool top) {
        if (top) {
            this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
        } else {
            this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
        }
    }
    // 设置窗口任务栏显示
    void windowTaskbar(bool taskbar) {
        if (taskbar) {
            this->setWindowFlags(this->windowFlags() | Qt::Window);
        } else {
            this->setWindowFlags(this->windowFlags() & ~Qt::Window);
        }
    }
    // 设置默认边框
    void windowFramelessHint(bool title) {
        if (title) {
            this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
        } else {
            this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint);
        }
    }
    // 自定义背景颜色
    void windowBackground(bool background) {
        this->setAttribute(Qt::WA_TranslucentBackground, background);
    }

    /**
     * @brief 设置窗口移动区域
     * @param widget
     */
    void setMoveAreaWidget(QWidget* widget) {
        framelessHelper.setMoveWidget(widget);
        // 设置界面上的按钮autoDefault属性为false，避免按下回车键，导致触发按钮点击事件
        auto buttons = this->template findChildren<QPushButton*>();
        for (const auto& it : buttons) {
            it->setAutoDefault(false);
        }
    }

protected:
    WindowFramelessHelper framelessHelper;
    BaseAnimation* animation;
    BaseAnimation* closeAnimation;
};

LITE_NAMESPACE_END