#include "module/calculator/result_text_builder.h"
#include <sstream>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/to_string.h>

namespace ra::module::calculator {
namespace {

std::wstring GetBasePrefix(int base) {

    switch (base) {
    case 2:
        return L"0b";
    case 8:
        return L"0";
    case 16:
        return L"0x";
    default:
        return {};
    }
}

}

ResultTextBuilder::ResultTextBuilder(
	const calculator::EvaluateResult& evaluate_result,
	const calculator::Modifier& modifier) 
	:
	evaluate_result_(evaluate_result),
	modifier_(modifier) {

}


ResultText ResultTextBuilder::Build() const {

    ResultText result;
    result.prefix = GetBasePrefix(modifier_.base);
    
    if (modifier_.base == 10) {
        result.content = GetDecimalText();
    }
    else {
        result.content = GetNonDecimalText();
    }

    return result;
}


std::wstring ResultTextBuilder::GetDecimalText() const {

    auto value = evaluate_result_.decimal_value;

    switch (modifier_.unit) {
    case calculator::NumberUnit::Tera:
        value /= 1024;
    case calculator::NumberUnit::Giga:
        value /= 1024;
    case calculator::NumberUnit::Mega:
        value /= 1024;
    case calculator::NumberUnit::Kilo:
        value /= 1024;
    default:
        break;
    }

    std::wstring result;
    if (modifier_.bit_length) {

        auto value_64 = value.convert_to<std::uint64_t>();

        switch (*modifier_.bit_length) {
        case 8:
            if (modifier_.use_unsigned_type) {
                result = zaf::ToWideString(static_cast<std::uint8_t>(value_64));
            }
            else {
                result = zaf::ToWideString(static_cast<std::int8_t>(value_64));
            }
            break;
        case 16:
            if (modifier_.use_unsigned_type) {
                result = zaf::ToWideString(static_cast<std::uint16_t>(value_64));
            }
            else {
                result = zaf::ToWideString(static_cast<std::int16_t>(value_64));
            }
            break;
        case 32:
            if (modifier_.use_unsigned_type) {
                result = zaf::ToWideString(static_cast<std::uint32_t>(value_64));
            }
            else {
                result = zaf::ToWideString(static_cast<std::int32_t>(value_64));
            }
            break;
        case 64:
            if (modifier_.use_unsigned_type) {
                result = zaf::ToWideString(static_cast<std::uint64_t>(value_64));
            }
            else {
                result = zaf::ToWideString(static_cast<std::int64_t>(value_64));
            }
            break;
        default:
            assert(false);
            break;
        }
    }
    else {

        std::ostringstream stream;
        stream << std::setprecision(20) << value;
        result = zaf::FromUTF8String(stream.str());
    }

    return result;
}


std::wstring ResultTextBuilder::GetNonDecimalText() const {

    const auto& value = evaluate_result_.decimal_value;

    zaf::ToStringOptions to_string_options;
    to_string_options.Base(modifier_.base);

    std::wstring result;
    if (modifier_.bit_length) {
        switch (*modifier_.bit_length) {
        case 8:
            result = zaf::ToWideString(value.convert_to<std::uint8_t>(), to_string_options);
            break;

        case 16:
            result = zaf::ToWideString(value.convert_to<std::uint16_t>(), to_string_options);
            break;

        case 32:
            result = zaf::ToWideString(value.convert_to<std::uint32_t>(), to_string_options);
            break;

        case 64:
            result = zaf::ToWideString(value.convert_to<std::uint64_t>(), to_string_options);
            break;

        default: 
            assert(false);
            break;
        }
    }
    else {

        auto value_64 = value.convert_to<std::uint64_t>();
        if (value_64 > std::numeric_limits<std::uint32_t>::max()) {

            result = zaf::ToWideString(value_64, to_string_options);
        }
        else {
            result = zaf::ToWideString(static_cast<std::uint32_t>(value_64), to_string_options);
        }
    }

    if (modifier_.use_upper_case) {
        zaf::Uppercase(result);
    }

    return result;
}

}