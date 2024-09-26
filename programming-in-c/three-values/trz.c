#include <stdio.h>
#include <stdlib.h>

typedef struct pair{
	int type, pos;
} pair;

int min(int x, int y){
	return (x < y ? x : y);
}

int max(int x, int y){
	return (x > y ? x : y);
}

int main()
{
	int n;
	if(scanf("%d", &n)){};
	int* arr = (int*) malloc(sizeof(int) * (size_t)(n + 2));
	int* dist = (int*) malloc(sizeof(int) * (size_t)(n + 2));
	for(int i = 1; i <= n; ++i) 
		if(scanf("%d%d", &arr[i], &dist[i])){};
	
	// l[i]/r[i] holds three closest/furthest positions of distinct types of motels left/right to the i-th motel
	pair** l = (pair**) malloc(sizeof(pair*) * 3);
	pair** r = (pair**) malloc(sizeof(pair*) * 3);
	for(int i = 0; i < 3; ++i){
		l[i] = (pair*) malloc(sizeof(pair) * (size_t)(n + 2));
		r[i] = (pair*) malloc(sizeof(pair) * (size_t)(n + 2));
	}
	
	// >>>> CLOSEST TRIPLE <<<<
	
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j <= n + 1; ++j) l[i][j] = r[i][j] = (pair){-1, -1};
	}
	
	// finding closest motels on the left
	for(int i = 1; i <= n; ++i){
		int changed = 0;
		int smallest = 0;
		for(int j = 0; j < 3; ++j) l[j][i]=l[j][i - 1];
		for(int j = 0; j < 3; ++j){
			if(l[j][i - 1].type == arr[i]){
				l[j][i].pos = dist[i];
				changed = 1;
			}
			if(l[j][i - 1].type == -1 && !changed){
				l[j][i] = (pair){arr[i], dist[i]};
				changed = 1;
				break;
			}
			if(l[smallest][i - 1].pos > l[j][i - 1].pos) smallest = j;
		}
		if(!changed){
			l[smallest][i] = (pair){arr[i], dist[i]};
		}
	}
	
	// finding closest motels on the right
	for(int i = n; i >= 1; --i){
		int changed = 0;
		int biggest = 0;
		for(int j = 0; j < 3; ++j) r[j][i]=r[j][i + 1];
		for(int j = 0; j < 3; ++j){
			if(r[j][i + 1].type == arr[i]){
				r[j][i].pos = dist[i];
				changed = 1;
			}
			if(r[j][i + 1].type == -1 && !changed){
				r[j][i] = (pair){arr[i], dist[i]};
				changed = 1;
				break;
			}
			if(r[biggest][i + 1].pos < r[j][i + 1].pos) biggest = j;
		}
		if(!changed){
			r[biggest][i] = (pair){arr[i], dist[i]};
		}
	}
	
	// looking for closest triple of distinct motels
	int min_dist = -1;
	for(int i = 1; i <= n; ++i){
		for(int a = 0; a < 3; ++a){
			if(l[a][i].type == -1 || l[a][i].type == arr[i]) continue;
			for(int b = 0; b < 3; ++b){
				if(r[b][i].type == -1 || r[b][i].type == arr[i]) continue;
				if(l[a][i].type != r[b][i].type){
					if(min_dist == -1) min_dist = max(dist[i] - l[a][i].pos, r[b][i].pos - dist[i]);
					else min_dist = min(min_dist, max(dist[i] - l[a][i].pos, r[b][i].pos - dist[i]));
				}
			}
		}
	}
	
	// >>>> FURTHEST TRIPLE <<<<
	
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j <= n + 1; ++j) l[i][j] = r[i][j] = (pair){-1, -1};
	}
	
	// finding furthest motels on the left
	for(int i = 1; i <= n; ++i){
		for(int j = 0; j < 3; ++j) l[j][i] = l[j][i - 1];
		for(int j = 0; j < 3; ++j){
			l[j][i] = l[j][i - 1];
			if(l[j][i - 1].type == arr[i]) break;
			if(l[j][i - 1].type == -1){
				l[j][i] = (pair){arr[i], dist[i]};
				break;
			}
		}
	}
	
	// finding furthest motels on the right
	for(int i = n; i >= 1; --i){
		for(int j = 0; j < 3; ++j) r[j][i] = r[j][i + 1];
		for(int j = 0; j < 3; ++j){
			if(r[j][i + 1].type == arr[i]) break;
			if(r[j][i + 1].type == -1){
				r[j][i] = (pair){arr[i], dist[i]};
				break;
			}
		}
	}
	
	// looking for furthest triple of distinct motels
	int max_dist = 0;
	for(int i = 1; i <= n; ++i){
		for(int a = 0; a < 3; ++a){
			if(l[a][i].type == -1 || l[a][i].type == arr[i]) continue;
			for(int b = 0; b < 3; ++b){
				if(r[b][i].type == -1 || r[b][i].type == arr[i]) continue;
				if(l[a][i].type != r[b][i].type)
					max_dist=max(max_dist, min(dist[i] - l[a][i].pos, r[b][i].pos - dist[i]));
			}
		}
	}
	
	if(min_dist == -1) min_dist = 0;
	printf("%d %d\n", min_dist, max_dist);
}
