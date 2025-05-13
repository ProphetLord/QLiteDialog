#pragma once

#include "../global.h"
#include <qwidget.h>

QLITEDIALOG_NAMESPACE_BEGIN
enum MouseRegion {
    Window_Left_Border,            ///< 窗口左边框
    Window_Right_Border,           ///< 窗口右边框
    Window_Top_Border,             ///< 窗口上边框
    Window_Bottom_Border,          ///< 窗口下边框
    Window_Left_Top_Corner,        ///< 窗口左上角
    Window_Right_Top_Corner,       ///< 窗口右上角
    Window_Left_Bottom_Corner,     ///< 窗口左下角
    Window_Right_Bottom_Corner,    ///< 窗口右上角
    Window_Internal_Area           ///< 窗口内部区域
};

class WindowFramelessHelper : public QWidget
{
public:
    explicit WindowFramelessHelper(QWidget* parent = nullptr);

    void initWidgetMember(QWidget* wid);

    void pressEvent(QMouseEvent *event);

    void moveEvent(QMouseEvent *event);

    bool eventFilter(QObject *watched, QEvent *event) override;

    void releaseEvent(QMouseEvent *event);

    MouseRegion getMouseRegion(QPoint mousePos);

    void changeCursorShapeByRegion();

    void stretchWindowByMouse(QMouseEvent *event) const;

    void setMoveWidget(QWidget* widget);

    void setWindowResizeable(bool resizeable);
private:
    bool leftButtonPressed = false; ///< 窗口拖动状态标志
    QPoint start;   ///< 窗口拖动的起始位置
    MouseRegion currentMouseRegion = MouseRegion::Window_Internal_Area;     ///< 当前鼠标所在区域
    QWidget* widget = nullptr;
    QWidget* moveWidget = nullptr;
    bool windowResizeable = true;
};
QLITEDIALOG_NAMESPACE_END