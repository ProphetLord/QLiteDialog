#pragma once

#include "baseproperty.h"

QLITEDIALOG_NAMESPACE_BEGIN
class AbstractBaseDialog
{
public:
    virtual ~AbstractBaseDialog() = default;

protected:
    virtual void setupProperty(BasePropertyBuilder& builder) = 0;
};
QLITEDIALOG_NAMESPACE_END