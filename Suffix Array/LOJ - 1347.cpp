// Status       : ACCEPTED
// Source       : http://lightoj.com/volume_showproblem.php?problem=1347
// DS & ALGO    : Suffix Array

#include <cstdio>
#include <vector>
#include <utility>
#include <cstring>

using namespace std;
using vi = vector<int>;
using ii = pair<int,int>;
#define rint register int

const int mx = 2e4+5;

int n, n1, n2;
char s[mx], p[mx]; 
int sa[mx],lcp[mx],pn[mx];
int c[mx], cn[mx], cnt[mx];

// constructing suffix array by sort cyclic shifts
void build(){
    s[n++] = '$';
    memset(cnt, 0, mx<<2);
    register int i, j, k; int m = 256;
    
    for(i=0; i<n; i++) cnt[ s[i] ]++;
    for(i=1; i<m; i++) cnt[i] += cnt[i-1];
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
            ii cur = {c[sa[i]],   c[(sa[i]   + k)%n]};
            ii pre = {c[sa[i-1]], c[(sa[i-1] + k)%n]};
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

int id(int idx){
    if(idx > n2) return 2;
    return idx > n1;
}

bool ok(int i, int j, int k){
    int ans0 = (i==0) || (j==0) || (k==0);
    int ans1 = (i==1) || (j==1) || (k==1);
    int ans2 = (i==2) || (j==2) || (k==2);
    return ans0 + ans1 + ans2 > 2;
}

void __print(){
    puts(s);
    for(rint i=0; i<n; i++)
        printf("%3d %3d %3d %d %s\n", i, sa[i], lcp[i], id(sa[i]), s+sa[i]);
}

int main(int argc, const char** argv) {

    int tc, ans; scanf("%d", &tc);
    for(rint cs=1; cs<=tc; cs++){

        scanf("%s", s);
        n1 = strlen(s);
        strcat(s, "@");

        scanf("%s", p);
        strcat(s, p);
        n2 = strlen(s);
        strcat(s, "#");

        scanf("%s", p);
        strcat(s, p);
        n = strlen(s);

        build();
        buildLCP();
        // __print();

        ans = 0;
        for(int i=3; i<n-2; i++)
            if(ok(id(sa[i]), id(sa[i+1]), id(sa[i+2])))
                ans = max(ans, min(lcp[i], lcp[i+1]));

        printf("Case %d: %d\n", cs, ans);
    }

    return 0;
}