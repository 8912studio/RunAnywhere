#pragma once

#include <functional>
#include <zaf/crypto/hash_algorithm.h>

namespace ra::mod::tool::hash {

using HashAlgorithmCreator = std::function<zaf::crypto::HashAlgorithm()>;

}