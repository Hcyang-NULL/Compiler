#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main() {
    vector<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    vector<int> b;
    vector<int> c;
    b = a;
    c = a;
    b[1] = 7;
    for(int i = 0; i < c.size(); i++)
    {
        cout << c[i] << " ";
    }
    cout << endl;
    for(int i = 0; i < b.size(); i++)
    {
        cout << b[i] << " ";
    }
    cout << endl;
}