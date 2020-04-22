// Status       : TLE
// Source       : http://poj.org/problem?id=3974
// DS & ALG0    : Suffix Array

/**
    Why TLE ?
    Complexity Testcase : O(n log n)
    n = 1e6 + 1e6 = 2e6
    log n = log 2e6 = 21
    n log n = 21 * 2e6 = 4e7

    For 30 Testcases = 30 * 4e7
                     = 1.2e9 = too much
*/

#include <cstdio>
#include <utility>
#include <cstring>

const int mx = 2e6+5;
typedef std::pair<int,int> ii;

char s[mx]; int n, m;
int sa[mx],lcp[mx],pn[mx];
int c[mx], cn[mx], cnt[mx];

inline void build(){
    register int i, j, k;
    memset(cnt, 0, mx << 2);
    for(i=0; i<n; i++) cnt[ s[i] ]++;
    for(i=1; i<256; i++) cnt[i] += cnt[i-1];
    for(i=0; i<n; i++) sa[--cnt[s[i]]] = i;

    c[sa[0]] = 0; int cls = 1;
    for(i=1; i<n; c[sa[i++]]=cls-1)
        if(s[sa[i]] != s[sa[i-1]]) cls++;

    for(j=0,k=1; k<n; j++,k<<=1){
        for(i=0; i<n; i++){
            pn[i] = sa[i] - k;
            if(pn[i] < 0) pn[i] += n;
        }

        memset(cnt, 0, cls<<2);
        for(i=0; i<n; i++) cnt[c[pn[i]]]++;
        for(i=1; i<cls; i++) cnt[i] += cnt[i-1];
        for(i=n-1; ~i; i--) sa[--cnt[c[pn[i]]]] = pn[i];

        cn[sa[0]] = 0; cls = 1;
        for(i=1; i<n; cn[sa[i++]]=cls-1){
            ii cur = ii(c[sa[i]],   c[(sa[i]   + k)%n]);
            ii pre = ii(c[sa[i-1]], c[(sa[i-1] + k)%n]);
            if(cur != pre) cls++;
        }
        for(int i=0; i<n; i++) c[i] = cn[i];
    }
}

inline void buildLCP(){
    memset(lcp, 0, mx<<2);
    register int i, j = 0;
    for(int i=0; i<n; i++) pn[sa[i]] = i;
    for(int i=0; i<n; i++){
        if(pn[i]==n-1){j=0; continue;}
        int k = sa[pn[i] + 1];
        while(i+j<n && j+k<n && s[i+j]==s[j+k]) j++;
        lcp[ pn[i] ] = j; if( j ) j--;
    }
}

inline int id(int x){return x>=m;}

inline int max(int a, int b){
    return a>b?a:b;}

int main(){
    int cs = 1;
    while(scanf("%s", s)){
        if(strcmp(s,"END") == 0) break;

        m = n = strlen(s);
        for(int i=0,j=n-1; i<n; i++,j--)
            s[i+n+1] = s[j];
        s[n]='#'; n+=n+1; s[n++]='$';

        build(); buildLCP();
        int ans = 0;
        for(int i=1; i<n; i++)
            if(id(sa[i-1]) != id(sa[i]))
                ans = max(ans, lcp[i-1]);
        printf("Case %d: %d\n", cs++, ans);
    }

    return 0;
}
