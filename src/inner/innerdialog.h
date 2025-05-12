#pragma once

#include "../basedialog.h"
#include <qwidget.h>
LITE_NAMESPACE_BEGIN
class InnerDialog : public BaseDialog<QWidget> {
    Q_OBJECT
public:
    static InnerDialog& getInstance();

    static void setMainWidget(QWidget* widget);

    static void addWatchWidget(QObject* widget);

    static void setShadowColor(const QColor& color, int shadow = 150);

private:
    explicit InnerDialog(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *) override;

    bool eventFilter(QObject *, QEvent *) override;

    void paintEvent(QPaintEvent *event) override;

private:
    QWidget* mainWidget = nullptr;
    QColor backgroundColor = QColor(0, 0, 0, 150);

    QList<QObject*> watchWidgets;
    QList<QWidget*> currentActiveWidget;
};
LITE_NAMESPACE_END