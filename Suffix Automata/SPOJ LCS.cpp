// Status       : ACCEPTED
// Source       : https://www.spoj.com/problems/LCS/
// DS & ALGO    : Suffix Automata

#include <bits/stdc++.h>
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

int cnt[mx<<1];
set < ii > base;
// number of occurence helper

void sa_extend(char c) {
    int cur = sz++, p = last; cnt[cur] = 1;

    base.insert({st[p].len+1, cur});
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

            cnt[cln] = 0;
            st[cln].len = st[p].len+1;
            base.insert({st[cln].len, cln});

            for(; ~p && st[p].next[c]==q; p=st[p].link)
                st[p].next[c] = cln;
            st[q].link = st[cur].link = cln;
        }
    }
    last = cur;
}

void build(string &s){
    st[0] = {0,-1}; sz = 1; last = 0;
    for(int i=0; i<s.size(); i++) sa_extend(s[i]);
    for(auto it=base.rbegin(); it!=base.rend(); it++)
        cnt[st[it->se].link] += cnt[it->se];
}

int lcs(string &s, string &p){
    build(s);
    int u, l, ans, pos;
    u = l = ans = pos = 0;
    for(int i=0; i<p.size(); i++){
        while(u && !st[u].next.count(p[i]))
            u = st[u].link, l = st[u].len;
        if(st[u].next.count(p[i]))
            u = st[u].next[p[i]], l++;
        if(l > ans) ans = l, pos = i;
    }
    return ans;
}

int main(int argc, char const *argv[]){
    
    ios_base :: sync_with_stdio(false);
    cin.tie ( nullptr );

    string s, t; cin >> s >> t;
    cout << lcs(s, t) << "\n";

    return 0;
}
