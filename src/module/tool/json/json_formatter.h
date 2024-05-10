#pragma once

#include <boost/json.hpp>
#include <zaf/control/textual/styled_text.h>

namespace ra::mod::tool::json {

class JSONFormatter {
protected:
    void StartFormatting(const boost::json::value& value);

    virtual void OutputNull(std::string_view text) = 0;
    virtual void OutputBool(std::string_view text) = 0;
    virtual void OutputInteger(std::string_view text) = 0;
    virtual void OutputDouble(std::string_view text) = 0;
    virtual void OutputString(std::string_view text) = 0;
    virtual void OutputArrayBegin(std::string_view text) = 0;
    virtual void OutputArrayEnd(std::string_view text) = 0;
    virtual void OutputObjectBegin(std::string_view text) = 0;
    virtual void OutputObjectEnd(std::string_view text) = 0;
    virtual void OutputKey(std::string_view text) = 0;
    virtual void OutputColon(std::string_view text) = 0;
    virtual void OutputComma(std::string_view text) = 0;

private:
    void FormatValue(const boost::json::value& value);
    void FormatDouble(double value);
    void FormatArray(const boost::json::array& array_value);
    void FormatObject(const boost::json::object& object);
};


class JSONBeautifiedFormatter : public JSONFormatter {
public:
    JSONBeautifiedFormatter();

    zaf::textual::StyledText Format(const boost::json::value& value);

protected:
    void OutputNull(std::string_view text) override;
    void OutputBool(std::string_view text) override;
    void OutputInteger(std::string_view text) override;
    void OutputDouble(std::string_view text) override;
    void OutputString(std::string_view text) override;
    void OutputArrayBegin(std::string_view text) override;
    void OutputArrayEnd(std::string_view text) override;
    void OutputObjectBegin(std::string_view text) override;
    void OutputObjectEnd(std::string_view text) override;
    void OutputKey(std::string_view text) override;
    void OutputColon(std::string_view text) override;
    void OutputComma(std::string_view text) override;

private:
    void EnterLevel(std::string_view text);
    void ExitLevel(std::string_view text);
    void OutputIdent();

private:
    zaf::textual::TextStyle key_style_;
    zaf::textual::TextStyle string_style_;
    zaf::textual::TextStyle number_style_;
    zaf::textual::TextStyle keyword_style_;

    zaf::textual::StyledText result_;
    std::size_t deep_{};
};


class JSONPrimitiveFormatter : public JSONFormatter {
public:
    std::string Format(const boost::json::value& value);

protected:
    void OutputNull(std::string_view text) override;
    void OutputBool(std::string_view text) override;
    void OutputInteger(std::string_view text) override;
    void OutputDouble(std::string_view text) override;
    void OutputString(std::string_view text) override;
    void OutputArrayBegin(std::string_view text) override;
    void OutputArrayEnd(std::string_view text) override;
    void OutputObjectBegin(std::string_view text) override;
    void OutputObjectEnd(std::string_view text) override;
    void OutputKey(std::string_view text) override;
    void OutputColon(std::string_view text) override;
    void OutputComma(std::string_view text) override;

private:
    std::string result_;
};

}