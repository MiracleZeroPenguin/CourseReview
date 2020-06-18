#include<bits/stdc++.h>
using namespace std;

int read(){
    char c;int x;while(c=getchar(),c<'0'||c>'9');x=c-'0';
    while(c=getchar(),c>='0'&&c<='9') x=x*10+c-'0';return x;
}

const int MAXN=10005,MAXM=100005;

int n,m,st,en,x,y,c,H,T,ans,use,cnt=0;
int nxt[MAXM*2],head[MAXN],dis[MAXN],q[MAXM*6];
struct node{
    int to,val;
}L[MAXM*2];

void add(int x,int y,int c){
    L[cnt]=(node){y,c};
    nxt[cnt]=head[x];
    head[x]=cnt;
    cnt++;
    L[cnt]=(node){x,0};
    nxt[cnt]=head[y];
    head[y]=cnt;
    cnt++;
}

int BFS(){
    H=0;T=1;q[T]=st;
    memset(dis,0,sizeof(dis));dis[st]=1;
    while(H<T){
        int front=q[++H];use=head[front];
        while(use!=-1){
            if(!dis[L[use].to]&&L[use].val){
                dis[L[use].to]=dis[front]+1;
                q[++T]=L[use].to;
            }
            use=nxt[use];
        }
    }
    return dis[en];
}

int dinic(int now,int x){
    if(now==en) return x;
    use=head[now];
    while(use!=-1){
        int rec=use;
        if(dis[L[use].to]==dis[now]+1&&L[use].val){
            int fd=dinic(L[use].to,min(x,L[use].val));
            use=rec;
            if(fd){
                L[use].val-=fd;
                L[use^1].val+=fd;
                return fd;
            }
        }
        use=nxt[rec];
    }
    return 0;
}

int main()
{
    memset(head,-1,sizeof(head));
    n=read();m=read();st=read();en=read();
    for(int i=1;i<=m;i++){
        x=read();y=read();c=read();
        add(x,y,c);
    }
    while(BFS()){
        ans+=dinic(st,2e9);
    } 
    printf("%d",ans);
    return 0;
}

