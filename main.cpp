#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

void dfs(int v, int f, const vector<vector<pair<int, char> > > &graph, vector<bool> &used, string path="")
{
    if(v == f)
    {
        cout<<endl<<path<<"--out of the pathhhhh"<<endl<<endl;
        return ;
    }
    //  cout<<endl<<v<<endl;
    used[v] = 1;
    int cnt = 0;
    bool check=false;
    for(int j=0; j<graph[v].size(); j++)
    {
        if(graph[v][j].second == 'p')
            cnt++;
    }
    for(int i=0; i<graph[v].size(); i++)
    {
        if(!used[graph[v][i].first] /*&& cnt==0*/)
        {
            cout<<"count of p:"<<cnt<<" - here is the lertter:: "<<graph[v][i].second<<" "<<" on which position in room:"<<v <<endl;
            dfs(graph[v][i].first, f, graph, used, path+graph[v][i].second);
            /* dfs(graph[v][i].first, f, graph, used, path+to_string(graph[v][i].first)+":"+graph[v][i].second);*/
        }
        else if(graph[v][i].second == 'p' && check==false)
        {
            cout<<endl<<"count of p:"<<cnt<<" p on which position:: "<<v<<" here is p occutance:: "<<endl;
            path+=graph[v][i].second;
            cnt--;
            check = true;
            i=0;
        }
    }
    used[v] = 0;
}

void dfs(int s, int f, const  vector<vector<pair<int, char> > > &graph)
{
    vector<bool> used;
    used.assign(graph.size(), 0);
    dfs(s, f, graph, used);
}

void readFromFile(ifstream& fout, int& m, int& n, pair<int, int>& Jerry, pair<int, int>& Tom,  int& furnitureCount, int& paintSpotsCount,
                  vector<vector<int> >& room, pair<int, int>& paintSpotsCoordinates)
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
        int r = furnitureX, c=furnitureY ;
        while ( str != "===")
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
        fout >> paintSpotX >> paintSpotY;
        int r = paintSpotX, c=paintSpotY ;
        room[r][c]=2;
        tempPaintSpCount--;
    }
}
int main()
{
    int n, m;// length and width
    pair<int,int> Jerry;
    pair<int,int> Tom;
    int furnitureCount, paintSpotsCount;
    vector<vector<int> >room;
    pair<int, int> paintSpots;
    string fileName = "text1.txt";
    ifstream fout;
    fout.open(fileName);
    readFromFile(fout, m, n, Jerry, Tom, furnitureCount, paintSpotsCount, room, paintSpots);
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
            cout<<room[i][j];
        cout<<endl;
    }
    vector<vector<pair<int, char> > >graph;
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
    int x1=Tom.first, y1 =Tom.second, x2=Jerry.first, y2=Jerry.second;
    dfs(x1*m+y1, x2*m+y2, graph);

    return 0;

}
