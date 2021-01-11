#ifndef _ROOM_H_
#define _ROOM_H_
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;
class Room
{
private:
    vector<vector<int> >room;
    pair<int,int> Jerry;
    pair<int,int> Tom;
    int furnitureCount, paintSpotsCount;
    pair<int, int> paintSpots;
    int n, m;
    void loadGraphwithoutFurniture(vector<vector<pair<int, char> > > &);
    void loadRoom(ifstream& fileName);
    void findAllPaths(int, int, const vector<vector<pair<int, char> > > &, vector<bool> &, string=""); //dfs
    int findMinimalPaths(int, int, const vector<vector<pair<int, char> > > &, vector<vector<pair<char,int> > >&);//bfs
    void dfs(int, int, const  vector<vector<pair<int, char> > > &);

public:
    Room(ifstream& fileName);
    void start();   //Room& operator=(Room const& other);
    void print();

};
void Room::loadRoom(ifstream& fout)
{
    fout>>m>>n>>Jerry.first>>Jerry.second>>Tom.first>>Tom.second>>furnitureCount>>paintSpotsCount;
    room.resize(n);
    for(int i=0; i<room.size(); i++)
        room[i].resize(m);
    int tempfCount = furnitureCount, tempPaintSpCount = paintSpotsCount;
    while (tempfCount != 0)
    {
        int furnitureX, furnitureY;
        fout >> furnitureX >> furnitureY;
        string str;
        fout.get();//gets rid of newline
        getline(fout, str);
        int r = furnitureX, c=furnitureY ;
        while (str != "===")
        {
            int cnt=0;
            for(int i=0; i<str.length(); i++)
            {
                if(str[i] != ' ' )
                {
                    room[r][c] = 1;
                }
                else // cout<<str[i]<<i<<c;
                {
                    room[r][c] = 0;
                }
                c++;
                cnt++;
            }
            r++;
            c=c-cnt;
            getline(fout, str);
        }

        tempfCount--;
    }

    while (tempPaintSpCount != 0)
    {
        int paintSpotX, paintSpotY;
        fout>>paintSpotX>>paintSpotY;
        int r = paintSpotX, c=paintSpotY ;
        room[r][c]=2;
        tempPaintSpCount--;
    }

}
Room::Room(ifstream& fout)
{
    loadRoom(fout);
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
            cout<<room[i][j];
        cout<<endl;
    }

}
void Room::loadGraphwithoutFurniture(vector<vector<pair<int, char> > > &graph)
{
    graph.resize(n*m);

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            if(room[i][j] == 0 || room[i][j] == 2)
            {
                int x = i*m+j;
                if(room[i][j] == 2)
                {
                    graph[x].push_back(make_pair(x, 'p'));
                }
                if(j+1 < m &&  (room[i][j+1] == 0 || room[i][j+1] == 2))
                {
                    int y=i*m+j+1;
                    graph[x].push_back(make_pair(y, 'e'));
                    graph[y].push_back(make_pair(x, 'w'));
                }
                if(i+1 < n && (room[i+1][j] == 0 || room[i+1][j] == 2))
                {
                    int y=(i+1)*m+j;
                    graph[x].push_back(make_pair(y, 's'));
                    graph[y].push_back(make_pair(x, 'n'));
                }
            }
        }

    }
    for(int i=0; i<n*m; i++)
    {
        cout<<i<<": ";
        for(int j=0; j<graph[i].size(); j++)
            cout<<"("<<graph[i][j].first<<", "<<graph[i][j].second<<") ";
        cout<<endl;
    }
}
void Room::findAllPaths(int v, int f, const vector<vector<pair<int, char> > > &graph, vector<bool> &used,  string path)
{
    if(v == f)
    {
        cout<<endl<<path/*<<"--out of the strhhhh"*/<<endl<<endl;
        return;
    }
    used[v] = 1;
    int cnt = 0;
    bool check=false;
    for(int i=0; i<graph[v].size(); i++)
    {
        if(!used[graph[v][i].first] )
        {
            findAllPaths(graph[v][i].first, f, graph, used, path + graph[v][i].second);
        }
        else if(graph[v][i].second == 'p')
        {
            path += graph[v][i].second;
        }
    }
    used[v] = 0;
}
int Room::findMinimalPaths(int v, int f, const vector<vector<pair<int, char> > > &g, vector<vector<pair<char, int> > >&parents)
{
    if(v==f)
        return 0;
    ///as role as used but shows the distance from the start vertex and the number of paths
    vector<vector<int> >dist;
    vector<vector<int> >times;
    dist.resize(n);
    times.resize(n);
    for(int i=0; i<n; i++)
    {
        dist[i].assign(m,-1);
        times[i].assign(m,-1);

    }
    int x=v/m, y=v%m;
    dist[x][y]=0;
    times[x][y]=1;;
    queue<int> q;
    q.push(v);
    parents[v].push_back(make_pair('s', v));
//   dist[v]=make_pair(0, 1);
    int k = 0;
    while(!q.empty() )
    {
        int v = q.front();
        q.pop();
        for(int i=0; i<g[v].size(); i++)
        {
            int x=v%m, y=i%m;

            int u=g[v][i].first;
            // cout<<u<<endl;
            ///we have another path with the same distance
            /* if(g[v][i].second == 'p')
                    parents[u].push_back(make_pair(g[v][i].second,u));*/
            // else
            // {
            if(dist[u/m][y] == dist[x][y])
            {
                times[u/m][y]=dist[x][y]+2;
                parents[u].push_back(make_pair(g[v][i].second, v));
            }
            if(dist[u/m][y] == -1)
            {
                dist[u/m][y] = dist[x][y]+1;
                if(u != f)
                    q.push(u);
                //   else if(u==f)
                //parents[u].push_back(make_pair(g[v][i].second,u));
                parents[u].push_back(make_pair(g[v][i].second,v));
            }
        }
    }
    //parents[13].clear();
    for(int i=0; i<dist.size(); i++)
    {
        cout<<"i: "<<i;
        for(int j=0; j<dist[i].size(); j++)
            cout<<"j: "<<j<<" ("<<dist[i][j]<<")";
        //cout<<endl;
        cout<<endl;
    }
    for(int i=0; i<times.size(); i++)
    {
        cout<<"i: "<<i;
        for(int j=0; j<times[i].size(); j++)
            cout<<"j: "<<j<<" ("<<times[i][j]<<")";
        //cout<<endl;
        cout<<endl;
    }
    int newX=f/m, newY =f%m;
    return times[newX][newY];
}
void printPathTo(int v,vector<vector<pair<char, int> > >&parents )
{
    for(int i=0; i<parents.size(); i++)
    {
        if(parents[v][i].second == v)
        {
            cout<<v<<" ";
            return;
        }
        printPathTo(parents[v][i].second,parents);
        cout<<v<<" ";
    }
}
void Room::dfs(int s, int f, const  vector<vector<pair<int, char> > > &graph)
{
    vector<bool> used;
    used.assign(graph.size(), 0);
    findAllPaths(s, f, graph, used);
}
void Room::start()
{
    vector<vector<pair<int, char> > >graph;
    loadGraphwithoutFurniture(graph);
    int x1=Tom.first, y1 =Tom.second, x2=Jerry.first, y2=Jerry.second;
    dfs(x1*m+y1, x2*m+y2, graph);
    vector<vector<pair<char, int> > >parents;
    parents.resize(n*m);
    cout<<findMinimalPaths(x1*m+y1, x2*m+y2, graph,parents);
    for(int i=0; i<n*m; i++)
    {
        cout<<"i: "<<i;
        for(int j=0; j<parents[i].size(); j++)
            cout/*<<"j: "<<j*/<<" ("<<parents[i][j].first<<", "<<parents[i][j].second<<") ";
        cout<<endl;
    }
//   printPathTo(x2*m+y2, parents);

}
#endif
