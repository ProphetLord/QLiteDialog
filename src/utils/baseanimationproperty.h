#pragma once

#include "global.h"
#include "baseanimation.h"

QLITEDIALOG_NAMESPACE_BEGIN

struct AnimationProperty
{
    QByteArray animationProperty;
    QVariantAnimation::KeyValues values;
    int msec = 250;
    QEasingCurve::Type type = QEasingCurve::OutBack;
    Qt::AnchorPoint anchor = Qt::AnchorBottom;
    BaseAnimation::AnimationType animationType = BaseAnimation::None;
};

class AnimationPropertyBuilder
{
public:
    AnimationPropertyBuilder& setAnimationProperty(const QByteArray& animationProperty) {
        m_property.animationProperty = animationProperty;
        return *this;
    }

    AnimationPropertyBuilder& setValues(const QVariantAnimation::KeyValues& values) {
        m_property.values = values;
        return *this;
    }

    AnimationPropertyBuilder& setMsec(int msec) {
        m_property.msec = msec;
        return *this;
    }

    AnimationPropertyBuilder& setType(QEasingCurve::Type type) {
        m_property.type = type;
        return *this;
    }

    AnimationPropertyBuilder& setAnchor(Qt::AnchorPoint anchor) {
        m_property.anchor = anchor;
        return *this;
    }

    AnimationPropertyBuilder& setAnimationType(BaseAnimation::AnimationType animationType) {
        m_property.animationType = animationType;
        return *this;
    }

    AnimationProperty build() const {
        return m_property;
    }

private:
    AnimationProperty m_property;
};

QLITEDIALOG_NAMESPACE_END