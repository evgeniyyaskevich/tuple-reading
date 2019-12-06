#ifndef FILE_RECORD_H_
#define FILE_RECORD_H_

#include "Util.h"
#include <tuple>

using namespace std;

template<typename Tuple, typename Key>
class Record;

template<typename Keys>
class Key;

template<int... Nums>
struct key {
};

template<int... Indexes, typename Tuple>
decltype(auto) extractKeys(Tuple &&t) {
    return make_tuple(get<Indexes>(forward<Tuple>(t))...);
}

template<typename... Types>
class Key<tuple<Types...>> {
public:
    tuple<Types...> keys;

    explicit Key(tuple<Types...> const& _keys) : keys(_keys) {};

    template <typename KeyType>
    bool operator < (KeyType const& right) {
        return compare(keys, right.keys) < 0;
    }

    template <typename KeyType>
    bool operator > (KeyType const& right) {
        return compare(keys, right.keys) > 0;
    }

    template <typename KeyType>
    bool operator == (KeyType const& right) {
        return compare(keys, right.keys) == 0;
    }
};

template<typename... Types, int... Nums>
class Record<tuple<Types...>, key<Nums...>> {

    //TODO: think about private, protected
public:
    tuple<Types...> fields;

    Record() {}
    Record(tuple<Types...> _fields) : fields(_fields) { }

    auto key() const {
        auto keyFields = extractKeys<Nums...>(fields);
        return Key<decltype(keyFields)>(keyFields);
    }

    ~Record() { };
};

#endif