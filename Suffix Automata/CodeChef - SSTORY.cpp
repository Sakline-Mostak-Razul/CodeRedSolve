// Status       : ACCEPTED
// Source       : https://www.codechef.com/problems/SSTORY
// DS & ALGO    : Suffix Automata

#include "bits/stdc++.h"
using namespace std;

#define ff first
#define se second
const int mx = 5e5+5;
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

void build(string &s){
    for(int i=0; i<sz; i++) st[i].next.clear();
    st[0] = {0,-1}; sz = 1; last = 0;
    for(int i=0; i<s.size(); i++) sa_extend(s[i]);
}

int main(int argc, const char** argv) {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, p; cin >> s >> p; build(s);

    int u, l, ans, pos;
    u = l = ans = pos = 0;
    for(int i=0; i<p.size(); i++){
        while(u && !st[u].next.count(p[i]))
            u = st[u].link, l = st[u].len;
        if(st[u].next.count(p[i]))
            u = st[u].next[p[i]], l++;
        if(l > ans) ans = l, pos = i;
    }

    if(ans == 0)
        return cout << "0\n", 0;

    u = l = 0;
    for(int i=0; i<p.size(); i++){
        while(u && !st[u].next.count(p[i]))
            u = st[u].link, l = st[u].len;
        if(st[u].next.count(p[i]))
            u = st[u].next[p[i]], l++;
        if(l == ans) {pos = i; break;}
    }

    cout << p.substr(pos-ans+1, ans) << "\n";
    cout << ans << "\n";

    return 0;
}