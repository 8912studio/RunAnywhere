#pragma once

#include <variant>

namespace ra::mod::ai::gpt {

template<int>
class SingleID {
public:
    SingleID() = default;

    explicit SingleID(std::uint64_t value) : value_(value) {

    }

    std::uint64_t Value() const {
        return value_;
    }

    friend auto operator<=>(SingleID id1, SingleID id2) = default;

private:
    std::uint64_t value_{};
};

using DialogTransientID = SingleID<__LINE__>;
using DialogPermanentID = SingleID<__LINE__>;

using RoundTransientID = SingleID<__LINE__>;
using RoundPermanentID = SingleID<__LINE__>;


template<typename TID, typename PID>
class ComplexID {
public:
    explicit ComplexID(TID transient_id) : id_(transient_id) {

    }

    explicit ComplexID(PID permanent_id) : id_(permanent_id) {

    }

    const TID* TransientID() const {
        return std::get_if<TID>(&id_);
    }

    const PID* PermanentID() const {
        return std::get_if<PID>(&id_);
    }

    friend auto operator<=>(const ComplexID& id1, const ComplexID& id2) = default;

private:
    std::variant<TID, PID> id_;
};

using DialogID = ComplexID<DialogTransientID, DialogPermanentID>;
using RoundID = ComplexID<RoundTransientID, RoundPermanentID>;

}