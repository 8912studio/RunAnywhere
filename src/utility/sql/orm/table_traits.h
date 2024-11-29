#pragma once

template<typename E>
struct sql__TableType;

namespace ra::utility::sql {

template<typename E>
using Table = typename sql__TableType<E>::type;

template<typename E>
using Table2 = decltype(typename sql__TableType<E>::Dumb());

}
