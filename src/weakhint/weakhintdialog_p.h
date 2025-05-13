#pragma once

#include "weakhintdialog.h"
#include "../basewidget.h"
#include <qpen.h>

QLITEDIALOG_NAMESPACE_BEGIN
class WeakHintDialogPrivate;
class WeakHintWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit WeakHintWidget(const QString& text, WeakHintDialog::WeakHintCloseMode mode, WeakHintDialogPrivate* weak, QWidget* parent = nullptr);

    void setImage(const QImage &icon);

    void setInterval(qint64 sec) const;

    void setPosition(WeakHintDialog::WeakHintPosition position, const QPoint& point);

    QSize getWidgetSize() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void setupProperty(BasePropertyBuilder &builder) override;

private:
    QTimer* timer = nullptr;
    QString context; // 提示文本
    QImage widget_image;

    bool isWidgetPressed = false;

    WeakHintDialog::WeakHintCloseMode closeMode = WeakHintDialog::AutoClose;
    WeakHintDialogPrivate* curWeakHint;
};

class WeakHintDialogPrivate : public QObject
{
    Q_DECLARE_PUBLIC(WeakHintDialog)

public:
    explicit WeakHintDialogPrivate(WeakHintDialog *q);

    void addWeakHintWidget(WeakHintWidget *widget, WeakHintDialog::WeakHintPosition position);

    void setPositionDisplayType(WeakHintDialog::WeakHintPosition, WeakHintDialog::WeakHintDisplayType);

private:
    QScreen* getScreen();
    QHash<WeakHintWidget*, QSize> getWeakHintWidgetsBias(WeakHintDialog::WeakHintPosition position);
    QPoint getWeakHintWidgetPoint(WeakHintDialog::WeakHintPosition position, QSize biasSize, QSize biasRectSize);
    void weakHintWidgetProcess(bool widgetMove = false);

    bool eventFilter(QObject *watched, QEvent *event) override;

public:

    WeakHintDialog * q_ptr = nullptr;
    QWidget* parentWidget = nullptr;

    QMargins widgetMargins;
    int widgetRadius;
    int widgetSpace;
    QSize imageSize;
    QColor backgroundColor;
    QFont textFont;
    QPen widgetPen;
    QMargins screenMargin;

    int screenIndex = 0;

private:
    QHash<WeakHintDialog::WeakHintPosition, QList<WeakHintWidget*>> weakHintWidgets;
    QHash<WeakHintDialog::WeakHintPosition, WeakHintDialog::WeakHintDisplayType> weakHintDisplayType;
};
QLITEDIALOG_NAMESPACE_END