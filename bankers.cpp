// Sanjoy Chowdhury 20172123

#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;

// added comparator function
bool sortbysec(const pair<int,int> &a,const pair<int,int> &b)
{
    return (a.second < b.second);
}


int main(int argc, char*argv[])
{

	///////////////////////////File streams/////////////////////////////
	fstream infile(argv[1], std::ios_base::in);
	fstream outfile(argv[2], std::ios_base::out);


	///////////////////////////INPUTS///////////////////////////////////
	int n,r,q,isAvailable = 0;
	infile >> n >> r >> q;
	int x;


	//for PID///////////////////////////////////////////////////////////
	vector<int> pid;
	for(int i=0;i<n;++i)
	{
		infile >> x;
		pid.push_back(x);
	}


	//FOR ARRIVAL///////////////////////////////////////////////////////

	multimap<int,int> arrival;

	for(int i=0;i<n;++i)
	{
		infile >> x;
		arrival.insert(pair <int, int> (x,pid[i]));
	}

	//FOR AVAILABLE/////////////////////////////////////////////////////

	vector<int> available;
	for(int i=0;i<r;++i)
	{
		infile >> x;
		available.push_back(x);
	}

	//FOR MAXNEED///////////////////////////////////////////////////////

	map< int,vector<int> > maxneed;

	for(int i=0;i<n;++i)
	{
		vector<int>temp;
		for(int j=0;j<r;++j)
		{
			infile >> x;
			temp.push_back(x);
		}

		maxneed.insert(pair <int, vector<int> >(pid[i], temp));
	}

	//FOR ALLOCATED/////////////////////////////////////////////////////

	map< int,vector<int> > allocated;

	for(int i=0;i<n;++i)
	{
		vector<int>temp;
		for(int j=0;j<r;++j)
		{
			infile >> x;
			temp.push_back(x);
		}

		allocated.insert(pair <int, vector<int> >(pid[i], temp));
	}

	//FOR QUERIES///////////////////////////////////////////////////////

	// map containing the queries fired at runtime by the user
	map< int,vector< vector<int> > >queries;

	for(int i=0;i<q;++i)
	{
		int prc;
		infile >> prc;
		vector<int>temp;
		for(int j=0;j<r;++j)
		{
			infile >> x;
			temp.push_back(x);
		}
		if(queries.find(prc)==queries.end())
		queries.insert(pair <int, vector<vector<int> > >(prc, vector<vector<int> >() ));
		queries[prc].push_back(temp);
	}

	/////////////////////////END OF INPUT///////////////////////////////

	vector<int>safesequence;
	vector<int>readyqueue;
	map <int, int> :: iterator itr_arr;  //first arrival
	itr_arr=arrival.begin();
	int tt = itr_arr->first;

	map <int, int> :: iterator itr_arrend;  //last arrival
	itr_arrend=arrival.end();
	itr_arrend--;
	int ttend = itr_arrend->first;
	unsigned int fails=0;



	while(true)
	{
		//REFRESHING READY QUEUE////////////////////////////////////////

		for(itr_arr=arrival.begin();itr_arr!=arrival.end();++itr_arr)
		{
			if(tt==itr_arr->first)
			{
				readyqueue.push_back(itr_arr->second);
				sort(readyqueue.begin(),readyqueue.end());
			}
		}

		////////////////////////////////////////////////////////////////

		for(unsigned int i=0;i<readyqueue.size();i++)
		{
			int currpid=readyqueue[i];
			vector<vector<int> > cpq = queries[currpid];
			vector<int> cpqt = cpq[0];
			bool cangrant = true;
			for(unsigned int j=0;j<cpqt.size();j++)  //Checking if top query of current process can be satisfied
			{
				if(cpqt[j]>available[j])
				{
					cangrant = false;
					fails++;
				}
			}
			if(cangrant)  //if YES
			{
				fails=0; //Reseting the fails ie Continuous Denials

				for(unsigned int j=0;j<cpqt.size();j++)  //Decreasing the available
					available[j]=available[j]-cpqt[j];

				vector<int> cpav = allocated[currpid];  //Increasing the allocated of current process
				for(unsigned int j=0;j<cpqt.size();j++)
				{
					cpav[j]=cpav[j]+cpqt[j];
				}
				allocated[currpid]=cpav;

				if(isAvailable) {} // For debugging purpose
				vector<int> cpm = maxneed[currpid];  //Checking of the MAX need is satisfied
				bool maxfulfilled=true; // To check if all request has been fulfilled
				for(unsigned int j=0;j<cpqt.size();j++
					if(cpav[j]!=cpm[j])
					{
						maxfulfilled = false;
						break;
					}
				if(maxfulfilled)  //if YES
				{
					readyqueue.erase(readyqueue.begin() + i);  //Remove the current process from ready queue
					for(unsigned int j=0;j<cpqt.size();j++)
					{
						available[j]=available[j]+cpm[j];
					}
 				}
				else
				{
					cpq.erase(cpq.begin());  //remove the current query from the current process list
					queries[currpid]=cpq;
				}

				tt++;	//Increasing time
				outfile << "Grant" << endl;
				safesequence.push_back(currpid);
				break;  //As we have to start again


			} //end of YES : Query can run
			else
			{
				outfile << "Deny" << endl;
			}
		} //end of the ReadyQueue Iterator



		if((readyqueue.empty() && tt > ttend ))
		{
			for(unsigned int k=0;k<safesequence.size();++k)
			{
				outfile << safesequence[k] << " ";
			}
			break;
		}

		if(fails>=readyqueue.size())
		{
			tt++;
			if(tt>ttend)
			{
				outfile << "Not safe";
				return 0;
			}
		}

	} //end of while



	//END OF PROGRAM

	return 0;

}
