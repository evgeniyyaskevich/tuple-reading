#include "FileRecord.h"
#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
#include <sstream>

using namespace std;

int compare(string left, string right) {
    istringstream leftInput(left);
    istringstream rightInput(right);

    int k = 0;
    while (leftInput && rightInput) {
        leftInput >> left;
        rightInput >> right;
        if (left == right) {
            ++k;
        } else {
            break;
        }
    }

    return k;
}

int main() {

    int levelDifference = 0;
    customCompare(make_tuple(1, 1, 3), make_tuple(1, 1, 4), levelDifference);

    typedef tuple<int, double, int, string> recordType;
    typedef key<0, 1, 2> keyType;

    Record<recordType, keyType> record;
    auto x = record.key();

    ifstream file("../input.txt");

    FileReader<recordType, keyType> fileReader("../input.txt");
    auto line = fileReader.readLine();
    auto keys = line.key();
    auto keys1 = fileReader.readLine().key();
    cout << (keys < keys1);

    struct Record {
        int n = 0;
        int nt = 0;
        string rz;
    };

    int nMax = 4;
    stack<Record> st;

    Record q;
    q.n = q.nt = 0;
    string z, zBuf;
    getline(file, z);
    int k = 0;
    while (true) {
        while (q.n <= nMax) {
            st.push(q);
            ++q.n;
            q.nt = 1;
            q.rz = z;
        }

        if (st.empty()) {
            break;
        }

        q = st.top();
        st.pop();
        if (q.n == nMax) {
            //print info about the leaf
            cout << "Leaf with z = " + q.rz << endl;
            getline(file, zBuf);
            k = compare(z, zBuf);

            if (k == nMax) {
                z = zBuf;
                ++q.nt;
            } else {
                q.n = nMax + 1;
            }
        } else {
            //print info about the inner node
            cout << "inner node" << endl;
            if (q.n == k) {
                z = zBuf;
                ++q.nt;
            } else {
                q.n = nMax + 1;
            }
        }
    }

    return 0;
}