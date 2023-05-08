#include "utility/path_trimming.h"

namespace ra::utility {

zaf::TextTrimming CreateTextTrimmingForPath() {

    zaf::TextTrimming text_trimming;
    text_trimming.SetGranularity(zaf::TextTrimmingGranularity::Character);
    text_trimming.SetDelimiter(L'\\');
    text_trimming.SetDelimiterCount(2);
    return text_trimming;
}

}