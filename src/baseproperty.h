#pragma once
#include "global.h"

QLITEDIALOG_NAMESPACE_BEGIN
struct BaseProperty
{
    bool isModal = false;
    bool isReleaseOnClose = false;
    bool isFrameless = false;
    bool isTopHint = false;
    bool isTaskBar = false;
    bool isTranslucentBackground = false;

    bool isEnterAnimation = false;  // 窗口是否允许进入动画
    bool isCloseAnimation = false;  // 窗口是否允许关闭动画

    bool isResizeable = false;      // 窗口是否允许拖动改变大小
    bool isWidgetShadow = false;    // 是否设置窗口阴影
};

class BasePropertyBuilder
{
public:
    BasePropertyBuilder& setModal(bool enabled = false) {
        m_baseProperty.isModal = enabled;
        return *this;
    }

    BasePropertyBuilder& setReleaseOnClose(bool enabled = false) {
        m_baseProperty.isReleaseOnClose = enabled;
        return *this;
    }

    BasePropertyBuilder& setFrameless(bool enabled = false) {
        m_baseProperty.isFrameless = enabled;
        return *this;
    }

    BasePropertyBuilder& setTopHint(bool enabled = false) {
        m_baseProperty.isTopHint = enabled;
        return *this;
    }

    BasePropertyBuilder& setTaskBar(bool enabled = false) {
        m_baseProperty.isTaskBar = enabled;
        return *this;
    }

    BasePropertyBuilder& setTranslucentBackground(bool enabled = false) {
        m_baseProperty.isTranslucentBackground = enabled;
        return *this;
    }

    BasePropertyBuilder& setResizeable(bool enabled = false) {
        m_baseProperty.isResizeable = enabled;
        return *this;
    }

    BasePropertyBuilder& setWidgetShadow(bool enabled = false) {
        m_baseProperty.isWidgetShadow = enabled;
        return *this;
    }

    BasePropertyBuilder& setAnimationEnable(bool enterEnable = false, bool closeEnable = false) {
        m_baseProperty.isEnterAnimation = enterEnable;
        m_baseProperty.isCloseAnimation = closeEnable;
        return *this;
    }

    BaseProperty build() const {
        return m_baseProperty;
    }

private:
    BaseProperty m_baseProperty;
};
QLITEDIALOG_NAMESPACE_END