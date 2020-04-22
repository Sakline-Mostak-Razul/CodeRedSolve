// Status       : ACCEPTED
// Source       : http://poj.org/problem?id=3294
//                https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=24&page=show_problem&problem=2048
// DS & ALGO    : Suffix Array

#include <cstdio>
#include <utility>
#include <cstring>

const int mx = 1e5+105;
typedef std::pair<int,int> pii;

char s[mx],t[mx];
int vis[205], id[mx];
int n,m,ans[mx];
int c[mx],cn[mx],cnt[mx];
int sa[mx],lcp[mx],pn[mx];

void build(){
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
            pii cur = pii(c[sa[i]],   c[(sa[i]   + k)%n]);
            pii pre = pii(c[sa[i-1]], c[(sa[i-1] + k)%n]);
            if(cur != pre) cls++; 
        }
        for(int i=0; i<n; i++) c[i] = cn[i];
    }
}

void buildLCP(){
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

int cvis = 0, sz = 1;
inline void add(int x){
    int idx = id[x];
    if(idx && !vis[idx])
        vis[idx] = 1, cvis++;
}

inline bool can(int x){
    cvis = 0; sz = 1;
    memset(vis,0,sizeof vis);

    for(int i=0; i<n-1; i++){
        if(lcp[i] >= x){
            add(sa[i]);
            add(sa[i+1]);
        }
        else{
            if(cvis > m/2) ans[sz++] = sa[i];
            cvis = 0; memset(vis, 0, sizeof vis);
        }
    }
    if(cvis > m/2) 
        ans[sz++] = sa[n-1];
    ans[0] = sz;
    return sz > 1;
}

int main(int argc, const char** argv) {
    int flag = 0;
    while(scanf("%d", &m), m){
        if(flag) puts(""); flag=1;

        strcpy(s, ""); n = 0;
        for(int i=1; i<=m; i++){
            scanf("%s", t+n);
            while(t[n]){
                s[n]=t[n];
                id[n++]=i;
            }
            s[n] = i; 
            id[n++] = 0;
        }
        build(); buildLCP();

        int lw = 1, hh = n, res = 0;
        while(lw <= hh){
            int mid = (lw + hh) / 2;
            if(can(mid))
                res = mid, lw = mid + 1;
            else hh = mid - 1;
        }

        if(res == 0) puts("?");
        else{
            can(res);
            for(int i=1; i<ans[0]; i++){
                strncpy(t, s+ans[i], res);
                t[res] = 0;
                puts(t);
            }
        }
    }

    return 0;
}
