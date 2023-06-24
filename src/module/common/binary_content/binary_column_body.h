#pragma once

#include <zaf/control/control.h>
#include <zaf/graphic/text/text_format.h>
#include "module/common/binary_content/byte_index.h"

namespace ra::mod {

class BinaryColumnBody : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetBinary(std::vector<std::byte> binary);

protected:
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) override;
    void ReleaseRendererResources() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

    void OnMouseMove(const zaf::MouseMoveInfo& event_info) override;
    void OnMouseLeave(const zaf::MouseLeaveInfo& event_info) override;

    void OnMouseDown(const zaf::MouseDownInfo& event_info) override;
    void OnMouseUp(const zaf::MouseUpInfo& event_info) override;

private:
    class SelectionInfo {
    public:
        const ByteIndex& MinByteIndex() const {
            return begin_byte_index < end_byte_index ? begin_byte_index : end_byte_index;
        }

        const ByteIndex& MaxByteIndex() const {
            return begin_byte_index < end_byte_index ? end_byte_index : begin_byte_index;
        }

    public:
        ByteIndex begin_byte_index;
        ByteIndex end_byte_index;
        bool is_in_character_area{};
    };

private:
    static std::optional<ByteIndex> FindByteIndex(
        const zaf::Point& position,
        bool adjust_to_nearest_index,
        bool& is_in_character_area);

    static std::optional<std::size_t> FindByteIndexInLine(
        float x,
        bool adjust_to_nearest_index,
        bool& is_in_character_area);

    static std::optional<std::size_t> FindByteLine(float y, bool adjust_to_nearest_index);

private:
    void PrepareGraphicResources(zaf::Renderer& renderer);

    void PaintLineHeader(zaf::Canvas& canvas, std::size_t line_index);

    void PaintByteHex(zaf::Canvas& canvas, const ByteIndex& byte_index);
    zaf::TextLayout GetByteHexTextLayout(std::byte byte);

    void PaintByteCharacter(zaf::Canvas& canvas, const ByteIndex& byte_index);
    zaf::TextLayout GetByteCharacterTextLayout(wchar_t character);

    bool IsByteSelected(const ByteIndex& byte_index) const;

    void HandleMouseMove(const zaf::Point& position);
    std::vector<std::size_t> HandleMouseOverBytesOnMouseMove(const zaf::Point& mouse_position);
    std::vector<std::size_t> HandleSelectedBytesOnMouseMove(const zaf::Point& mouse_position);
    void HandleMouseDown(const zaf::Point& position, const zaf::MouseMessage& message);
    void HandleMouseUp(const zaf::Point& position);

    std::vector<std::size_t> GetSelectedLines() const;
    void RepaintLines(const std::vector<std::size_t>& lines);

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
    std::optional<SelectionInfo> selection_info_;
};

}