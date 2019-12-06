#include <sstream>

//===========IndexTupleBuilder==============

template<int... Indexes>
struct IndexesTuple {
};

template<int Num, typename Tp = IndexesTuple<>>
struct IndexTupleBuilder;

template<int Num, int... Indexes>
struct IndexTupleBuilder<Num, IndexesTuple<Indexes...>>
        : IndexTupleBuilder<Num - 1, IndexesTuple<Indexes..., sizeof...(Indexes)>> {
};

template<int... Indexes>
struct IndexTupleBuilder<0, IndexesTuple<Indexes...>> {
    typedef IndexesTuple<Indexes...> value;
};

//===========convertToType====================

template<typename T, typename Array>
T convertToType(Array const &a, int i) {
    istringstream ss(a[i]);
    T temp;
    ss >> temp;
    return temp;
}

template<typename Array>
string convertToType(Array const &a, int i) {
    return a[i];
}
