// Status       : ACCEPTED
// Source       : http://poj.org/problem?id=3261
// DS & ALGO    : Suffix Array


#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

const int mx  = 2e4+5;
const int mx2 = 1e6+5; 
typedef pair<int,int> pii;

int Min[mx][16];
int s[mx]; int n, k;
int sa[mx],lcp[mx],pn[mx];
int c[mx],cn[mx],cnt[mx2];

// constructing suffix array by sort cyclic shifts
void build(){s[n++] = 0;
    register int i, j, k;

    for(i=0; i<n; i++) cnt[ s[i] ]++;
    for(i=1; i<mx2; i++) cnt[i] += cnt[i-1];
    for(i=0; i<n; i++) sa[--cnt[s[i]]] = i;

    c[sa[0]] = 0; int cls = 1;
    for(i=1; i<n; c[sa[i++]]=cls-1)
        if(s[sa[i]] != s[sa[i-1]]) cls++;
    
    for(j=0,k=1; k<n; j++,k<<=1){
        for(i=0; i<n; i++){
            pn[i] = sa[i] - k;
            if(pn[i] < 0) pn[i] += n;
        }
        
        fill(cnt, cnt+cls, 0);
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
    register int i, j = 0;
    for(int i=0; i<n; i++) pn[sa[i]] = i;
    for(int i=0; i<n; i++){
        if(pn[i]==n-1){j=0; continue;}
        int k = sa[pn[i] + 1];
        while(i+j<n && j+k<n && s[i+j]==s[j+k]) j++;
        lcp[ pn[i] ] = j; if( j ) j--;
    }
}

int query(int l, int r){
    int k = __lg(r - l + 1);
    return min(
        Min[l][k],
        Min[r-(1<<k)+1][k]
    );
}

int main(int argc, const char** argv) {

    ios::sync_with_stdio(0);
    cin.tie( NULL );

    cin >> n >> k;
    for(int i=0; i<n; i++)
        cin >> s[i], s[i]++;
    
    build();
    buildLCP();

    for(int i=0; i<n; i++) Min[i][0] = lcp[i];
    for(int j=1; (1<<j)<n; j++)
    for(int i=0; i+(1<<j)<=n; i++)
        Min[i][j] = min(
            Min[i][j-1],
            Min[i+(1<<(j-1))][j-1]
        );

    int ans = 0;
    for(int i=0, j=k-2; j<n; i++, j++)
        ans = max(ans, query(i,j));
    cout << ans << "\n";

    return 0;
}
