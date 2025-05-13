#include "weakhintdialog.h"
#include "weakhintdialog_p.h"


#include <qlist.h>
#include <qapplication.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qscreen.h>
#include <qfontmetrics.h>

#include <qdebug.h>

QLITEDIALOG_NAMESPACE_BEGIN
WeakHintWidget::WeakHintWidget(const QString &text, WeakHintDialog::WeakHintCloseMode mode,
    WeakHintDialogPrivate* weak, QWidget *parent)
    : BaseWidget(parent), context(text), widget_image(QImage())
    , closeMode(mode)
    , curWeakHint(weak)
{
    setupBaseProperty();
    hide();
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, [&] {
        close();
    });
}

void WeakHintWidget::setImage(const QImage &image) {
    widget_image = image;
}

void WeakHintWidget::setInterval(qint64 sec) const {
    timer->setInterval(sec);
}

void WeakHintWidget::setPosition(WeakHintDialog::WeakHintPosition position, const QPoint &point) {
    if (position == WeakHintDialog::ParentCenter ||
        position == WeakHintDialog::ParentBottom ||
        position == WeakHintDialog::ParentTop ||
        position == WeakHintDialog::ParentLeft ||
        position == WeakHintDialog::ParentRight) {
        auto pos = curWeakHint->parentWidget->pos();
        setGeometry(QRect(point + pos, getWidgetSize()));
    } else {
        setGeometry(QRect(point, getWidgetSize()));
    }
}

QSize WeakHintWidget::getWidgetSize() const {
    QFontMetrics fm(curWeakHint->textFont);
    int width = fm.horizontalAdvance(context);
    int height = fm.height();
    if (!widget_image.isNull()) {
        width += curWeakHint->imageSize.width();
        width += curWeakHint->widgetSpace;

        height = qMax(height, widget_image.height());
    }

    return QSize(width + curWeakHint->widgetMargins.left() + curWeakHint->widgetMargins.right(),
                height + curWeakHint->widgetMargins.top() + curWeakHint->widgetMargins.bottom());
}

void WeakHintWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // painter.fillRect(rect(), curWeakHint->backgroundColor);
    painter.setPen(curWeakHint->backgroundColor);
    painter.setBrush(curWeakHint->backgroundColor);
    painter.drawRoundedRect(rect(), curWeakHint->widgetRadius, curWeakHint->widgetRadius);
    auto mainRect = rect().adjusted(curWeakHint->widgetMargins.left(), curWeakHint->widgetMargins.top(),
                                    -curWeakHint->widgetMargins.right(), -curWeakHint->widgetMargins.bottom());
    if (!widget_image.isNull()) {
        auto top = mainRect.top() + (mainRect.height() / 2 - curWeakHint->imageSize.height() / 2);
        auto imageRect = QRect(mainRect.left(), top, curWeakHint->imageSize.width(), curWeakHint->imageSize.height());
        painter.drawImage(imageRect, widget_image);
        mainRect = mainRect.adjusted(curWeakHint->imageSize.width() + curWeakHint->widgetSpace, 0, 0, 0);
    }
    painter.setPen(curWeakHint->widgetPen);
    painter.drawText(mainRect, context, QTextOption(Qt::AlignCenter));
}

void WeakHintWidget::showEvent(QShowEvent *event) {
    if (closeMode == WeakHintDialog::AutoClose) {
        if (!timer->isActive()) {
            timer->start();
        }
    }
    BaseWidget::showEvent(event);
}
void WeakHintWidget::mousePressEvent(QMouseEvent *event) {
    if (closeMode == WeakHintDialog::ManualClose) {
        isWidgetPressed = true;
    }
    BaseWidget::mousePressEvent(event);
}
void WeakHintWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (isWidgetPressed) {
        close();
    }
    BaseWidget::mouseReleaseEvent(event);
}
void WeakHintWidget::setupProperty(BasePropertyBuilder &builder) {
    builder.setTranslucentBackground(true);
    BaseWidget::setupProperty(builder);
}

WeakHintDialogPrivate::WeakHintDialogPrivate(WeakHintDialog *q)
    : QObject(q), q_ptr(q)
{
    widgetMargins = QMargins(4, 4, 4, 4);
    widgetRadius = 4;
    widgetSpace = 4;
    imageSize = QSize(24, 24);
    backgroundColor = QColor(0x29292C);
    textFont = qApp->font();
    widgetPen = QPen(QColor(0xC9D1D9));
    screenMargin = QMargins(20, 20, 20, 20);
}

