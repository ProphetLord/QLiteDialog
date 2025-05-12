#include "normaldialog.h"
LITE_NAMESPACE_BEGIN
NormalDialog::NormalDialog(QWidget *parent)
    : BaseDialog(parent)
{
    setMinimumSize(200, 200);

    setStyleSheet("background:red");
}
LITE_NAMESPACE_END