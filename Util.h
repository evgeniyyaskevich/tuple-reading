#ifndef UTIL_H_
#define UTIL_H_

#include <tuple>
#include <sstream>

using namespace std;
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

//============ Tuple comparing =============

template<bool flag, size_t N1, size_t N2>
struct Selector {
    enum {
        maxValue = N1
    };
};

template<size_t N1, size_t N2>
struct Selector<false, N1, N2> {
    enum {
        maxValue = N2
    };
};

template<typename Tuple1, typename Tuple2, size_t N>
struct ComparingHelper {
    static int compare(Tuple1 const &t1, Tuple2 const &t2) {
        int result = ComparingHelper<Tuple1, Tuple2, N - 1>::compare(t1, t2);
        if (result == 0) {
            if (get<N - 1>(t1) > get<N - 1>(t2)) {
                result = 1;
            } else if (get<N - 1>(t1) < get<N - 1>(t2)) {
                result = -1;
            } else {
                result = 0;
            }
        }
        return result;
    }

    static int customCompare(Tuple1 const &t1, Tuple2 const &t2, int &levelDifference) {
        int isCompareNext = ComparingHelper<Tuple1, Tuple2, N - 1>::customCompare(t1, t2, levelDifference);
        if (isCompareNext) {
            if (get<N - 1>(t1) == get<N - 1>(t2)) {
                ++levelDifference;
                return true;
            }
        }
        return false;
    }
};

template<typename Tuple1, typename Tuple2>
struct ComparingHelper<Tuple1, Tuple2, 1> {
    static int compare(Tuple1 const &t1, Tuple2 const &t2) {
        if (get<0>(t1) > get<0>(t2)) {
            return 1;
        } else if (get<0>(t1) < get<0>(t2)) {
            return -1;
        } else {
            return 0;
        }
    }

    static int customCompare(Tuple1 const &t1, Tuple2 const &t2, int &levelDifference) {
        if (get<0>(t1) == get<0>(t2)) {
            levelDifference++;
            return true;
        }
        return false;
    }
};

template<typename... Args1, typename... Args2>
int compare(tuple<Args1...> const &t1,
            tuple<Args2...> const &t2) {
    return
            ComparingHelper<decltype(t1), decltype(t2),
                    Selector<sizeof...(Args1) < sizeof...(Args2),
                            sizeof...(Args1), sizeof...(Args2)>::maxValue>::compare(t1, t2);
}

template<typename... Args1, typename... Args2>
int customCompare(tuple<Args1...> const &t1,
                  tuple<Args2...> const &t2,
                  int &levelDifference) {
    return
            ComparingHelper<decltype(t1), decltype(t2),
                    Selector<sizeof...(Args1) < sizeof...(Args2),
                            sizeof...(Args1), sizeof...(Args2)>::maxValue>::customCompare(t1, t2, levelDifference);
}



//======== Tuple Writer ============

template<class Tuple, size_t N>
struct TupleWriter {
    static void write(ostream& os, Tuple const& tuple) {
        TupleWriter<Tuple, N - 1>::write(os, tuple);
        os << " " << get<N - 1>(tuple);
    }
};

template<class Tuple>
struct TupleWriter<Tuple, 1> {
    static void write(ostream& os, Tuple const& tuple) {
        os << get<0>(tuple);
    }
};

#endif