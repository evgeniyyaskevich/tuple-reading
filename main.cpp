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

    IteratorRecord curNode;
    stack<IteratorRecord> st;
    curNode.level = curNode.treeNumber = 0;
    FileReader<recordSubType, keySubType> fileReader("../input.txt", ' ');
    Record<recordSubType, keySubType> z = fileReader.readLine(), zBuf = z;
    int maxLevel = 4;

    int k = 0;
    while (true) {
        while (curNode.level <= maxLevel) {
            curNode.value = z;
            st.push(curNode);
            ++curNode.level;
            curNode.treeNumber = 1;
        }

        if (st.empty()) {
            break;
        }

        curNode = st.top();
        st.pop();
        if (curNode.level == maxLevel) {
            //print info about the leaf
            cout << "Leaf: " << curNode << endl;
            zBuf = fileReader.readLine();
            k = 1;
            customCompare(z.fields, zBuf.fields, k);

            if (k == maxLevel) {
                z = zBuf;
                ++curNode.treeNumber;
            } else {
                curNode.level = maxLevel + 1;
            }
        } else {
            //print info about the inner node
            cout << "Inner node: " << "Level=" << curNode.level << " treeNumber=" << curNode.treeNumber << endl;
            if (curNode.level == k) {
                z = zBuf;
                ++curNode.treeNumber;
            } else {
                curNode.level = maxLevel + 1;
            }
        }
    }

    return 0;
}