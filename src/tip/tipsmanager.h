#pragma once

#include "tipsdialog.h"
LITE_NAMESPACE_BEGIN
class TipsManager : public QObject {
    Q_OBJECT
public:
    static TipsManager& getInstance();

    static TipsDialog* addTipsWidget(QObject* obj, const QString& tips);

    static void removeTipsWidget(QObject* obj);

    static void setTipsLabelStyleSheet(const QString& style);

    static void setTipsLabelMargins(const QMargins& margins);

    static TipsDialog* getTipsDialog(QObject* object);

private:
    explicit TipsManager();

private:
    QHash<QObject*, TipsDialog*> tipsHash;

    QString labelStyleSheet;
    QMargins labelMargins;
};
LITE_NAMESPACE_END