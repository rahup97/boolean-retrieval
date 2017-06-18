#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <dirent.h>

using namespace std;

map<string,vector<pair<int,int> > > voc;
int docID=1;
int pos;

void index(string s)
{
    istringstream iss(s);
    do
    {
        string sub;
        iss>>sub;
        voc[sub].push_back(make_pair(docID,pos++));
    }while(iss);
}

void display()
{
    cout<<"\nIndex-:\n\n";
    map<string,vector<pair<int,int> > >:: iterator it;
    voc.erase(voc.begin());
    for(it=voc.begin();it!=voc.end();it++)
    {
        cout<<it->first;
        vector<pair<int,int> >:: iterator q;
        for(q=it->second.begin();q!=it->second.end();q++)
            cout<<"->"<<q->first<<","<<q->second;
        cout<<endl;
    }
}

void extract_file(char * s)
{
    pos=1;
    string line;
    ifstream fin(s);
    if (fin.is_open())
    {
        while(getline(fin,line))
        {
            for(unsigned int i=0;i<line.length();i++)
            {
                if(ispunct(line[i]))
                    line.erase(line.begin()+i);
            }
            transform(line.begin(),line.end(),line.begin(),::tolower);
            index(line);
            pos--;
        }
        fin.close();
    }
    else
        cout << "Unable to open file";
}

vector<pair<int,int> > intersect(vector<pair<int,int> > p1, vector<pair<int,int> > p2)
{
    vector<pair<int,int> > res;
    unsigned int i=0,j=0;
    while(i<p1.size() && j<p2.size())
    {
        if(p1[i].first==p2[j].first)
        {
            res.push_back(make_pair(p1[i].first,0));
            i++;
            j++;
        }
        else if(p1[i].first<p2[j].first)
            i++;
        else
            j++;
    }
    return res;
}

void search(string q)
{
    cout<<"\nSearch Results-:\n";
    vector<pair<int,int> > ans,temp;
    vector<pair<int,int> >:: iterator r;
    string s1;
    istringstream iss(q);
    iss>>s1;
    ans=voc[s1];
    while(iss)
    {
        iss>>s1;
        temp=voc[s1];
        ans=intersect(ans,temp);
    }

    for(r=ans.begin();r!=ans.end();r++)
    {
        if(r==ans.end()-1)
        {
            cout<<r->first<<endl;
            break;
        }
        if(r->first!=(r+1)->first)
            cout<<r->first<<endl;
    }
}

int main()
{
    const string str(".txt");
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("D:\\gcc-codeblocks\\ir1")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            string s(ent->d_name);
            if(s.find(str)!=string::npos)
            {
                extract_file(ent->d_name);
                docID++;
            }
        }
        display();
        closedir(dir);
    }
    else
        cout<<"\nCould not access the collection!";
    cout<<"\nEnter a Boolean Query:";
    string q;
    getline(cin,q);
    search(q);
    return 0;
}