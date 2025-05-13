#pragma once

#include "abstractbasedialog.h"
#include "utils/baseanimationproperty.h"
#include "utils/windowframelesshelper.h"

#include <QWidget>

QLITEDIALOG_NAMESPACE_BEGIN
class BaseWidget : public QWidget, public AbstractBaseDialog {
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = nullptr);

    void setWidgetShadow(QWidget* widget, int radius, const QSize& offset, const QColor& color);

protected:
    void setupProperty(BasePropertyBuilder &builder) override;
    virtual void setupEnterAnimationProperty(AnimationPropertyBuilder& builder);
    virtual void setupCloseAnimationProperty(AnimationPropertyBuilder& builder);

    void setMoveAreaWidget(QWidget *widget);

    /**
     * @brief 窗口拖动开始
     * @note 函数功能全面说明
     *     - 实现窗口拖动开始的判定
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief 窗口拖动结束
     * @note 函数功能全面说明
     *     - 实现窗口拖动结束的处理
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    /**
     * @brief 窗口拖动
     * @note 函数功能全面说明
     *     - 实现窗口拖动过程中窗口位置的计算
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
protected slots:
    void setupBaseProperty();
    void setupAnimationProperty(bool enter = true);

private:
    WindowFramelessHelper framelessHelper;
    BaseAnimation* animation = nullptr;
    BaseAnimation* closeAnimation = nullptr;
};
QLITEDIALOG_NAMESPACE_END