#include "tipsmanager.h"
#include <qhash.h>
QLITEDIALOG_NAMESPACE_BEGIN
TipsManager &TipsManager::getInstance() {
    static TipsManager instance;
    return instance;
}

TipsDialog *TipsManager::addTipsWidget(QObject *obj, const QString &tips) {
    auto &instance = getInstance();
    auto tip = new TipsDialog;
    tip->bindTo(obj);
    tip->setText(tips);
    tip->setLabelStyleSheet(instance.labelStyleSheet);
    tip->setLabelMargin(instance.labelMargins);
    if (instance.tipsHash.contains(obj)) {
        auto oldTips = instance.tipsHash.take(obj);
        oldTips->deleteLater();
    }
    instance.tipsHash.insert(obj, tip);

    connect(obj, &QObject::destroyed, [&, obj] {
        instance.tipsHash.remove(obj);
    });
    return tip;
}
void TipsManager::removeTipsWidget(QObject *obj) {
    auto &instance = getInstance();
    if (instance.tipsHash.contains(obj)) {
        instance.tipsHash.remove(obj);
    }
}
void TipsManager::setTipsLabelStyleSheet(const QString &style) {
    auto& instance = getInstance();
    instance.labelStyleSheet = style;
    for (const auto& it : instance.tipsHash) {
        it->setLabelStyleSheet(style);
    }
}

void TipsManager::setTipsLabelMargins(const QMargins &margins) {
    auto &instance = getInstance();
    instance.labelMargins = margins;
    for (const auto &it: instance.tipsHash) {
        it->setLabelMargin(margins);
    }
}
TipsDialog *TipsManager::getTipsDialog(QObject *object) {
    auto& instance = getInstance();
    if (instance.tipsHash.contains(object)) {
        return instance.tipsHash.value(object);
    } else {
        return nullptr;
    }
}

TipsManager::TipsManager() {

}
QLITEDIALOG_NAMESPACE_END