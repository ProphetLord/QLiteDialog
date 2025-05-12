#include "innerdialog.h"

#include <qapplication.h>
#include <qdebug.h>
#include <qevent.h>
#include <qpainter.h>
LITE_NAMESPACE_BEGIN
InnerDialog::InnerDialog(QWidget* parent)
    : BaseDialog(parent)
{
    windowBackground(true);
    windowRelease(false);
    qApp->installEventFilter(this);

    hide();
}

InnerDialog &InnerDialog::getInstance() {
    static InnerDialog instance;
    return instance;
}

void InnerDialog::setMainWidget(QWidget *widget) {
    auto &instance = getInstance();
    instance.mainWidget = widget;
}

void InnerDialog::addWatchWidget(QObject *widget) {
    auto &instance = getInstance();
    if (!instance.watchWidgets.contains(widget)) {
        instance.watchWidgets.append(widget);
        connect(widget, &QObject::destroyed, &instance, [&](QObject *obj) {
            auto &inner_dialog = getInstance();
            inner_dialog.watchWidgets.removeAll(obj);
        });
    }
}
void InnerDialog::setShadowColor(const QColor &color, int shadow) {
    auto& instance = getInstance();
    instance.backgroundColor = color;
    instance.backgroundColor.setAlpha(shadow);
}

void InnerDialog::showEvent(QShowEvent *show_event) {
    if (mainWidget != nullptr) {
        setGeometry(mainWidget->rect());
        move(mainWidget->pos());
    }
}

bool InnerDialog::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::Show) {
        if (watchWidgets.contains(object)) {
            this->show();
            auto w = dynamic_cast<QWidget *>(object);
            w->setParent(this);
            auto size = w->size();
            w->move((width() - size.width()) / 2, (height() - size.height()) / 2);
            w->setVisible(true);
            w->installEventFilter(this);
            if (!currentActiveWidget.contains(w)) {
                currentActiveWidget << w;
            }
        }
    } else if (event->type() == QEvent::Close) {
        if (object == mainWidget) {
            if (!currentActiveWidget.isEmpty()) {
                event->ignore();
                return true;
            }
        } else if (watchWidgets.contains(object)) {
            currentActiveWidget.removeOne(dynamic_cast<QWidget *>(object));
            if (currentActiveWidget.isEmpty()) {
                this->hide();
            }
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        if (object == this) {
            if (!currentActiveWidget.isEmpty()) {
                auto topWidget = currentActiveWidget.last();
                if (!topWidget->geometry().contains(dynamic_cast<QMouseEvent *>(event)->pos())) {
                    auto cancelOutside = topWidget->property("cancelOutside");
                    if (!cancelOutside.isValid() || cancelOutside.toBool()) {
                        topWidget->close();
                    }
                }
            }
        }
    } else if (event->type() == QEvent::Resize) {
        if (object == mainWidget) {
            if (!currentActiveWidget.isEmpty()) {
                setGeometry(QRect(mainWidget->pos(), mainWidget->size()));
                for (const auto &widget: currentActiveWidget) {
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
    painter.setBrush(backgroundColor);
    painter.setPen(backgroundColor);
    painter.drawRect(rect());
}
LITE_NAMESPACE_END