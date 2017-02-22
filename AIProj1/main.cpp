//
//  main.cpp
//  AIProj1
//
//  Created by Joseph on 2/18/17.
//  Copyright © 2017 JosephC. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <stdlib.h>
using namespace std;
#include <fstream>
#include "sorting.h"

struct sack{
    string * names;
    int* costs;
    int* values;
    double* ratios;
};
void init_sack(sack &s, int n){
    s.names = new string[n];
    s.costs = new int[n];
    s.values = new int[n];
    s.ratios = new double[n];
}
void destroy(sack& s)
{
    delete [] s.names;
    delete [] s.costs;
    delete [] s.values;
    delete [] s.ratios;
}
void greedy(int n, sack* s, int maxweight, bool* masterlist)
{
    //s is ordered in descending usefulness
    int tolwt=0;
    int tolval=0;
    //initially sack is empty
    for(int i=0; i<n; i++)
    {
        tolwt+=s->costs[i];//try adding current item to sack or skip
        if(tolwt <= maxweight)
        {
            tolval += s->values[i];
            masterlist[i] = true;
        }
        else
            tolwt -= maxweight;
    }
}
void partial(int n, sack* s, int maxweight)
{
    
}
void brute(int cur, int n, sack* s, int weight, int value, int maxweight, bool* list, bool* masterlist, int &bestval)
{
    //check cur vs int
    if(cur>=n)
        return;
    
    //exclude current
    brute(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval);
    
    //include current
    weight += s->costs[cur];
    value += s->values[cur];
    
    //if overweight then break branch
    if(weight <= maxweight)
        return;
    
    //not underweight,
    list[cur]=true;
    
    //current value is better than best value seen,
    if(value > bestval)
    {
        bestval = value;
        for(int i=0; i<n; i++)
        {
            masterlist[i] = list[i];
        }
    }
    brute(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval);
}

void exhaustive(int cur, int n, sack* s, int weight, int value, int maxweight, bool* list, bool* masterlist, int &bestval)
{
    //check cur vs int
    if(cur>=n)
        return;
    
    //exclude current
    exhaustive(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval);
    
    //include current
    weight += s->costs[cur];
    value += s->values[cur];
    
    //if overweight then break branch
    if(weight <= maxweight)
        return;
    
    //not underweight,
    list[cur]=true;
    
    //current value is better than best value seen,
    if(value > bestval)
    {
        bestval = value;
        for(int i=0; i<n; i++)
        {
            masterlist[i] = list[i];
        }
    }
    exhaustive(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval);
}

void printsack(sack s, int n)
{
    cout<<"SACK_BEGIN\n";
    for(int i=0; i<n; i++)
    {
        cout<<s.names[i]<<","<<s.costs[i]<<","<<s.values[i]<<','<<s.ratios[i]<<endl;
    }
    cout<<"/SACK_END\n\n";
}
int main()
{
    
    string filename, bigline; int items,capacity; sack sack1;
    bool * master;
    ifstream fin;
    cout<<"enter filename: ";
    cin>>filename;
    fin.open(filename.c_str());
    if(fin.is_open())
    {
        init_sack(sack1, 200);
        master = new bool[items];
        int cur = 0;
        string sto="";
        getline(fin,sto);
        capacity = stoi(sto);
        while(!fin.eof())
        {
            getline(fin,bigline);
            sto="";
            //item name
            int ii = 0;
            for(;bigline[ii]!=','; ii++)//until char is comma
            {
                sto += bigline[ii]; //add char to sto
            }
            sack1.names[cur] = sto;
            ii++;
            sto = "";
            //1st int
            for(;bigline[ii]!=','; ii++)//until char is comma
            {
                sto += bigline[ii]; //add char to sto
            }
            sack1.costs[cur] = stoi(sto);
            ii++;
            sto = "";
            //2nd int
            for(;bigline[ii]!=0; ii++)//until 0 termin string
            {
                sto += bigline[ii]; //add char to sto
            }
            sack1.values[cur] = stoi(sto);
            cur++;
        }
        items = cur;
        printsack(sack1, items);
        int totsumsum = 0;
        for(int i=0; i<items; i++)
            totsumsum+=sack1.costs[i];
        cout<<"TOTAL COST ALL ITEMS = "<<totsumsum<<endl;
    //do phase 1
        
        //calculate the ratios
        for(int i=0; i<items; i++)
        {
            sack1.ratios[i] = (double)sack1.values[i] / sack1.costs[i];
        }
        
        printsack(sack1, items);
        
        ShellSort4ByFirstInt(sack1.names, sack1.costs, sack1.values, sack1.ratios, items);
        cout<<"sorted by lowest cost first:\n";
        printsack(sack1, items);
        sack minisack;
        int miniitems=0;
        init_sack(minisack, items);
        greedy(items, &sack1, capacity, master);
        for(int i=0; i<items; i++)
        {
            if(master[i])
            {
                minisack.names[miniitems]=sack1.names[i];
                minisack.costs[miniitems]=sack1.costs[i];
                minisack.values[miniitems]=sack1.values[i];
                minisack.ratios[miniitems]=sack1.ratios[i];
                miniitems++;
            }
        }
        cout<<"results from greedy low cost first:\n";
        printsack(minisack, miniitems);
        
        
        
        
        for(int i=0; i<items; i++)//temp flip the values to sort descending (transform & conquer)
            sack1.values[i] *= -1;
        ShellSort4ByFirstInt(sack1.names, sack1.values, sack1.costs, sack1.ratios, items);
        for(int i=0; i<items; i++)//unflip
            sack1.values[i] *= -1;
        cout<<"sorted by highest value first:\n";
        printsack(sack1, items);
        greedy(items, &sack1, capacity, master);
        miniitems=0;
        for(int i=0; i<items; i++)
        {
            if(master[i])
            {
                minisack.names[miniitems]=sack1.names[i];
                minisack.costs[miniitems]=sack1.costs[i];
                minisack.values[miniitems]=sack1.values[i];
                minisack.ratios[miniitems]=sack1.ratios[i];
                miniitems++;
            }
        }
        cout<<"results from greedy highest value first:\n";
        printsack(minisack, miniitems);

        
        ShellSort4ByDoubleDescending(sack1.names, sack1.costs, sack1.values, sack1.ratios, items);
        cout<<"sorted by highest ratio first:\n";
        printsack(sack1,items);
        greedy(items, &sack1, capacity, master);
        miniitems=0;
        for(int i=0; i<items; i++)
        {
            if(master[i])
            {
                minisack.names[miniitems]=sack1.names[i];
                minisack.costs[miniitems]=sack1.costs[i];
                minisack.values[miniitems]=sack1.values[i];
                minisack.ratios[miniitems]=sack1.ratios[i];
                miniitems++;
            }
        }
        cout<<"results from greedy highest ratio first:\n";
        printsack(minisack, miniitems);
        
        
    //end phase 1
        
        
        destroy(sack1);
        destroy(minisack);
        delete [] master;
        
        fin.close();
    }
    else
        cout<<"failed to open file.\n";
}
