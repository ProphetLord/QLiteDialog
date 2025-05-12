#pragma once

#include "../basedialog.h"
#include <qdialog.h>
#include <qlabel.h>
#include <qpointer.h>

LITE_NAMESPACE_BEGIN
class TipsDialog : public BaseDialog<QDialog> {
    Q_OBJECT
public:
    explicit TipsDialog(QWidget* parent = nullptr);

    // 绑定到指定控件
    void bindTo(QObject *obj);

    void setText(const QString &text);

    void setLabelMargin(const QMargins& margins);

    void setLabelStyleSheet(const QString& style);

    void setLabelWrap(bool wrap);

    QLabel* getLabel() const {
        return label;
    }

    void setCustomPoint(const QPoint& point, bool isPoint);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void showAsTooltip();

    void refreshTipsSize();

    // 简单计算显示位置
    QPoint computeTipPosition() const;

private:
    QLabel* label = nullptr;
    QPointer<QObject> m_target;

    bool isCustomPoint = false;
    QPoint customPoint;
};
LITE_NAMESPACE_END