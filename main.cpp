#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>


using namespace std;
struct Tree
{
    char data;
    vector<Tree*> children;

    Tree* addChild(const char c)
    {
        children.push_back(new Tree(c));
        return children[children.size()-1];
    }

    Tree* getChild(const char c)
    {
        for(int i=0; i<children.size(); i++)
        {
            if(children[i]->data == c)
                return children[i];
        }
        return nullptr;
    }
    void deleteTree()
    {
        for(int i=0; i<children.size(); i++)
            delete children[i];
    }
    //  public:
    Tree(const char data) : data(data) {}
    ~Tree()
    {
        deleteTree();
    }
    void addPath(string str)
    {
        if(str.size()==0)
            return;

        Tree* child = getChild(str[0]);
        if(child == nullptr)
            child = addChild(str[0]);
        child->addPath(str.substr(1));
    }
    void dfs()
    {
        cout<<data;
        for(int i=0; i<children.size(); i++)
            children[i]->dfs();
        //  cout<<endl;
    }

};

void dfs(int v, int f, const vector<vector<pair<int, char> > > &graph, vector<bool> &used,vector<string>& vec,  string path="")
{
    if(v == f)
    {
        vec.push_back(path);
        // cout<<endl<<path/*<<"--out of the strhhhh"*/<<endl<<endl;
        return ;
    }
    used[v] = 1;
    int cnt = 0;
    bool check=false;
    /*for(int j=0; j<graph[v].size(); j++)
    {
        if(graph[v][j].second == 'p')
            cnt++;
    }*/
    for(int i=0; i<graph[v].size(); i++)
    {
        if(!used[graph[v][i].first] /*&& cnt==0*/)
        {
            /*  cout<<"count of p:"<<cnt<<" - here is the lertter:: "<<graph[v][i].second<<" "<<" on which position in room:"<<v <<endl;*/
            dfs(graph[v][i].first, f, graph, used, vec, path + graph[v][i].second);
        }
        else if(graph[v][i].second == 'p' /*&& check==false*/)
        {
            /*   cout<<endl<<"count of p:"<<cnt<<" p on which position:: "<<v<<" here is p occutance:: "<<endl;*/
            path += graph[v][i].second;
            //cnt--;
            // check = true;
            //  i=0;
        }
    }
    used[v] = 0;
}

void dfs(int s, int f, const  vector<vector<pair<int, char> > > &graph, vector<string>& vec)
{
    vector<bool> used;
    used.assign(graph.size(), 0);
    dfs(s, f, graph, used, vec);
}

void readFromFile(ifstream& fout, int& m, int& n, pair<int, int>& Jerry, pair<int, int>& Tom,  int& furnitureCount, int& paintSpotsCount,
                  vector<vector<int> >& room, pair<int, int>& paintSpotsCoordinates)
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

int minimalPathLength(vector<pair<string, int > > vec)
{
    int  minimalLength = vec[0].first.size();
    for (int i = 1; i < vec.size(); i++)
    {
        if (minimalLength > vec[i].first.size())
            minimalLength = vec[i].first.size();
    }
    return minimalLength;
}
int getTurns(string& str)
{
    int  turnCnt = 0;
    for (int i = 0; i < str.size()-1; i++)
    {
        if (str[i]!=str[i+1])
            turnCnt++;
    }
    return turnCnt;
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
    Tree tomsTree(' ');
    vector<string> vec;
    vector<pair<string, int> > vecWithoutP;

    dfs(x1*m+y1, x2*m+y2, graph, vec);
    for(int i=0; i<vec.size(); i++)
    {
        string newStr;
        int cnt = 0;
        for(int j=0; j<vec[i].size(); j++)
        {
            if(vec[i][j] != 'p')
            {
                newStr+=vec[i][j];

            } else cnt++;
        }
        vecWithoutP.push_back(make_pair(newStr, cnt));
    }
   /*  for(int i=0; i<vec.size(); i++)
    {
        cout<<vecWithoutP[i].second<<endl;
    }*/
    //toms
    //tomsTree.dfs();
    vector<pair<string, int> > minimalPaths;
    int minNum =  minimalPathLength(vecWithoutP);
    for (int i = 0; i <vecWithoutP.size(); i++)
    {
       // cout<<vecWithoutP[i].first.size();
       if (vecWithoutP[i].first.size() ==  minNum)
         minimalPaths.push_back(make_pair(vecWithoutP[i].first, vecWithoutP[i].second));
    }
    cout<<"minimal path legth is "<< minNum<<endl;
   /*  for(int i=0; i<minimalstrs.size(); i++)
    {
        cout<< minimalstrs[i].first<<endl;
    }*/
    for (int i=0; i<minimalPaths.size(); i++)
    {
        cout << "Minimal str " << i + 1 << " : ";
        for (int j = 0; j <  minimalPaths[i].first.size(); j++)
        {
            if (j ==  minimalPaths[i].first.size() - 1)
                cout<< minimalPaths[i].first[j];
            else
                cout<< minimalPaths[i].first[j] << " -> ";}

                cout<<endl;
       /* cout<<" Steps until Jerry:"<<minNum<<endl;*/
        cout<<"Painted Spots until Jerry:"<< minimalPaths[i].second<<endl;
        cout<<"Turns until Jerry:"<<getTurns(minimalPaths[i].first)<<endl;
        cout<<"this is all about the path above"<<endl<<endl;
   }
    return 0;

}
