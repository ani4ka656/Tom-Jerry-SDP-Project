#ifndef _ROOM_H_
#define _ROOM_H_
#include <fstream>
#include "Tree.hpp"
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
    void loadGraphwithoutFurniture(vector<vector<pair<int, char> > >&);
    void loadRoom(ifstream&);
    int findMinimalPaths(int, int, const vector<vector<pair<int, char> > >& , vector<vector<char > >&);//bfs
    void printPathTo(int v, vector<vector<char> >&parents, string = "");
public:
    Room(string&);
    void start();
    void print();

};
void Room::loadRoom(ifstream& fout)
{
    fout >> m >> n >> Jerry.first >> Jerry.second >> Tom.first >> Tom.second >> furnitureCount >> paintSpotsCount;
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
        int r = furnitureX, c = furnitureY ;
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
            c = c - cnt;
            getline(fout, str);
        }

        tempfCount--;
    }

    while (tempPaintSpCount != 0)
    {
        int paintSpotX, paintSpotY;
        fout>>paintSpotX>>paintSpotY;
        int r = paintSpotX, c=paintSpotY ;
        room[r][c] = 2;
        tempPaintSpCount--;
    }

}
Room::Room(string& fileName)
{
    ifstream fout;
    fout.open(fileName);
    if(!fout)
    {
        fout.close();
        cout<<"File cannot be openned"<<endl;
        return;
    }
    else
    {

        loadRoom(fout);
        fout.close();
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<m; j++)
                cout<<room[i][j];
            cout<<endl;
        }

    }
}
void Room::loadGraphwithoutFurniture(vector<vector<pair<int, char> > >& graph)
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
int Room::findMinimalPaths(int v, int f, const vector<vector<pair<int, char> > >& g, vector<vector<char> >& parents)
{
    if(v==f)
        return 0;
    ///as role as used but shows the distance from the start vertex and the number of paths
    vector<pair<int, int> >dist;

    dist.assign(g.size(), make_pair(-1, -1));
    queue<int> q;
    q.push(v);
    parents[v].push_back('b');
    dist[v] = make_pair(0, 1);
    while(!q.empty() )
    {
        int v = q.front();
        q.pop();

        for(int i=0; i<g[v].size(); i++)
        {
            int u=g[v][i].first;
            ///we have another path with the same distance
           /*  if(g[v][i].second == 'p')
                    parents[u].push_back(g[v][i].second);*/
            if(dist[u].first == dist[v].first+1)
            {
                dist[u].second++;
                parents[u].push_back(g[v][i].second);
            }
            if(dist[u].first == -1)
            {
                dist[u] = make_pair(dist[v].first + 1, dist[v].second);
                if(u != f)
                    q.push(u);
                parents[u].push_back(g[v][i].second);
            }
        }
    }
    for(int i=0; i<dist.size(); i++)
        cout<<"i: "<<i<<" ("<<dist[i].first<<", "<<dist[i].second<<")"<<endl;

    return dist[f].second;
}
void Room::printPathTo(int v,vector<vector<char> >&parents, string path)
{
    for(int i=0; i<parents[v].size(); i++)
    {
        int newV=0;

        if(parents[v][i] == 'n')
            newV = v - m;
        if(parents[v][i] == 'e')
            newV = v + 1;
        if(parents[v][i] == 'w')
            newV = v - 1;
        if(parents[v][i] == 's')
            newV = v + m;
       /* if(parents[v][i]== 'p')
            newV = -1;*/
        if(parents[v][i] == 'b')
        {
            string newPath="";
            int k = 0;
            for(int i=0; i<path.size(); i++){
                if(path[i]=='s')newPath[k] = 'n';
                if(path[i]=='n')newPath[k] = 's';
                if(path[i]=='w')newPath[k] = 'e';
                if(path[i]=='e')newPath[k] = 'w';
                k++;
            }
            cout<<k;
            return;
        }
        path += parents[v][i];
        printPathTo(newV, parents, path);
    }
}
void Room::start()
{
    vector<vector<pair<int, char> > > graph;
    loadGraphwithoutFurniture(graph);
    int x1=Tom.first, y1 =Tom.second, x2=Jerry.first, y2=Jerry.second;
    vector<vector<char> >parents;
    parents.resize(n*m);

    cout<<findMinimalPaths(x1*m+y1, x2*m+y2, graph,parents);
    for(int i=0; i<n*m; i++)
    {
        cout<<"i: "<<i;
        for(int j=0; j<parents[i].size(); j++)
            cout/*<<"j: "<<j*/<<" ("<<parents[i][j]<<") ";
        cout<<endl;
    }
   // printPathTo(x2*m+y2, parents);
    string path = "nnnee";
    Tree t(' ');
    t.addPath(path);
    path = "nneen";
    t.addPath(path);
    t.print();

}
#endif
