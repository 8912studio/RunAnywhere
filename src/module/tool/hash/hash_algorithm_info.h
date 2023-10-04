#pragma once

#include <functional>
#include <zaf/crypto/hash_algorithm.h>
#include <zaf/graphic/color.h>

namespace ra::mod::tool::hash {

using HashAlgorithmCreator = std::function<zaf::crypto::HashAlgorithm()>;

class HashAlgorithmInfo {
public:
    HashAlgorithmCreator algorithm_creator;
    std::wstring displayed_name;
    std::wstring tooltip;
};

}