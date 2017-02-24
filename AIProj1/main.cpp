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
#include <iomanip>

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
int valueofsack(sack* a, int n)
{
    int tot = 0;
    for(int i=0; i<n; i++)
    {
        tot += a->values[i];
    }
    return tot;
}
void copylefttoright(sack * a, sack * b, int n)//unused
{
    
}
void filterlefttoright(sack* a,int items, sack* b, int& miniitems, bool* master)
{
    miniitems = 0;
    for(int i=0; i<items; i++)
    {
        if(master[i])
        {
            b->names[miniitems]=a->names[i];
            b->costs[miniitems]=a->costs[i];
            b->values[miniitems]=a->values[i];
            b->ratios[miniitems]=a->ratios[i];
            miniitems++;
        }
    }
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
  //  cout<<"doing greedy, "<<n<<"items, max weight = "<<maxweight<<"\n";
    for(int i=0; i<n; i++)
    {
        masterlist[i] = false;
    }
    //s is ordered in descending usefulness
    int tolwt=0;
    int tolval=0;
    //initially sack is empty
    for(int i=0; i<n; i++)
    {
  //      cout<<"trying "<<s->names[i]<<", ";
        tolwt += s->costs[i];//try adding current item to sack or skip
        if(tolwt <= maxweight)
        {
  //          cout<<"success\n";
            tolval += s->values[i];
            masterlist[i] = true;
        }
        else
        {
  //          cout<<"failed\n";
            tolwt -= s->costs[i];
        }
    }
 //   cout<<"weight after greedy: "<<tolwt;
}
bool partial(int n, sack* s, int maxweight, bool* masterlist, double& pct, int &extraspot)
{
    for(int i=0; i<n; i++)
    {
        masterlist[i] = false;
    }
    //s is ordered in descending usefulness
    int tolwt=0;
    double tolval=0;
    //initially sack is empty
    for(int i=0; i<n; i++)
    {
        tolwt+=s->costs[i];//try adding current item to sack or else if not underweight
        if(tolwt < maxweight)
        {
            tolval += s->values[i];
            masterlist[i] = true;
        }
        else //not underweight
        {
            int wtgap = tolwt - maxweight;
            if (wtgap != 0) //overweight
            {
                tolwt -= s->costs[i];
                wtgap = maxweight - tolwt;
                int partial = s->costs[i];//give partial i's cost
                pct = (double) wtgap / (double) partial; //ratio is gap/partial
                extraspot = i;
                return true;
            }
            else//perfect fit
            {
                return false;
            }
        }
    }
    return false;
}
void brute(int cur, int n, sack* s, int weight, int value, int maxweight, bool* list, bool* masterlist, int &bestval)
{
    //cur is current spot in the bool list simulated tree, initially called with 0
    //n is total length of tree
    //list is
    
    //check cur vs int
    if(cur>=n)
        return;
    
    //exclude current
    brute(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval);
    
    //include current
    weight += s->costs[cur];
    value += s->values[cur];
    list[cur]=true;
    
    //current value is better than best value seen and valid weight
    if(value > bestval && weight <= maxweight)
    {
        bestval = value;
        for(int i=0; i<n; i++)
        {
            masterlist[i] = list[i];
        }
    }
    brute(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval);
    list[cur]=false;
}

void exhaustive(int cur, int n, sack* s, int weight, int value, int maxweight, bool* list, bool* masterlist, int &bestval, int rem, int& toppest)
{
    //check cur vs int
    if(cur>=n)
        return;
    if(value + rem < toppest)
        return;
    //exclude current
    
    rem = rem - s->values[cur];
    exhaustive(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval, rem, toppest);
    
    //include current
    weight += s->costs[cur];
    
    //if overweight then break branch
    if(weight > maxweight)
        return;
    //if unfruitful return
    //if current total value + 
    
    
    
    value += s->values[cur];
    //not underweight,
    list[cur]=true;
    
    //current value is better than best value seen,
    if(value > bestval)
    {
        if( value > toppest)
            toppest = value;
        bestval = value;
        for(int i=0; i<n; i++)
        {
            masterlist[i] = list[i];
        }
    }
    exhaustive(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval, rem, toppest);
    list[cur] = false;
}