void WeakHintDialogPrivate::addWeakHintWidget(WeakHintWidget *widget, WeakHintDialog::WeakHintPosition position) {
    auto &widgets = weakHintWidgets[position];
    widgets.append(widget);
    weakHintWidgets[position] = widgets;
    connect(widget, &QObject::destroyed, this, [&, position, widget](QObject *obj) {
        weakHintWidgets[position].removeAll(widget);
        weakHintWidgetProcess();
    });
    weakHintWidgetProcess();
}
void WeakHintDialogPrivate::setPositionDisplayType(WeakHintDialog::WeakHintPosition position, WeakHintDialog::WeakHintDisplayType type) {
    weakHintDisplayType[position] = type;
}
QScreen *WeakHintDialogPrivate::getScreen() {
    auto screens = QGuiApplication::screens();
    if (screens.isEmpty()) {
        return nullptr;
    }
    /*
    for (int i = 0; i < screens.size(); i++) {
        auto screen = screens[i];
        qDebug() << "屏幕" << i << "分辨率:" << screen->geometry();
        qDebug() << "画布区域:" << screen->availableGeometry();
    }
    */
    return screenIndex < screens.size() ? screens[screenIndex] : screens.first();
}
QHash<WeakHintWidget *, QSize> WeakHintDialogPrivate::getWeakHintWidgetsBias(WeakHintDialog::WeakHintPosition position) {
    QHash<WeakHintWidget *, QSize> result;
    WeakHintDialog::WeakHintDisplayType type = WeakHintDialog::Cover;
    if (weakHintDisplayType.contains(position)) {
        type = weakHintDisplayType[position];
    }
    auto widgets = weakHintWidgets[position];
    QSize totalSize = QSize(20, 20);
    for (const auto &it: widgets) {
        switch (position) {
            case WeakHintDialog::ScreenCenter:
            case WeakHintDialog::ScreenTop:
            case WeakHintDialog::ScreenBottom:
            case WeakHintDialog::ParentCenter:
            case WeakHintDialog::ParentTop:
            case WeakHintDialog::ParentBottom:
            case WeakHintDialog::CustomPosition: {
                totalSize += QSize(it->getWidgetSize().width() + 20, 0);
            }
                break;
            case WeakHintDialog::ScreenLeft:
            case WeakHintDialog::ScreenRight:
            case WeakHintDialog::ParentLeft:
            case WeakHintDialog::ParentRight: {
                totalSize += QSize( 0, it->getWidgetSize().height() + 20);
            }
                break;
        }
    }

    for (const auto &it: widgets) {
        switch (type) {
            case WeakHintDialog::Cover: {
                result[it] = widgets.last()->getWidgetSize();
                return result;
            }
            case WeakHintDialog::Overlap: {
                result[it] = it->getWidgetSize();
            } break;
            case WeakHintDialog::Arrange: {
                result[it] = totalSize;
            } break;
        }
    }

    return result;
}
QPoint WeakHintDialogPrivate::getWeakHintWidgetPoint(WeakHintDialog::WeakHintPosition position, QSize biasSize, QSize biasRectSize) {
    auto screen = getScreen();
    if (screen == nullptr) {
        qFatal("not has screen...");
        return QPoint();
    }
    auto screenSize = screen->geometry();
    auto screenPos = screen->availableGeometry().topLeft();

    switch (position) {
        case WeakHintDialog::ScreenCenter: {
            int w = screenSize.width() / 2 - biasSize.width() / 2;
            int h = screenSize.height() / 2 - biasSize.height() / 2;
            return screenPos + QPoint(w, h) + QPoint(biasRectSize.width(), 0);
        }
        case WeakHintDialog::ScreenLeft: {
            int w = screenMargin.left();
            int h = screenSize.height() / 2 - biasSize.height() / 2;
            return screenPos + QPoint(w, h) + QPoint(0, biasRectSize.height());
        }
        case WeakHintDialog::ScreenRight: {
            int w = screenSize.width() - biasSize.width() - screenMargin.right();
            int h = screenSize.height() / 2 - biasSize.height() / 2;
            return screenPos + QPoint(w, h) + QPoint(0, biasRectSize.height());
        }
        case WeakHintDialog::ScreenTop: {
            int w = screenSize.width() / 2 - biasSize.width() / 2;
            int h = screenMargin.top();
            return screenPos + QPoint(w, h) + QPoint(biasRectSize.width(), 0);
        }
        case WeakHintDialog::ScreenBottom: {
            int w = screenSize.width() / 2 - biasSize.width() / 2;
            int h = screenSize.height() - biasSize.height() - screenMargin.bottom();
            return screenPos + QPoint(w, h) + QPoint(biasRectSize.width(), 0);
        }
        case WeakHintDialog::ParentCenter: {
            if (parentWidget) {
                int w = parentWidget->width() / 2 - biasSize.width() / 2;
                int h = parentWidget->height() / 2 - biasSize.height() / 2;
                return QPoint(w, h) + QPoint(biasRectSize.width(), 0);
            }
        } break;
        case WeakHintDialog::ParentLeft: {
            if (parentWidget) {
                int w = screenMargin.left();
                int h = parentWidget->height() / 2 - biasSize.height() / 2;
                return QPoint(w, h) + QPoint(0, biasRectSize.height());
            }
        } break;
        case WeakHintDialog::ParentRight: {
            if (parentWidget) {
                int w = parentWidget->width() - biasSize.width() - screenMargin.right();
                int h = parentWidget->height() / 2 - biasSize.height() / 2;
                return QPoint(w, h) + QPoint(0, biasRectSize.height());
            }
        } break;
        case WeakHintDialog::ParentTop: {
            if (parentWidget) {
                int w = parentWidget->width() / 2 - biasSize.width() / 2;
                int h = screenMargin.top();
                return QPoint(w, h) + QPoint(biasRectSize.width(), 0);
            }
        } break;
        case WeakHintDialog::ParentBottom: {
            if (parentWidget) {
                int w = parentWidget->width() / 2 - biasSize.width() / 2;
                int h = parentWidget->height() - biasSize.height() - screenMargin.bottom();
                return QPoint(w, h) + QPoint(biasRectSize.width(), 0);
            }
        } break;
        case WeakHintDialog::CustomPosition: {

        } break;
    }
    return QPoint();
}

