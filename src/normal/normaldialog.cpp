#include "normaldialog.h"
#include "inner/innerdialog.h"
#include <qpainter.h>

QLITEDIALOG_NAMESPACE_BEGIN
NormalDialog::NormalDialog(QWidget *parent)
    : BaseWidget(parent)
{
    setupBaseProperty();
    resize(200, 200);
    InnerDialog::addWatchWidget(this);
}
void NormalDialog::setupProperty(BasePropertyBuilder &builder) {
    builder.setTranslucentBackground(true).setReleaseOnClose(true);
}
void NormalDialog::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::red);
}
QLITEDIALOG_NAMESPACE_END