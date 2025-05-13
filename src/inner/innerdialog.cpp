#include "innerdialog.h"
#include "innerdialog_p.h"

#include <qapplication.h>
#include <qdebug.h>
#include <qevent.h>
#include <qpainter.h>

QLITEDIALOG_NAMESPACE_BEGIN
InnerDialogPrivate::InnerDialogPrivate(InnerDialog *parent)
    : QObject(parent), q_ptr(parent)
{
    scaleSize = qMakePair(1.0, 1.0);
}
void InnerDialogPrivate::addWidget(QObject *obj) {
    if (!watchWidgets.contains(obj)) {
        watchWidgets.append(obj);
        connect(obj, &QObject::destroyed, this, [&] {
            watchWidgets.removeAll(obj);
        });
    }
}
void InnerDialogPrivate::setWidgetScale(qreal w_scale, qreal h_scale) {
    scaleSize = qMakePair(w_scale, h_scale);
}

InnerDialog::InnerDialog(QWidget *parent)
    : BaseWidget(parent), d_ptr(new InnerDialogPrivate(this)) {
    setupBaseProperty();
    qApp->installEventFilter(this);
    hide();
}

void InnerDialog::refreshWidgetSize() {
    if (d_ptr->mainWidget != nullptr) {
        auto scale = d_ptr->scaleSize;
        auto pos = d_ptr->mainWidget->pos();
        auto rectSize = d_ptr->mainWidget->rect().size();
        auto w_size = QSize(rectSize.width() * scale.first, rectSize.height() * scale.second);
        auto w_pos = QPoint(pos.x() + (rectSize.width() - w_size.width()) / 2, pos.y() + (rectSize.height() - w_size.height()) / 2);
        setGeometry(QRect(w_pos, w_size));
    }
}

InnerDialog &InnerDialog::getInstance() {
    static InnerDialog instance;
    return instance;
}

void InnerDialog::setMainWidget(QWidget *widget) {
    auto &instance = getInstance();
    instance.d_ptr->mainWidget = widget;
}

void InnerDialog::addWatchWidget(QObject *widget) {
    auto &instance = getInstance();
    instance.d_ptr->addWidget(widget);
}
void InnerDialog::setShadowColor(const QColor &color, int shadow) {
    auto &instance = getInstance();
    instance.d_ptr->backgroundColor = color;
    instance.d_ptr->backgroundColor.setAlpha(shadow);
}
void InnerDialog::setInnerWidgetScale(InnerSizeType type, qreal scale) {
    auto& instance = getInstance();
    switch (type) {
        case SizeScale:
            instance.d_ptr->setWidgetScale(scale, scale);
            break;
        case WidthScale:
            instance.d_ptr->setWidgetScale(scale, instance.d_ptr->scaleSize.second);
            break;
        case HeightScale:
            instance.d_ptr->setWidgetScale(instance.d_ptr->scaleSize.first, scale);
            break;
    }
}

void InnerDialog::showEvent(QShowEvent *show_event) {
    refreshWidgetSize();
}

bool InnerDialog::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::Show) {
        if (d_ptr->watchWidgets.contains(object)) {
            this->show();
            auto w = dynamic_cast<QWidget *>(object);
            w->setParent(this);
            auto size = w->size();
            w->move((width() - size.width()) / 2, (height() - size.height()) / 2);
            w->setVisible(true);
            w->installEventFilter(this);
            if (!d_ptr->currentActiveWidget.contains(w)) {
                d_ptr->currentActiveWidget << w;
            }
        }
    } else if (event->type() == QEvent::Close) {
        if (object == d_ptr->mainWidget) {
            if (!d_ptr->currentActiveWidget.isEmpty()) {
                event->ignore();
                return true;
            }
        } else if (d_ptr->watchWidgets.contains(object)) {
            d_ptr->currentActiveWidget.removeOne(dynamic_cast<QWidget *>(object));
            if (d_ptr->currentActiveWidget.isEmpty()) {
                this->hide();
            }
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        if (object == this) {
            if (!d_ptr->currentActiveWidget.isEmpty()) {
                auto topWidget = d_ptr->currentActiveWidget.last();
                if (!topWidget->geometry().contains(dynamic_cast<QMouseEvent *>(event)->pos())) {
                    auto cancelOutside = topWidget->property("cancelOutside");
                    if (!cancelOutside.isValid() || cancelOutside.toBool()) {
                        topWidget->close();
                    }
                }
            }
        }
    } else if (event->type() == QEvent::Resize || event->type() == QEvent::Move) {
        if (object == d_ptr->mainWidget) {
            if (!d_ptr->currentActiveWidget.isEmpty()) {
                refreshWidgetSize();
                for (const auto &widget: d_ptr->currentActiveWidget) {
                    auto size = widget->size();
                    widget->move((width() - size.width()) / 2, (height() - size.height()) / 2);
                }
            }
        }
    }
    return QWidget::eventFilter(object, event);
}

void InnerDialog::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(d_ptr->backgroundColor);
    painter.setPen(d_ptr->backgroundColor);
    painter.drawRect(rect());
}
void InnerDialog::setupProperty(BasePropertyBuilder &builder) {
    builder.setFrameless(true).setTaskBar(true).setTopHint(true).setTranslucentBackground(true);
}
QLITEDIALOG_NAMESPACE_END