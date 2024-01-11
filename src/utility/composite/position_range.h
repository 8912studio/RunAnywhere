#pragma once

#include <zaf/graphic/point.h>

namespace ra::utility::composite {

class PositionRange {
public:
    PositionRange(const zaf::Point& begin, const zaf::Point& end) :
        begin_(begin), 
        end_(end) {

    }

    const zaf::Point& Begin() const {
        return begin_;
    }

    const zaf::Point& End() const {
        return end_;
    }

    std::pair<zaf::Point, zaf::Point> Sort() const {
        if (std::tie(begin_.y, begin_.x) < std::tie(end_.y, end_.x)) {
            return std::make_pair(begin_, end_);
        }
        else {
            return std::make_pair(end_, begin_);
        }
    }

private:
    zaf::Point begin_;
    zaf::Point end_;
};

}