void printsack(sack s, int n)
{
    cout<<"SACK_BEGIN\n";
    int weight = 0;
    int value = 0;
    for(int i=0; i<n; i++)
    {
        weight += s.costs[i];
        value += s.values[i];
        cout<<s.names[i]<<","<<s.costs[i]<<","<<s.values[i]<<','<<s.ratios[i]<<endl;
    }
    cout<<"TOTAL WEIGHT: "<<weight<<endl;
    cout<<"TOTAL VALUE: "<<value <<endl;
    cout<<"/SACK_END\n\n";
}
string printsack2(sack s, int n)
{
    //print cost and items
    int val=0;
    int cst=0;
    for(int i=0; i<n; i++)
    {
        val += s.values[i];
        cst += s.costs[i];
    }
    string start = "value: ";
    start += to_string(val);
    start += ", cost: ";
    start += to_string(cst);
    start += ", items: ";
    start += s.names[0];
    for(int i=1; i<n; i++)
    {
        start += ", ";
        start += s.names[i];
    }
    return start;
}
string printsack3(sack s, int n, double d, int c)
{
    //print cost and items
    double val=0;
    for(int i=0; i<n; i++)
    {
        val += s.values[i];
    }
    val += d;
    int realval = val;
    string start = "value: ";
    start += to_string(realval);
    start += ", cost: ";
    start += to_string(c);
    start += ", items: ";
    start += s.names[0];
    for(int i=1; i<n; i++)
    {
        start += ", ";
        start += s.names[i];
    }
    return start;
}
int main()
{
    string out1="1. ",out2="",out3="",out4="",out5="",out6="6. ",out7="7. ";
    string filename, bigline; int items,capacity; sack sack1;
    bool * master, doingbruteforce;
    ifstream fin;
    ofstream fout;
    cout<<"Enter exact filename for testing: ";
    cin>>filename;
    out1 += filename;
    fin.open(filename.c_str());
    if(fin.is_open())
    {
        cout<<"file opened successfully.\n";
        init_sack(sack1, 200);
        master = new bool[200];
        int cur = 0;
        string store="";
        getline(fin,store);
        out2 = "2. ";
        out2+=store;
        capacity = stoi(store);
        cout<<store<<endl;
        while(!fin.eof())
        {
         //   cout<<"took a line again\n";
            getline(fin,bigline);
            if(bigline == "")
                break;
            store="";
            //item name
            int ii = 0;
            for(;bigline[ii]!=','; ii++)//until char is comma
            {
                store += bigline[ii]; //add char to sto
            }
            sack1.names[cur] = store;
            ii++;
            store = "";
            //1st int
            for(;bigline[ii]!=','; ii++)//until char is comma
            {
                store += bigline[ii]; //add char to sto
            }
            sack1.costs[cur] = stoi(store);
            ii++;
            store = "";
            //2nd int
            for(;bigline[ii]!=0; ii++)//until 0 termin string
            {
                store += bigline[ii]; //add char to sto
            }
            sack1.values[cur] = stoi(store);
            cur++;
        }
        items = cur;
        printsack(sack1, items);
        int totsumsum = 0;
        for(int i=0; i<items; i++)
            totsumsum+=sack1.costs[i];
        cout<<"TOTAL COST ALL ITEMS = "<<totsumsum<<endl;
        string yesorno;
        
        //check if doing brute force
        cout<<"\nThis file has "<<items<<" items. Include brute force method? (y/n)";
        cin>>yesorno;
        if(yesorno == "y")
            doingbruteforce=true;
        else
            doingbruteforce=false;
        
        
        
    //do phase 1: greedy low cost, high val, high ratio
        cout<<"Beginning greedy tests:\n";
        //calculate the ratios
        for(int i=0; i<items; i++)
        {
            sack1.ratios[i] = (double)sack1.values[i] / sack1.costs[i];
        }
        
       // printsack(sack1, items);
        
        
        int lcval, hvval, hrval;
        //begin low cost
        ShellSort4ByFirstInt(sack1.names, sack1.costs, sack1.values, sack1.ratios, items);
//        cout<<"sorted by lowest cost first:\n";
//        printsack(sack1, items);
        
        sack lowcostsack;
        int lowcostitems;
        init_sack(lowcostsack, items);
        greedy(items, &sack1, capacity, master);
        filterlefttoright(&sack1, items, &lowcostsack, lowcostitems, master);

        ShellSort4ByName(lowcostsack.names, lowcostsack.costs, lowcostsack.values, lowcostsack.ratios, lowcostitems);
     //   cout<<"results from greedy low cost first:\n";
  //      printsack(lowcostsack, lowcostitems);
        lcval = valueofsack(&lowcostsack, lowcostitems);
        
        
        //begin highest val
        for(int i=0; i<items; i++)//temp flip the values to sort descending (transform & conquer)
            sack1.values[i] *= -1;
        ShellSort4ByFirstInt(sack1.names, sack1.values, sack1.costs, sack1.ratios, items);
        for(int i=0; i<items; i++)//unflip
            sack1.values[i] *= -1;
//        cout<<"sorted by highest value first:\n";
//        printsack(sack1, items);
        sack highvalsack;
        int highvalitems;
        init_sack(highvalsack, items);
        greedy(items, &sack1, capacity, master);
        filterlefttoright(&sack1, items, &highvalsack, highvalitems, master);
        
        ShellSort4ByName(highvalsack.names, highvalsack.costs, highvalsack.values, highvalsack.ratios, highvalitems);
  //      cout<<"results from greedy highest value first:\n";
 //       printsack(highvalsack, highvalitems);
        hvval = valueofsack(&highvalsack, highvalitems);
        
        //begin highest ratio
        ShellSort4ByDoubleDescending(sack1.names, sack1.costs, sack1.values, sack1.ratios, items);
//        cout<<"sorted by highest ratio first:\n";
//        printsack(sack1,items);
        
        sack highratsack;
        int highratitems;
        init_sack(highratsack, items);
        greedy(items, &sack1, capacity, master);
        filterlefttoright(&sack1, items, &highratsack, highratitems, master);
        
        
        ShellSort4ByName(highratsack.names, highratsack.costs, highratsack.values, highratsack.ratios, highratitems);
 //       cout<<"results from greedy highest ratio first:\n";
 //       printsack(highratsack, highratitems);
        hrval = valueofsack(&highratsack, highratitems);
        //begin partial
//        sack partialsack;
//        int partialitems;
//        init_sack(partialsack, items);
//        greedy(items, &sack1, capacity, master);
//        filterlefttoright(&sack1, items, &highratsack, highratitems, master);
        
        double pct;
        int exindex;
        if(partial(items, &sack1, capacity, master, pct, exindex))
        {
            sack partialsack;
            int partialitems;
            init_sack(partialsack, items);
            filterlefttoright(&sack1, items, &partialsack, partialitems, master);
            out4 = "4. Results from partial sack:\n";
            ShellSort4ByName(partialsack.names, partialsack.costs, partialsack.values, partialsack.ratios, partialitems);
            
            out4 += printsack3(partialsack, partialitems, pct * (double)sack1.values[exindex], capacity);
            
            out4 +=", plus ";
            int inthere = pct * 100;
            out4 += to_string(inthere);
            out4 += "% of ";
            out4 += sack1.names[exindex];
           // cout<<", plus "<< pct << "of "<<sack1.names[exindex]<<"("<<pct * (double)sack1.values[exindex]<<")";
            
            destroy(partialsack);
        }
        else
        {
            cout<<"No partial sack, perfect fit.\n";
            sack partialsack;
            int partialitems;
            init_sack(partialsack, items);
            filterlefttoright(&sack1, items, &partialsack, partialitems, master);
            out4 = "Results from partial sack:\n";
            ShellSort4ByName(partialsack.names, partialsack.costs, partialsack.values, partialsack.ratios, partialitems);
            out4 += printsack2(partialsack, partialitems);
            
            destroy(partialsack);

        }
    //end phase 1
        cout<<"Greedy tests complete.\n";
        int topper;
        out3 = "";//best greedy min boundary
        if(lcval >= hvval && lcval >= hrval)//lcval largest of the 3
        {
            out3 += "3. The greedy min boundary is the low-cost first method.\n";
            out3 += printsack2(lowcostsack, lowcostitems);
            topper = lcval;
        }
        else if(hvval >= lcval && hvval >= hrval)//hvval (high value sack) largest of the 3
        {
            out3 += "3. The greedy min boundary is the high-value first method.\n";
            out3 += printsack2(highvalsack, highvalitems);
            topper = hvval;

        }
        else//hrval is either largest or not less than the others
        {
            out3 += "3. The greedy min boundary is the high-ratio first method.\n";
            out3 += printsack2(highratsack, highratitems);
            topper = hrval;
        }
        //cout<<"p1 done, greedy thing on next line:\n"<<out3<<endl;
        bool * state = new bool[200];
        
        
        int best = 0;
        for(int i=0; i<items; i++)
        {
            state[i] = false;
            master[i] = false;
        }
        
        //        clock_t start, end;
        clock_t brutestart, bruteend;

        if(doingbruteforce == true)
        {
            cout<<"Starting brute force method:\n";
            double brutime = 0;
            brutestart = clock();
            brute(0, items, &sack1, 0, 0, capacity, state, master, best);
            bruteend = clock();
            brutime = (bruteend-brutestart) / (double) CLOCKS_PER_SEC;
            out6 += "brute force time: ";
            out6 += to_string(brutime);
            out6 += "sec.";
            sack brutesack;
            int bruteitems=0;
            init_sack(brutesack, items);
            filterlefttoright(&sack1, items, &brutesack, bruteitems, master);
            
            ShellSort4ByName(brutesack.names, brutesack.costs, brutesack.values, brutesack.ratios, bruteitems);
   //         cout<<"results from brute force:"<<endl;
  //          printsack(brutesack,bruteitems);
            
            
            destroy(brutesack);
            cout<<"Brute force complete.\n";
        }
        
        //begin optimaled
        
        ShellSort4ByDoubleAscending(sack1.names, sack1.costs, sack1.values, sack1.ratios, items);

        
        
//        for(int i=0; i<items; i++)//temp flip the values to sort descending (transform & conquer)
//            sack1.costs[i] *= -1;
//        ShellSort4ByFirstInt(sack1.names, sack1.costs, sack1.values, sack1.ratios, items);
//        for(int i=0; i<items; i++)//unflip
//            sack1.costs[i] *= -1;
        
        cout<<"Starting optimized method:\n";
        best = 0;
        int rem= 0;
        for(int i=0; i<items; i++)
        {
            state[i] = false;
            master[i] = false;
            rem += sack1.values[i];
        }
        double optime = 0;
        clock_t optstart, optend;
        optstart = clock();
        exhaustive(0, items, &sack1, 0, 0, capacity, state, master, best, rem , topper);
        optend = clock();
        optime = (optend-optstart) / (double) CLOCKS_PER_SEC;
        out7 += "optimized time: ";
        out7 += to_string(optime);
        out7 += "sec.";
        
        sack optsack;
        int optitems=0;
        init_sack(optsack, items);
        filterlefttoright(&sack1, items, &optsack, optitems, master);
        ShellSort4ByName(optsack.names, optsack.costs, optsack.values, optsack.ratios, optitems);
        out5 = "5. Results from optimized search: ";
        out5 += printsack2(optsack, optitems);
  //      printsack(optsack, optitems);
        cout<<"Optimized method complete.\n";
        destroy(sack1);
        destroy(lowcostsack);
        destroy(highvalsack);
        destroy(highratsack);

        delete [] master;
        delete [] state;
        fin.close();
        cout<<endl<<out1<<endl<<out2<<endl<<out3<<endl<<out4<<endl<<out5<<endl<<out6<<endl<<out7<<endl;
        string outfile=filename;
        outfile.erase(outfile.end()-4,outfile.end());
        outfile += "_results.txt";
        fout.open(outfile.c_str());
        fout<<out1<<endl<<out2<<endl<<out3<<endl<<out4<<endl<<out5<<endl<<out6<<endl<<out7<<endl;
    }
    else
        cout<<"failed to open file.\n";
}
