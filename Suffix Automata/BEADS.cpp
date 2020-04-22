// Status       : ACCEPTED
// Source       : https://www.spoj.com/problems/BEADS/
// DS & ALGO    : Suffix Automata

#include "bits/stdc++.h"
using namespace std;
#define rint register int

#define ff first
#define se second
const int mx = 1e4+5;
using ii = pair<int,int>;

struct state {
    int len,link,fpos;
    map<char,int>next;
} st[mx << 1];
int sz, last, cnt[mx<<1];

void sa_extend(char c) {
    int cur = sz++, p = last; cnt[cur] = 1;
    st[cur] = {st[p].len+1, -1, st[p].len+1};

    for(; ~p && !st[p].next.count(c); p=st[p].link)
        st[p].next[c] = cur;
    if(p == -1) st[cur].link = 0;
    else {
        int q = st[p].next[c];
        if(st[p].len+1 == st[q].len) st[cur].link = q;
        else {
            int cln = sz++;
            cnt[cln] = 0;
            st[cln] = st[q];
            st[cln].len = st[p].len+1;

            for(; ~p && st[p].next[c]==q; p=st[p].link)
                st[p].next[c] = cln;
            st[q].link = st[cur].link = cln;
        }
    }
    last = cur;
}

void build(string &s){
    for(rint i=0; i<sz; i++) cnt[i]=0, st[i].next.clear();

    st[0] = {0,-1}; sz = 1; last = 0;
    for(rint i=0; i<s.size(); i++)  sa_extend(s[i]);
    for(rint i=0; i<s.size(); i++)  sa_extend(s[i]);
}

int main(int argc, char const *argv[]){
    
    ios::sync_with_stdio(0);
    cin.tie( nullptr );

    string s;
    int tc; cin >> tc;
    while(tc--){
        cin >> s; build(s);
        int u = 0;
        for(rint i=0; i<s.size(); i++)
            u = st[u].next.begin()->second;
        cout << st[u].fpos - s.size() + 1 << "\n";
    }
    return 0;
}
