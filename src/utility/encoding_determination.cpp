#include "utility/encoding_determination.h"

namespace ra::utility {

bool IsUTF8Encoded(const std::vector<std::byte>& data) {

    if (data.empty()) {
        return false;
    }

    std::size_t byte_count{};
    for (auto byte : data) {
        auto ch = static_cast<std::uint8_t>(byte);

        //This is the first byte.
        if (byte_count == 0) {

            //0xxxxxxx
            if ((ch >> 7) == 0b0) {
                byte_count = 0;
            }
            //110xxxxx
            else if ((ch >> 5) == 0b110) {
                byte_count = 1;
            }
            //1110xxxx
            else if ((ch >> 4) == 0b1110) {
                byte_count = 2;
            }
            //11110xxx
            else if ((ch >> 3) == 0b11110) {
                byte_count = 3;
            }
            else {
                return false;
            }
        }
        //This is the following byte.
        else {

            if ((ch >> 6) != 0b10) {
                return false;
            }

            --byte_count;
        }
    }

    return byte_count == 0;
}


bool IsUTF16Encoded(const std::vector<std::byte>& data) {
    return false;
}

}