#include "utility/path_trimming.h"

namespace ra::utility {

zaf::dwrite::TextTrimming CreateTextTrimmingForPath() {

    zaf::dwrite::TextTrimming text_trimming;
    text_trimming.SetGranularity(zaf::dwrite::TextTrimmingGranularity::Character);
    text_trimming.SetDelimiter(L'\\');
    text_trimming.SetDelimiterCount(2);
    return text_trimming;
}

}