void WeakHintDialogPrivate::weakHintWidgetProcess(bool widgetMove) {
    for (const auto &it: weakHintWidgets.keys()) {
        if (widgetMove) {
            if (it != WeakHintDialog::ParentCenter &&
                it != WeakHintDialog::ParentBottom &&
                it != WeakHintDialog::ParentTop &&
                it != WeakHintDialog::ParentLeft &&
                it != WeakHintDialog::ParentRight) {
                continue;
            }
        }
        auto type = weakHintDisplayType[it];
        auto bias = getWeakHintWidgetsBias(it);
        if (bias.isEmpty()) {
            continue;
        }

        int totalWidth = 0;
        int totalHeight = 0;
        for (const auto &its: bias.keys()) {
            auto biasSize = bias[its];
            switch (type) {
                case WeakHintDialog::Cover: {
                    its->setPosition(it, getWeakHintWidgetPoint(it, biasSize, QSize(0, 0)));
                } break;
                case WeakHintDialog::Overlap: {
                    its->setPosition(it, getWeakHintWidgetPoint(it, biasSize, QSize(0, 0)));
                } break;
                case WeakHintDialog::Arrange: {
                    its->setPosition(it, getWeakHintWidgetPoint(it, biasSize, QSize(totalWidth, totalHeight)));
                    totalWidth += its->getWidgetSize().width() + 20;
                    totalHeight += its->getWidgetSize().height() + 20;
                } break;
            }
            its->show();
        }
    }
}
bool WeakHintDialogPrivate::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::Move || event->type() == QEvent::Resize) {
        if (watched == parentWidget) {
            weakHintWidgetProcess(true);
        }
    }
    return QObject::eventFilter(watched, event);
}

WeakHintDialog::WeakHintDialog(QObject *parent)
    : QObject(parent), d_ptr(new WeakHintDialogPrivate(this)) {
}

void WeakHintDialog::addWidget(WeakHintWidget *widget, WeakHintPosition position) {
    Q_D(WeakHintDialog);
    d->addWeakHintWidget(widget, position);
}

void WeakHintDialog::addWeakHintLabel(const QString &text, WeakHintPosition position, QImage icon,
    WeakHintCloseMode close_mode, qint64 sec)
{
    auto widget = new WeakHintWidget(text, close_mode, d_ptr.data(), d_ptr->parentWidget);
    widget->setImage(icon);
    widget->setInterval(sec);
    addWidget(widget, position);
}

void WeakHintDialog::setParentWidget(QWidget *parent) {
    Q_D(WeakHintDialog);
    d->parentWidget = parent;
    parent->installEventFilter(d);
}
void WeakHintDialog::setWeakHintDisplayType(WeakHintPosition position, WeakHintDisplayType type) {
    Q_D(WeakHintDialog);
    d->setPositionDisplayType(position, type);
}
void WeakHintDialog::setWeakHintWidgetMargins(const QMargins &margins) const {
    d_ptr->widgetMargins = margins;
}
void WeakHintDialog::setScreenMargins(const QMargins &margins) const {
    d_ptr->screenMargin = margins;
}
void WeakHintDialog::setWeakHintWidgetRadius(int radius) const {
    d_ptr->widgetRadius = radius;
}
void WeakHintDialog::setWeakHintWidgetSpace(int space) const {
    d_ptr->widgetSpace = space;
}
void WeakHintDialog::setWeakHintWidgetImageSize(const QSize &size) const {
    d_ptr->imageSize = size;
}
void WeakHintDialog::setWeakHintWidgetBackground(const QColor &color) const {
    d_ptr->backgroundColor = color;
}
void WeakHintDialog::setWeakHintWidgetFont(const QFont &font) const {
    d_ptr->textFont = font;
}
void WeakHintDialog::setWeakHintWidgetPen(const QPen &pen) const {
    d_ptr->widgetPen = pen;
}
void WeakHintDialog::setScreenIndex(int index) {
    d_ptr->screenIndex = index;
}
QLITEDIALOG_NAMESPACE_END