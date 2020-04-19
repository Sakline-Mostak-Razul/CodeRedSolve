// Status       : ACCEPTED
// Source       : https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3358
// DS & ALGO    : Suffix Automata

#include "bits/stdc++.h"
using namespace std;
#define rint register int

#define ff first
#define se second
const int mx = 5e4+5;
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

vector<int> vec[mx];
void build(string &s){
    for(rint i=0; i<sz; i++) 
        cnt[i]=0, st[i].next.clear();

    st[0] = {0,-1}; sz = 1; last = 0;
    for(rint i=0; i<s.size(); i++) 
        sa_extend(s[i]);
}

void countHelper(int n){
    for(rint i=sz-1; i>0; i--)
        vec[ st[i].len ].push_back(i);
    for(rint i=n; i>0; i--){
        for(int p : vec[i]){
            if(st[p].link == -1) continue;
            cnt[st[p].link] += cnt[p];
            st[st[p].link].fpos = max(
                st[p].fpos,
                st[ st[p].link ].fpos
            );
        }
        vec[i].clear();
    }
}

/*
3
baaaababababbababbab
11
baaaababababbababbab
3
cccccc
0
*/

int main(int argc, const char** argv) {

    cin.tie( nullptr );
    ios_base::sync_with_stdio(false);

    int m; string s;
    while(cin >> m, m){
        cin >> s; 
        
        build(s); 
        countHelper(s.size());
        
        int ans = 0, pos = 0;
        for(int i=1; i<sz; i++)
            if(cnt[i]>=m)
                ans = max(ans, st[i].len);
        for(int i=1; i<sz; i++)
            if(cnt[i]>=m && ans==st[i].len)
                pos = max(pos, st[i].fpos-ans);

        if(ans == 0) cout << "none\n";
        else cout << ans << ' ' << pos << '\n';
    }

    return 0;
}