#pragma once

#include "../basedialog.h"
#include <qdialog.h>
LITE_NAMESPACE_BEGIN
class NormalDialog : public BaseDialog<QDialog> {
    Q_OBJECT
public:
    explicit NormalDialog(QWidget* parent = nullptr);
};
LITE_NAMESPACE_END