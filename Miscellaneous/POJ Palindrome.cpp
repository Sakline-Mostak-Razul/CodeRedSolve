// Status       : ACCEPTED
// Source       : http://poj.org/problem?id=3974
// DS & ALG0    : Manacher's Algo

#include <cstdio>
#include <utility>
#include <cstring>

const int mx = 1e6+5;

char s[mx]; int n;
int odd[mx], even[mx];

inline int min(int a, int b){
    return a < b ? a : b;}
inline int max(int a, int b){
    return a > b ? a : b;}

void OddManacher(){
    for(int i=0,l=0,r=-1; i<n; i++){
        int k = (i>r)?1:min(odd[l+r-i], r-i+1);
        while(0<=i-k && i+k<n && s[i-k]==s[i+k])
            k++;
        odd[i] = k--; if(i+k>r) l=i-k, r=i+k;
    }
}

void EvenManacher(){
    for(int i=0,l=0,r=-1; i<n; i++){
        int k = (i>r)?0:min(even[l+r-i+1], r-i+1);
        while(0<=i-k && i+k<n && s[i-k-1]==s[i+k])
            k++;
        even[i] = k--; if(i+k>r) l=i-k-1, r=i+k;
    }
}

int main(){
    int cs = 1;
    while(scanf("%s", s)){
        if(strcmp(s,"END") == 0) break;

        n = strlen(s);
        OddManacher();
        EvenManacher();

        int ans = 0;
        for(int i=0; i<n; i++)
            ans = max(ans, odd[i]*2 - 1);
        for(int i=0; i<n; i++)
            ans = max(ans, even[i]*2);
        printf("Case %d: %d\n", cs++, ans);
    }

    return 0;
}
