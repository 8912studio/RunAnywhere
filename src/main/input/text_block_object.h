#pragma once

#include "main/input/argument_object.h"

namespace ra::main::input {

class TextBlockObject : public ArgumentObject {
public:
    using ArgumentObject::ArgumentObject;

    GUID ClassID() const override;
};

}