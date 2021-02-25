// #include "bits/stdc++.h"

#include <map> 
#include <vector> 
#include <unordered_map> 
#include <string>
#include <iostream>
#include <iostream>
using namespace std;

class Street
{
public:
    int cost;
    int start;
    int end;
    string name;
};

class Intersection
{
public:
    int inter_id;
    unordered_map<string, Street> incoming;
    unordered_map<string, Street> outgoing;
};

unordered_map<int, vector<pair<string, int> > > optimize(unordered_map<string, Street> streets, unordered_map<int, Intersection> intersections, vector<vector<Street> > paths, int D){
    unordered_map<int, vector<pair<string, int> > >  ans; 
    int v=paths.size(); 
    // int J = interstections.size(); 
    // int s = streets.size(); 
    string data[v][D+1]; 
    // memset(data, 0, sizeof(int) * v * (D+1)); 
    for(int i=0; i<v; i++){
        for(int j =0; j<=D; j++){
            data[i][j] = ""; 
        }
    }
    // cout << "initialized data\n"; 
    for(int i=0;i<paths.size();i++){
        vector<Street> curr= paths[i];
        int cur_time=0;
        // cout << "\n"; 
        for(auto street: curr){
            if(cur_time>D){
                break; 
            }
            int next = min(cur_time + street.cost, D+1);
            for(int t = cur_time;t < next; t++){
                data[i][t] = street.name; 
                // cout << data[i][t] << " " << i  << "-" << t << "\n";  
            }
            // cout << cur_time << " " << next << " " << street.name << "\n"; 
            cur_time = next; 
        }
    }
    // cout << "data done\n"; 
    map<string, vector<int> > stats;
    for(int j =0; j<D+1; j++){
        map<string, int> counts; 
        for(int i=0; i<v; i++){
            if(data[i][j] !=  ""){
                counts[data[i][j]]++; 
            }
        }
        for(auto count: counts){
                stats[count.first].push_back(count.second); 
        }
    }
    map<string,int> medians;
    for(auto count: stats){
        sort(count.second.begin(), count.second.end()); 
        medians[count.first] = count.second[(count.second.size())/2]; 
    }
    
    for(auto inter: intersections){
        int incomingCount=inter.second.incoming.size();
        int cycleTotal=incomingCount*(D/2);
        int medianSum=0;
        vector<pair<string,int> > finalRes;
        for(auto st: inter.second.incoming){
            medianSum+=medians[st.first];
        }
        for(auto st: inter.second.incoming){
            int trafficTime = (medians[st.first] * cycleTotal) / medianSum;
            if(trafficTime>0)
                finalRes.push_back(make_pair(st.first,trafficTime));
        } 
        ans[inter.first]=finalRes;
    }

    return ans; 
}  




int main()
{
    int duration, n_inter, n_st, n_c, f;
    cin >> duration >> n_inter >> n_st >> n_c >> f;
    unordered_map<string, Street> streets;
    unordered_map<int, Intersection> intersections;
    vector<vector<Street> > paths;
    // get streets
    for (int i = 0; i < n_st; ++i)
    {
        Street new_st;
        cin >> new_st.start >> new_st.end >> new_st.name >> new_st.cost;
        streets[new_st.name]  = new_st;
        if (intersections.find(new_st.start) == intersections.end())
        {
            Intersection intersec;
            intersec.inter_id = new_st.start;
            intersec.outgoing[new_st.name]=  new_st;
            intersections[new_st.start] = intersec;
        }
        else
        {
            intersections[new_st.start].outgoing[new_st.name] =  new_st;
        }
        if (intersections.find(new_st.end) == intersections.end())
        {
            Intersection intersec;
            intersec.inter_id = new_st.end;
            intersec.incoming[new_st.name] = new_st;
            intersections[new_st.end] =  intersec;
        }
        else
        {
            intersections[new_st.end].incoming[new_st.name] = new_st;
        }
    }
    // for(auto street: streets){
    //     cout << street.first << " " << street.second.name << " " << street.second.start << " " << street.second.end << endl;
    // }
    // for (auto intersec :intersections){
    //     cout << intersec.first << endl;
    //     cout << "incoming: " << endl;
    //     for (auto street : intersec.second.incoming)
    //     {
    //         cout << street.first << " " << street.second.name << "\t";
    //     }
    //     cout << endl;
    //     cout << "outgoing:" << endl;
    //     for (auto street : intersec.second.outgoing)
    //     {
    //         cout << street.first << " " << street.second.name << "\t";
    //     }
    //     cout << endl;
    // }
    // // get paths
    for (int i = 0; i < n_c; ++i)
    {
        vector<Street> path;
        int pathlen;
        cin >> pathlen;
        for (int j = 0; j < pathlen; ++j)
        {
            string pname;
            cin >> pname;
            path.push_back(streets[pname]);
        }
        paths.push_back(path);
    }
    // for (int i = 0; i < paths.size();++i)
    // {
    //     cout << i << endl;
    //     for(auto street:paths[i]){
    //         cout << street.name << endl;
    //     }
    // }
    // cout << "Done taking input\n"; 

    unordered_map<int, vector<pair<string, int> > > output = optimize(streets, intersections, paths, duration);
    cout << output.size() << "\n";
    for (auto intersec : output)
    {
        cout << intersec.first << "\n";
        cout << intersec.second.size() << "\n";
        for (auto street : intersec.second)
        {
            cout << street.first << " " << street.second << "\n";
        }
    }
    return 0;
}