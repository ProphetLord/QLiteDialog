#include "tipsdialog.h"

#include <QVBoxLayout>
#include <corecrt_startup.h>

LITE_NAMESPACE_BEGIN
TipsDialog::TipsDialog(QWidget *parent)
    : BaseDialog(parent) {
    windowBackground(true);
    windowRelease(false);
    // setWindowFlags(Qt::ToolTip);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);

    label = new QLabel(this);

    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(label);
    setLayout(layout);

    installEventFilter(this);
}

void TipsDialog::bindTo(QObject *obj) {
    if (m_target) {
        m_target->removeEventFilter(this);
    }

    m_target = obj;

    if (obj) {
        obj->installEventFilter(this);
    }
}
void TipsDialog::setText(const QString &text) {
    label->setText(text);
    refreshTipsSize();
    adjustSize();
}
void TipsDialog::setLabelMargin(const QMargins &margins) {
    label->setContentsMargins(margins);
    refreshTipsSize();
}
void TipsDialog::setLabelStyleSheet(const QString &style) {
    label->setStyleSheet(style);
    refreshTipsSize();
}

void TipsDialog::setLabelWrap(bool wrap) {
    label->setWordWrap(wrap);
    refreshTipsSize();
}

void TipsDialog::setCustomPoint(const QPoint &point, bool isPoint) {
    isCustomPoint = isPoint;
    customPoint = point;
}
bool TipsDialog::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_target)// 监听目标对象事件
    {
        switch (event->type()) {
            case QEvent::Enter:
                showAsTooltip();
                return false;//让目标控件正常响应
            case QEvent::Leave: {
                // 鼠标从目标控件移开后，可能还在TipsDialog上，这里不关闭，让小窗自己监听自己鼠标
                QPoint globalCursorPos = QCursor::pos();
                if (!this->geometry().contains(globalCursorPos)) {
                    this->close();
                }
                return false;
            }
            case QEvent::MouseMove: {
                // 如需跟随移动
                showAsTooltip();
                return false;
            }
            default:
                break;
        }
    } else if (obj == this)// 监听本窗口鼠标事件，移出关闭
    {
        if (event->type() == QEvent::Leave) {
            this->close();
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
}
void TipsDialog::showAsTooltip() {
    if (!m_target)
        return;

    if (!isVisible()) {
        // 显示并尽量置前，且不抢焦点
        QPoint pos = computeTipPosition();
        this->move(pos);
        this->show();
    }
}
void TipsDialog::refreshTipsSize() {
    QFontMetrics fm(label->font());
    int width = fm.horizontalAdvance(label->text());
    int height = fm.height();
    auto margins = label->contentsMargins();
    width += margins.left() + margins.right();
    height += margins.top() + margins.bottom();
    resize(width, height);
}
QPoint TipsDialog::computeTipPosition() const {
    if (!m_target) {
        return mapToGlobal(QPoint(0,0));
    }

    QWidget *w = qobject_cast<QWidget*>(m_target);
    if (!w) {
        return mapToGlobal(QPoint(0,0));
    }

    if (isCustomPoint) {
        return customPoint;
    }

    QRect targetRect = w->rect();
    QPoint bottomLeftGlobal = w->mapToGlobal(targetRect.bottomLeft());

    QPoint tipPos = bottomLeftGlobal + QPoint(0, 5);
    return tipPos;
}
LITE_NAMESPACE_END