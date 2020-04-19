// Status       : ACCEPTED
// Source       : https://www.spoj.com/problems/MINMOVE/en/
// DS & ALGO    : Suffix Automata

#include "bits/stdc++.h"
using namespace std;

#define ff first
#define se second
const int mx = 2e5+5;
using ii = pair<int,int>;

int sz, last;
struct state {
    int len,link,fpos;
    map<char,int>next;
} st[mx << 1];

void sa_extend(char c) {
    int cur = sz++, p = last; 
    st[cur] = {st[p].len+1, -1, st[p].len};

    for(; ~p && !st[p].next.count(c); p=st[p].link)
        st[p].next[c] = cur;
    if(p == -1) st[cur].link = 0;
    else {
        int q = st[p].next[c];
        if(st[p].len+1 == st[q].len) st[cur].link = q;
        else {
            int cln = sz++;
            st[cln] = st[q];
            st[cln].len = st[p].len+1;

            for(; ~p && st[p].next[c]==q; p=st[p].link)
                st[p].next[c] = cln;
            st[q].link = st[cur].link = cln;
        }
    }
    last = cur;
}

int smallestCyclicShift(string &s){
    int len = s.size();

    /// resetting
    st[0] = {0,-1}; sz = 1; last = 0;
    for(int i=0; i<len; i++) sa_extend(s[i]);
    for(int i=0; i<len; i++) sa_extend(s[i]);

    int u = 0;
    for(int i=0; i<len; i++)
        u = st[u].next.begin()->se;
    return st[u].fpos - len + 1; /// 0-index
}

int main(int argc, const char** argv) {

    ios_base::sync_with_stdio(0);
    cin.tie(nullptr);

    string s; cin >> s;
    cout << smallestCyclicShift(s) << "\n";
    
    return 0;
}