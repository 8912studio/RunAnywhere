#include "main/input/active_path_object.h"

namespace ra::main::input {

ActivePathObject::ActivePathObject(std::shared_ptr<ActivePathData> data) :
    ArgumentObject(std::move(data)) {

}


ActivePathObject::ActivePathObject(std::wstring text) : 
    ActivePathObject(std::make_shared<ActivePathData>(std::move(text))) {

}


GUID ActivePathObject::ClassID() const {
    return { 0xe3669708, 0xc083, 0x43c4, { 0x82, 0x6f, 0x80, 0x3f, 0x2e, 0x2, 0x41, 0xd1 } };
}


std::shared_ptr<ArgumentData> ActivePathObject::CreateData(std::wstring text) {
    return std::make_shared<ActivePathData>(std::move(text));
}

}