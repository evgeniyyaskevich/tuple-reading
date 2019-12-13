#ifndef FILE_READER_H_
#define FILE_READER_H_

#include "Util.h"
#include <tuple>
#include <array>
#include <sstream>
#include <fstream>

template<typename Tuple, typename Key>
class FileReader;

//=========TupleReader=======================

template<typename Array, typename IndexesTuple, typename... Types>
struct TupleReader;

template<typename Array, int ... Indexes, typename... Types>
struct TupleReader<Array, IndexesTuple<Indexes...>, Types...> {

    decltype(auto) read(Array const &a, IndexesTuple<Indexes...> idx) {
        return make_tuple(convertToType<Types>(a, Indexes)...);
    }
};

template<typename Tuple, typename Array, typename Indexes>
struct TupleReaderWrapper;

template<typename... Types, typename Array, typename Indexes>
struct TupleReaderWrapper<tuple<Types...>, Array, Indexes> {

    decltype(auto) read(Array &arr, Indexes aInd) {
        TupleReader<Array, Indexes, Types...> reader;
        return reader.read(arr, aInd);
    }
};

//================ Convert string to tuple ===================

template<typename Tuple>
decltype(auto) getTuple(string const &s, char delimiter) {
    istringstream ss(s);
    const size_t n = tuple_size<Tuple>::value;
    array<string, n> arr;

    for (size_t i = 0; i < n; ++i) {
        getline(ss, arr[i], delimiter);
    }

    TupleReaderWrapper<Tuple, array<string, n>, typename IndexTupleBuilder<n>::value> caller;
    return caller.read(arr, typename IndexTupleBuilder<n>::value());
}

//================= FileReader =========================

template<typename... Types, int... Indexes>
class FileReader<tuple<Types...>, key<Indexes...>> {

    ifstream in;
    char delimiter;

public:
    typedef Record<tuple<Types...>, key<Indexes...>> RecordType;

    FileReader(string fileName, char _delimiter) : delimiter(_delimiter) {
        in.open(fileName);
    }

    RecordType readLine() {
        string si;
        getline(in, si);
        return RecordType(getTuple<tuple<Types...>>(si, delimiter));
    }

    bool hasNextLine() {
        return !in.eof();
    }

    ~FileReader() {
        in.close();
    }
};

#endif