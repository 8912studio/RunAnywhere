#include "main/input/active_path_object.h"
#include <zaf/creation.h>
#include "main/input/active_path_window.h"

namespace ra::main::input {

ActivePathObject::ActivePathObject(std::shared_ptr<ActivePathData> data) :
    ArgumentObject(std::move(data)) {

}


ActivePathObject::ActivePathObject(std::wstring text) : 
    ActivePathObject(std::make_shared<ActivePathData>(std::move(text))) {

}


std::shared_ptr<ArgumentData> ActivePathObject::CreateData(std::wstring text) {
    return std::make_shared<ActivePathData>(std::move(text));
}


zaf::Color ActivePathObject::GetBackgroundColor() const {
    return zaf::Color::FromARGB(0x7dffc25c);
}


std::shared_ptr<ArgumentObjectWindow> ActivePathObject::CreateArgumentObjectWindow() {
    return zaf::Create<ActivePathWindow>();
}


std::shared_ptr<zaf::textual::InlineObject> ActivePathObject::Clone() const {
    return zaf::Create<ActivePathObject>(this->Text());
}

}