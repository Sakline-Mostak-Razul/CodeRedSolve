// Status       : ACCEPTED
// Source       : http://poj.org/problem?id=1743
// DS & ALGO    : Suffix Array

#include <cstdio>
#include <utility>
#include <cstring>
using namespace std;

const int mx = 2e4+5;
typedef std::pair<int,int> ii;

int s[mx], n;
int c[mx],cn[mx],cnt[mx];
int sa[mx],lcp[mx],pn[mx];

void build(){s[n-1] = 0;
    register int i, j, k;
    memset(cnt, 0, mx << 2);

    for(i=0; i<n; i++) cnt[ s[i] ]++;
    for(i=1; i<200; i++) cnt[i] += cnt[i-1];
    for(i=0; i<n; i++) sa[--cnt[s[i]]] = i;

    c[sa[0]] = 0; int cls = 1;
    for(i=1; i<n; c[sa[i++]]=cls-1)
        if(s[sa[i]] != s[sa[i-1]]) cls++;
    
    for(j=0,k=1; k<n; j++,k<<=1){
        for(i=0; i<n; i++){
            pn[i] = sa[i] - k;
            if(pn[i] < 0) pn[i] += n;
        }
        
        memset(cnt, 0, cls << 2);
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

inline int max(int a, int b){return a > b ? a : b;}
inline int min(int a, int b){return a > b ? b : a;}

bool can(int m){
    int lw=n, hh=0;
    for(int i=1; i<n; i++){
        if(lcp[i-1] < m)
            lw = hh = sa[i];
        else{
            lw = min(lw, sa[i]);
            hh = max(hh, sa[i]);
        }
        if(hh-lw >= m) return true;
    }
    return false;
}

int main(int argc, const char** argv) {

    while(scanf("%d", &n), n){
        for(int i=0; i<n; i++) 
            scanf("%d", &s[i]);
        for(int i=1; i<n; i++)
            s[i-1] = s[i] - s[i-1] + 100;

        build();
        buildLCP();

        int lw=0,hh=n,ans=-1;
        while(lw <= hh){
            int mid = (lw+hh)/2;
            if(can(mid))
                ans=mid, lw=mid+1;
            else hh = mid-1;
        }

        if(ans < 4) ans = -1;
        printf("%d\n", ans+1);
    }

    return 0;
}
