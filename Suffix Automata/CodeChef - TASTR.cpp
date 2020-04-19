// Status       : ACCEPTED
// Source       : https://www.codechef.com/problems/TASTR
// DS & ALGO    : Suffix Automata


#include "bits/stdc++.h"
using namespace std;
using ll = long long;

#define int ll
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

void build(string &s){
    for(int i=0; i<sz; i++) st[i].next.clear();
    st[0] = {0,-1}; sz = 1; last = 0;
    for(int i=0; i<s.size(); i++) sa_extend(s[i]);
}

int dss[mx<<1]; /// distinct substring
int dssFun(int u){
    if(dss[u]) return dss[u];

    dss[u] = 1;
    for(auto p : st[u].next)
        dss[u] += dssFun(p.se);
    return dss[u];
}

int fun(string s){
    build(s);
    memset(dss, 0, mx<<3);
    return dssFun(0);
}

signed main() {

    ios_base :: sync_with_stdio(0);
    cin.tie( nullptr );

    string x, y; cin >> x >> y;
    ll xl = x.size()+1, yl = y.size()+1;

    ll ansx = fun(x), ansy = fun(y);
    ll ans  = fun(x + "#" + y) - (xl * yl); 

    ans = 2*ans - ansx - ansy;
    cout << ans << "\n";

    return 0;
}