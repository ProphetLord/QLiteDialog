#pragma once

#include <qobject.h>
#include <qimage.h>

#include "global.h"

QLITEDIALOG_NAMESPACE_BEGIN
class WeakHintWidget;
class WeakHintDialogPrivate;
class WeakHintDialog : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(WeakHintDialog)
public:
    enum WeakHintPosition {
        ScreenCenter,   // 屏幕中心
        ScreenLeft,     // 屏幕左侧
        ScreenRight,    // 屏幕右侧
        ScreenTop,      // 屏幕顶部
        ScreenBottom,   // 屏幕底部(默认)
        ParentCenter,   // 父控件中心(需设置setParentWidget)
        ParentLeft,     // 父控件左侧
        ParentRight,    // 父控件右侧
        ParentTop,      // 父控件顶部
        ParentBottom,   // 父控件底部
        CustomPosition  // 自定义位置
    };
    Q_ENUM(WeakHintPosition)

    enum WeakHintDisplayType
    {
        Cover,      // 覆盖(默认)
        Overlap,    // 重叠
        Arrange     // 排列
    };
    Q_ENUM(WeakHintDisplayType)

    enum WeakHintCloseMode
    {
        AutoClose,
        ManualClose
    };
    Q_ENUM(WeakHintCloseMode)

    explicit WeakHintDialog(QObject* parent = nullptr);

    void addWeakHintLabel(const QString& text, WeakHintPosition position = ScreenBottom, QImage icon = QImage(),
        WeakHintCloseMode close_mode = AutoClose, qint64 sec = 2000);

    void setParentWidget(QWidget* parent);

    void setWeakHintDisplayType(WeakHintPosition position, WeakHintDisplayType type);

    void setWeakHintWidgetMargins(const QMargins& margins) const;

    void setScreenMargins(const QMargins& margins) const;

    void setWeakHintWidgetRadius(int radius) const;

    void setWeakHintWidgetSpace(int space) const;

    void setWeakHintWidgetImageSize(const QSize& size) const;

    void setWeakHintWidgetBackground(const QColor& color) const;

    void setWeakHintWidgetFont(const QFont& font) const;

    void setWeakHintWidgetPen(const QPen& pen) const;

    /**
     * @brief 如果有多屏幕，可设置显示屏幕索引，默认显示主屏幕
     * @param index
     */
    void setScreenIndex(int index);
private:
    void addWidget(WeakHintWidget* widget, WeakHintPosition position);

private:
    QScopedPointer<WeakHintDialogPrivate> d_ptr;
};

QLITEDIALOG_NAMESPACE_END