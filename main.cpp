#include "FileRecord.h"
#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
#include <sstream>

using namespace std;

typedef tuple<int, double, int, string> recordSubType;
typedef key<0, 1, 2> keySubType;

struct IteratorRecord {
    int level = 0;
    int treeNumber = 0;
    Record<recordSubType, keySubType> value{};

    friend ostream& operator<< (ostream& os, IteratorRecord const& rec) {
        os << "Level=" << rec.level << ", treeNumber=" << rec.treeNumber << ", value={" << rec.value << "}";
    }
};

int main() {

    int nMax = 4;
    stack<IteratorRecord> st;

    IteratorRecord q;
    q.level = q.treeNumber = 0;
    FileReader<recordSubType, keySubType> fileReader("../input.txt");
    Record<recordSubType, keySubType> z = fileReader.readLine(), zBuf = z;

    int k = 0;
    while (true) {
        while (q.level <= nMax) {
            q.value = z;
            st.push(q);
            ++q.level;
            q.treeNumber = 1;
        }

        if (st.empty()) {
            break;
        }

        q = st.top();
        st.pop();
        if (q.level == nMax) {
            //print info about the leaf
            cout << "Leaf: " << q << endl;
            zBuf = fileReader.readLine();
            k = 1;
            customCompare(z.fields, zBuf.fields, k);

            if (k == nMax) {
                z = zBuf;
                ++q.treeNumber;
            } else {
                q.level = nMax + 1;
            }
        } else {
            //print info about the inner node
            cout << "Inner node: " << "Level=" << q.level << " treeNumber=" << q.treeNumber << endl;
            if (q.level == k) {
                z = zBuf;
                ++q.treeNumber;
            } else {
                q.level = nMax + 1;
            }
        }
    }

    return 0;
}