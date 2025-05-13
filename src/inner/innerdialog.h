#pragma once

#include "../basewidget.h"
#include <qwidget.h>
QLITEDIALOG_NAMESPACE_BEGIN

class InnerDialogPrivate;
class InnerDialog : public BaseWidget {
    Q_OBJECT
    Q_DECLARE_PRIVATE(InnerDialog);
public:
    static InnerDialog& getInstance();

    static void setMainWidget(QWidget* widget);

    static void addWatchWidget(QObject* widget);

    static void setShadowColor(const QColor& color, int shadow = 150);

    enum InnerSizeType
    {
        SizeScale,
        WidthScale,
        HeightScale,
    };
    static void setInnerWidgetScale(InnerSizeType type, qreal scale);

private:
    explicit InnerDialog(QWidget *parent = nullptr);

    void refreshWidgetSize();

protected:
    void showEvent(QShowEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;
    void paintEvent(QPaintEvent *event) override;
    void setupProperty(BasePropertyBuilder &builder) override;

private:
    QScopedPointer<InnerDialogPrivate> d_ptr;
};
QLITEDIALOG_NAMESPACE_END