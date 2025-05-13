#pragma once

#include "baseanimationproperty.h"

QLITEDIALOG_NAMESPACE_BEGIN

class BaseAnimationPrivate : public QObject{
    Q_OBJECT
    Q_DECLARE_PUBLIC(BaseAnimation)

public:
    explicit BaseAnimationPrivate(BaseAnimation* q);

    void createBaseAnimation();

public:
    AnimationProperty animation_property;
    QWidget* widget = nullptr;
    bool isClosing = false;
    bool isEnterAnimation = true;

private:
    BaseAnimation* q_ptr = nullptr;
};

QLITEDIALOG_NAMESPACE_END