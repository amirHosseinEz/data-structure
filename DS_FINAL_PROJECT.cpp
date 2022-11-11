//accepted by https://codeforces.com/contest/20/problem/C  (This question is a bit different and I changed my code a bit for this question
//															The bottom line is the link to my code with a slight change that has been accepted )
//my submission => https://codeforces.com/problemset/submission/20/164235623
#define ff first
#define ss second
#include<bits/stdc++.h>
using namespace std;
const int maxn = 1e5 + 1, maxd =210;
const long long inf = 1e15 + 21;
vector <pair<long long, long long>> g[maxn];
vector <long long> ans;
long long dis[maxn];
pair <long long, int> h[maxn][maxd]; // heap
int par[maxn], loc[maxn], c = 1;
bool mark[maxn];
void bu2(int j, int i, int d){   // balance up in binary heap
	while(h[j][i / 2].ff > h[j][i].ff && i > 1){
		int ind1 = h[j][i].ss;
		int ind2 = h[j][i/2].ss;
		if(ind1 == 0)
			ind1 = d;
		if(ind2 == 0)
			ind2 = d;
		swap(loc[j*d+ind1], loc[j*d+ind2]);
		swap(h[j][i], h[j][i / 2]);
		i /= 2;
	}
}
void bd2(int j, int i, int d){ // balance down in banary heap
	while(h[j][i].ff > min(h[j][i * 2].ff, h[j][i * 2 + 1].ff)){
		if(h[j][i * 2].ff == inf)
			break;
		else if(h[j][i * 2 + 1].ff == inf){
			if(h[j][i * 2].ff < h[j][i].ff){
				int ind1 = h[j][i].ss;
				int ind2 = h[j][i*2].ss;
				if(ind1 == 0)
					ind1 = d;
				if(ind2 == 0)
					ind2 = d;
				swap(loc[j*d+ind1], loc[j*d+ind2]);
				swap(h[j][i * 2], h[j][i]);
				i *= 2;
			}
			else
				break;
		}

		else if(h[j][i * 2].ff ==  min(h[j][i * 2].ff, h[j][i * 2 + 1].ff)){
			int ind1 = h[j][i].ss;
			int ind2 = h[j][i*2].ss;
			if(ind1 == 0)
				ind1 = d;
			if(ind2 == 0)
				ind2 = d;
			swap(loc[j*d+ind1], loc[j*d+ind2]);
			swap(h[j][i], h[j][i * 2]);
			i *= 2;
		}
		else{
			int ind1 = h[j][i].ss;
			int ind2 = h[j][i*2+1].ss;
			if(ind1 == 0)
				ind1 = d;
			if(ind2 == 0)
				ind2 = d;
			swap(loc[j*d+ind1], loc[j*d+ind2]);
			swap(h[j][i], h[j][i * 2 + 1]);
			i = i * 2 + 1;
		}
	}	
}
void bu(int p, int d){ // balance up in d-arr heap
	int i = p;
	if(i){
		int ind = i % d;
		if(ind == 0)
			ind = d;
		if(i != 0){
			h[(i-1)/d][ind] = {h[i][0].ff, ind};
			loc[i] = ind;
		}
	}
	int tmp2;
	bool flag = 0;
	while(h[(i-1)/d][0].ff > h[i][0].ff && i > 0){
		flag = 1;
		int ind = loc[i];
		int tmp = loc[(i-1)/d];
		tmp2 = tmp;
		swap(h[i][0], h[(i - 1) / d][0]);
		h[(i-1)/d][ind].ff = h[i][0].ff;
		bu2((i-1)/d, ind, d);
		if((i-1)/d){
			h[(((i-1)/d)-1)/d][tmp].ff = h[(i-1)/d][0].ff;
		}

		i =(i-1)/ d;
	}
	if(i != 0 && flag)
		bu2((i-1)/d, tmp2, d);

	if(!flag){
	int ind = p % d;
	if(ind == 0)
		ind = d;
	bu2((p-1)/d, ind, d);
	}

}

void bd(int i, int d){ // balance down in d-arr heap
	bool flag = 0;
	int count = 1;
	while(h[i][0].ff > h[i][1].ff){
		flag = 1;
		int ind = h[i][1].ss;
		ind %= d;
		if(ind == 0)
			ind = d;

		h[i][1].ff = h[i][0].ff;
		swap(h[i][0], h[d*i+ind][0]);
		if(i != 0){
			h[(i-1)/d][1].ff = h[i][0].ff;
			bd2((i-1)/d, 1, d);
		}
		i = i * d + ind;
	}
	if(flag)
		bd2((i - 1)/d, 1, d);	
}
int main(){
	int n, m, beg;
	cin >> n >> m >> beg;
	int d = m / n;
	if(d < 2)
		d = 2;
	for(int i = 0; i < m; i++){
		int v, u, w;
		cin >> v >> u >> w;
		g[v].push_back({w, u});
	}
	for(int i = 0; i < maxn; i++){
		dis[i] = inf;
		loc[i] = -1;
		for(int j = 0; j < maxd; j++){
			h[i][j].first = inf;
		}
	}
	dis[beg] = 0;
	h[0][0] = {0, beg};
	while(h[0][0].first != inf){
		int v = h[0][0].second;
		int k = loc[c-1];
		swap(h[0][0], h[c - 1][0]);
		h[c - 1][0].first = inf;
		if(c - 1 != 0){
			int ind = (c - 1)%d;
			if(ind == 0)
				ind = d;
			int ind1 = c - 1;
			int ind2 = ((c-2)/d)*d + h[(c-2)/d][ind].ss;
			h[(c-2)/d][k].ff =inf;
			swap(loc[ind2], loc[ind1]);
			swap(h[(c-2)/d][ind], h[(c-2)/d][k]);
			loc[c-1] = -1;
			bd2((c-2)/d, k, d);
			bu2((c-2)/d, k, d);
		}

		bd(0, d);
		c--;
		if(mark[v] == 0){
			for(auto x: g[v]){
				int w = x.first, u = x.second;
				if(dis[u] > dis[v] + w){
					dis[u] = dis[v] + w;
					
					h[c][0] = {dis[u], u};
					bu(c, d);
					c++;
					par[u] = v;
				}		
			}
		}
		mark[v] = 1;	
	}

	for(int i = 0; i <	n ;i++){
		cout << i << " ";
		if(dis[i] == inf)
			cout << "-1\n";
		else
			cout << dis[i] << "\n";
	}
}
