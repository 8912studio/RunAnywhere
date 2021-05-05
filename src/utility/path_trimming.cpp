#include "utility/path_trimming.h"

namespace ra::utility {

zaf::TextTrimming CreateTextTrimmingForPath() {

    zaf::TextTrimming text_trimming;
    text_trimming.granularity = zaf::TextTrimming::Granularity::Character;
    text_trimming.delimiter = L'\\';
    text_trimming.delimiter_count = 2;
    return text_trimming;
}

}