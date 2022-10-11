#pragma once

#include <zaf/control/control.h>
#include <zaf/graphic/text/text_format.h>
#include "module/tool/hex/byte_index.h"

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

    bool OnMouseMove(const zaf::Point& position, const zaf::MouseMessage& message) override;
    void OnMouseLeave(const std::shared_ptr<zaf::Control>& leaved_control) override;

    bool OnMouseDown(const zaf::Point& position, const zaf::MouseMessage& message) override;
    bool OnMouseUp(const zaf::Point& position, const zaf::MouseMessage& message) override;

private:
    void PrepareGraphicResources(zaf::Renderer& renderer);

    void PaintLineHeader(zaf::Canvas& canvas, std::size_t line_index);

    void PaintByteHex(zaf::Canvas& canvas, const ByteIndex& byte_index);
    zaf::TextLayout GetByteHexTextLayout(std::byte byte);

    void PaintByteCharacter(zaf::Canvas& canvas, const ByteIndex& byte_index);
    zaf::TextLayout GetByteCharacterTextLayout(wchar_t character);

    bool IsByteSelected(const ByteIndex& byte_index) const;

    void HandleMouseMove(const zaf::Point& position);
    void HandleMouseDown(const zaf::Point& position);
    void HandleMouseUp(const zaf::Point& position);

    std::optional<ByteIndex> FindByteIndex(const zaf::Point& position) const;
    std::optional<std::size_t> FindByteIndexInLine(float x) const;

private:
    std::vector<std::byte> content_;

    std::map<std::byte, zaf::TextLayout> byte_hex_text_layouts_;
    std::map<wchar_t, zaf::TextLayout> byte_character_text_layouts_;
    zaf::Brush mouse_over_background_brush_;
    zaf::Brush selected_background_brush_;
    zaf::Brush default_text_brush_;
    zaf::Brush blank_character_brush_;
    zaf::Brush unknown_character_brush_;

    std::optional<ByteIndex> mouse_over_byte_index_;
    std::optional<ByteIndexRange> selected_byte_index_range_;
};

}