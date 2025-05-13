#pragma once

#include "../basewidget.h"
#include <qdialog.h>
QLITEDIALOG_NAMESPACE_BEGIN
class NormalDialog : public BaseWidget {
    Q_OBJECT
public:
    explicit NormalDialog(QWidget *parent = nullptr);

protected:
    void setupProperty(BasePropertyBuilder &builder) override;

    void paintEvent(QPaintEvent *event) override;
};
QLITEDIALOG_NAMESPACE_END