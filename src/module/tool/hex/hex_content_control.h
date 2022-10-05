#pragma once

#include <zaf/control/control.h>
#include <zaf/graphic/text/text_format.h>

namespace ra::module::tool::hex {

class HexContentControl : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetContent(const std::vector<std::byte>& content);

protected:
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) override;
    void ReleaseRendererResources() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    void PrepareGraphicResources(zaf::Renderer& renderer);

    void PaintLineHeader(
        zaf::Canvas& canvas, 
        std::size_t line_index);

    void PaintByteHex(
        zaf::Canvas& canvas,
        std::byte byte,
        std::size_t line_index,
        std::size_t byte_index_in_line);

    zaf::TextLayout GetByteHexTextLayout(std::byte byte);

    void PaintByteCharacter(
        zaf::Canvas& canvas,
        std::byte byte,
        std::size_t line_index,
        std::size_t byte_index_in_line);

    zaf::TextLayout GetByteCharacterTextLayout(wchar_t character);

private:
    std::vector<std::byte> content_;

    std::map<std::byte, zaf::TextLayout> byte_hex_text_layouts_;
    std::map<wchar_t, zaf::TextLayout> byte_character_text_layouts_;
    zaf::Brush default_text_brush_;
    zaf::Brush blank_character_brush_;
    zaf::Brush unknown_character_brush_;
};

}