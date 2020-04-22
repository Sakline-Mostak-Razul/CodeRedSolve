// Status       : ACCEPTED
// Source       : http://poj.org/problem?id=2774
// DS & ALGO    : Suffix Automata

#include <map>
#include <cstdio>
#include <utility>
#include <cstring>
#define rint register int

const int mx = 2e5+5;
int len[mx], link[mx];
int sz, last, next[mx][26];

inline void sa_extend(int c) {
    int cur = sz++, p = last;
    len[cur] = len[p]+1;

    for(; ~p && (!next[p][c]); p=link[p]) next[p][c] = cur;
    if(p == -1) link[cur] = 0;
    else {
        int q = next[p][c];
        if(len[p]+1 == len[q]) link[cur] = q;
        else {
            int cln = sz++;
            len[cln] = len[p]+1;
            link[cln] = link[q];
            memcpy(next[cln], next[q], 26<<2);

            for(; ~p && next[p][c]==q; p=link[p])
                next[p][c] = cln;
            link[q] = link[cur] = cln;
        }
    }
    last = cur;
}

char s[mx]; int n;

int main(int argc, const char** argv) {

    scanf("%s",s); 
    n = strlen(s);

    sz = 1; last = 0; link[0] = -1;
    for(rint i=0; i<n; i++) 
        sa_extend(s[i] - 'a');

    scanf("%s",s); 
    n = strlen(s);

    int u=0, l=0, ans=0;
    for(rint i=0; i<n; i++){
        int c = s[i] - 'a';
        while(u && !next[u][c])
            u = link[u], l = len[u];
        if(next[u][c])
            u = next[u][c], l++;
        if(l > ans) ans = l;
    }
    printf("%d\n", ans);

    return 0;
}
