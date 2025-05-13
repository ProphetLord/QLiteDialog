#pragma once

#include "innerdialog.h"

QLITEDIALOG_NAMESPACE_BEGIN

class InnerDialogPrivate : public QObject
{
    Q_DECLARE_PUBLIC(InnerDialog)
public:
    explicit InnerDialogPrivate(InnerDialog* parent = nullptr);

    void addWidget(QObject* obj);

    void setWidgetScale(qreal w_scale, qreal h_scale);

public:
    QWidget* mainWidget = nullptr;
    QColor backgroundColor = QColor(0, 0, 0, 150);

    QList<QObject*> watchWidgets;
    QList<QWidget*> currentActiveWidget;

    QPair<qreal, qreal> scaleSize;
private:
    InnerDialog* q_ptr;
};

QLITEDIALOG_NAMESPACE_END