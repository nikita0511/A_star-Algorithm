#include <bits/stdc++.h>
using namespace std;
vector < vector < int > > space;

vector < pair < pair < int, int >, float > > neighbours(int x, int y, int X, int Y)
{
	vector < pair < pair < int, int >, float > > vec;
	if(x-1<X && x-1>=0 && y-1>=0 && y-1 <Y && space[x-1][y-1] == 0)
		vec.push_back(make_pair(make_pair(x-1,y-1), 1.414));
	if(x>=0 && y-1>=0 && x<X && y-1 <Y && space[x][y-1] == 0)
		vec.push_back(make_pair(make_pair(x,y-1), 1));
	if(x-1 <X && x-1>=0 && y<Y && y>=0&& space[x-1][y] == 0)
		vec.push_back(make_pair(make_pair(x-1,y), 1));
	if(x+1< X && x+1>=0 && y+1>=0 &&y+1<Y && space[x+1][y+1] == 0)
		vec.push_back(make_pair(make_pair(x+1,y+1), 1.414));
	if(x >=0 && x<X && y+1>=0 && y+1<Y && space[x][y+1] == 0)
		vec.push_back(make_pair(make_pair(x,y+1), 1));
	if(x+1 >= 0 && x+1< X && y>=0 && y< Y && space[x+1][y] == 0)
		vec.push_back(make_pair(make_pair(x+1,y), 1));
	if(x+1<X && x+1 >=0 && y-1<Y && y-1>=0 && space[x+1][y-1] == 0)
		vec.push_back(make_pair(make_pair(x+1,y-1),1.414));
	if(x-1>=0 && x-1 < X && y+1 >=0 && y+1 <Y && space[x-1][y+1] == 0)
		vec.push_back(make_pair(make_pair(x-1,y+1), 1.414));
	return vec;
}

float heuristic(int x, int y, int X, int Y)//returns h(n) = euclidian distance
{
	float euc_dist = sqrt(pow(X - x, 2) + pow(Y - y, 2)); //euclidian
	//float euc_dist = abs(X-x) + abs(Y-y); //manhattan (not admissible)
	return euc_dist;
}

 int parseLine(char* line) //reference: http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
 {
    int i = strlen(line);
    while (*line < '0' || *line > '9') line++;
    line[i-3] = '\0';
    i = atoi(line);
    return i;
}
int getValue(){ //Note: this value is in KB!//reference: http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];
    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0)
        {
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

int main(int argc, char *argv[])
{
	ios_base::sync_with_stdio(false);
	clock_t tStart = clock(); //starts calculating time
	char *filename;
	ifstream inputfile;
	filename = argv[1]; //taking name of file as input
	inputfile.open(filename,ifstream::in);
	int x,y,sx,sy,gx,gy,i,j,count =0,flag = 0;
	string word;
	inputfile>>word;
	inputfile>>y;
	inputfile>>x;
	inputfile>>word;
	inputfile>>sy;
	inputfile>>sx;
	inputfile>>word;
	inputfile>>gy;
	inputfile>>gx;
	inputfile>>word;
	for(i=0;i<x;i++)
	{	
		vector < int > temp1;
		for(j=0;j<y;j++)
		{
			int val;
			inputfile >> val;
			temp1.push_back(val);
		}
		space.push_back(temp1);
	}
	float Memory=(float)getValue()/1000.0; //calculates memory used by program
	ofstream outfile("Solution.txt");
	if(space[sx][sy] == 1 || space[gx][gy]==1)
		cout<<"Path Cost: 10000000"<<", States Expanded: "<<count<<", Run Time: "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<", Memory: "<<Memory<<"\n";	
	else
	{
		float f = heuristic(sx,sy,gx,gy);
		set < pair < float, pair < int, int > > > open;
		set < pair < float, pair < int, int > > >::iterator it;
		open.insert(make_pair(f, make_pair(sx,sy)));
		pair < float, pair < int , int > > temp;
		map < pair < int, int >, int > visited;
		visited[make_pair(sx,sy)] = 1;
		map < pair < int, int >, float > gVal;
		gVal[make_pair(sx,sy)] = 0;
		map < pair < int, int >, pair < int, int > > parent;
		parent[make_pair(sx,sy)] = make_pair(-1,-1);
		while(open.empty() == false)
		{
			float h= 0, g= 0;
			it=open.begin();
			temp = make_pair((*it).first, make_pair((*it).second.first, (*it).second.second));
			open.erase(it);
			//expanded node count
			count++;
			//Goal test
			if(temp.second.first == gx && temp.second.second == gy)
			{
				cout<<"Path Cost: "<<temp.first<<", States Expanded: "<<count<<", Run Time: "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<", Memory: "<<Memory<<"\n";
				flag = 1;
				break;
			}	
			visited[make_pair(temp.second.first, temp.second.second)] = 2;
			vector < pair < pair < int, int >, float > > vec =  neighbours(temp.second.first, temp.second.second, x,y);
			for(i = 0;i<vec.size();i++)
			{
				if(visited[make_pair(vec[i].first.first, vec[i].first.second)] == 0)
				{
					h = heuristic(vec[i].first.first, vec[i].first.second, gx, gy);
					g = gVal[make_pair(temp.second.first, temp.second.second)] + vec[i].second;
					open.insert(make_pair(g+h, make_pair(vec[i].first.first, vec[i].first.second)));
					visited[make_pair(vec[i].first.first, vec[i].first.second)] = 1;
					gVal[make_pair(vec[i].first.first, vec[i].first.second)] = g;
					parent[make_pair(vec[i].first.first, vec[i].first.second)] = make_pair(temp.second.first, temp.second.second);
				}
				else if(visited[make_pair(vec[i].first.first, vec[i].first.second)] == 1)
				{
					if(gVal[make_pair(vec[i].first.first, vec[i].first.second)] > gVal[make_pair(temp.second.first, temp.second.second)]+vec[i].second)
					{
						it = open.find(make_pair(gVal[make_pair(vec[i].first.first, vec[i].first.second)] + heuristic(vec[i].first.first, vec[i].first.second,gx,gy), make_pair(vec[i].first.first, vec[i].first.second)));
						open.erase(it);
						gVal[make_pair(vec[i].first.first, vec[i].first.second)] = gVal[make_pair(temp.second.first, temp.second.second)]+vec[i].second;
						open.insert(make_pair(gVal[make_pair(vec[i].first.first, vec[i].first.second)] + heuristic(vec[i].first.first, vec[i].first.second,gx,gy), make_pair(vec[i].first.first, vec[i].first.second)));
						parent[make_pair(vec[i].first.first, vec[i].first.second)] = make_pair(temp.second.first, temp.second.second);
					}
				}
			}	
		}	
		if(flag ==0)//no path found
			cout<<"Path Cost: 10000000"<<", States Expanded: "<<count<<", Run Time: "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<", Memory: "<<Memory<<"\n";
		if(flag == 1)//if path found
		{
			i=gx;j=gy;
			vector < pair < int, int > > path;
			while(i!= sx && j!=sy)
			{
				path.push_back(make_pair(j,i));
				int te = i;
				i=parent[make_pair(i,j)].first;
				j=parent[make_pair(te,j)].second;
			}
			for(int k=path.size()-1;k>=0;k--)
				outfile<<path[k].first<<","<<path[k].second<<"\n";	
		}	
	}
			
	return 0;	
}