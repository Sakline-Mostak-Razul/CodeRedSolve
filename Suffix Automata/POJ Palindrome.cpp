// Status       : MLE
// Source       : http://poj.org/problem?id=3974
// DS & ALGO    : Suffix Automata

#include <map>
#include <vector>
#include <cstdio>
#include <cstring>

using namespace std;
const int mx = 1e6+5;
#define rint register int

char s[mx];
int sz, last;

vector<int> len;
vector<int> link;
vector< map<char,int> > nxt;

void sa_extend(char c) {
    int cur = sz++, p = last;

    link.push_back(-1);
    len.push_back(len[p]+1);
    nxt.push_back(map<char,int>());

    for(; ~p && !nxt[p][c]; p=link[p])
        nxt[p][c] = cur;
    if(p == -1) link[cur] = 0;
    else {
        int q = nxt[p][c];
        if(len[p]+1 == len[q]) link[cur] = q;
        else {
            int cln = sz++;

            nxt.push_back(nxt[q]);
            len.push_back(len[p]+1);
            link.push_back(link[q]);

            for(; ~p && nxt[p][c]==q; p=link[p])
                nxt[p][c] = cln;
            link[q] = link[cur] = cln;
        }
    }
    last = cur;
}

void build(){

    len.clear();
    nxt.clear();
    link.clear();

    len.push_back(0);
    link.push_back(-1);
    nxt.push_back(map<char,int>());

    sz = 1; last = 0;
    for(rint i=0; s[i]; i++)
        sa_extend(s[i]);
}

template<typename T>
size_t vectorsizeof(const typename std::vector<T>& vec){
    return sizeof(T) * vec.size();}

int main(int argc, const char** argv) {

    /**
        why MLE ?

        map<char,int> mp;
        for(char a='a'; a<='z'; a++)
            mp[a] = 1;

        len.assign(mx<<1, 0);
        link.assign(mx<<1, 0);
        nxt.assign(mx<<1, map<char,int>(mp));

        int tot = vectorsizeof(len);
        tot += vectorsizeof(link);
        tot += vectorsizeof(nxt);

        printf("%d B\n", tot);
        printf("%.2lf KB\n", tot/1000.0); // = 112 MB > 65 MB
        return 0;
    */

    int cs = 1;
    while(scanf("%s", s)){
        if(strcmp(s, "END") == 0) break;

        build();

        int n = strlen(s);
        int u, l, ans, pos;
        u = l = ans = pos = 0;

        for(int i=n-1; i>=0; i--){
            char c = s[i];
            while(u && !nxt[u][c])
                u = link[u], l = len[u];
            if(nxt[u][c])
                u = nxt[u][c], l++;
            if(l > ans) ans = l, pos = i;
        }

        printf("Case %d: %d\n", cs++, ans);
    }
    return 0;
}
