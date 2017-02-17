#include "semaphore.h"
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

enum e_flag 
{
	M,
	W,
	N
};

e_flag flag = N;

int number = 0;

Semaphore sexe(1);
Semaphore occup(3);

struct Man
{
	Man(int v) : id(v) {}
	void operator()(void)
	{
		int t = (int) rand() % 10;

		if (flag == N)
		{
			P(sexe);
			flag = M;
			P(occup);
			++number;

			cout << number << endl;

			cout << "Man " << id << " in." << endl;
			this_thread::sleep_for(chrono::seconds(t));

			if (number == 1)
			{
				V(sexe);
				flag = N;
				V(occup);
				cout << "Man " << id << " out." << endl;
				--number;
			}
			else
			{
				V(occup);
				cout << "Man " << id << " out." << endl;
				--number;
			}

		}
		else if (flag == M)
		{
			P(occup);
			++number;

			cout << "Man " << id << " in." << endl;
			this_thread::sleep_for(chrono::seconds(t));

			if (number == 1)
			{
				V(sexe);
				flag = N;
				V(occup);
				cout << "Man " << id << " out." << endl;
				--number;
			}
			else
			{
				V(occup);
				cout << "Man " << id << " out." << endl;
				--number;
			}

		}
		else
		{
			P(sexe);
		}
		
	}
	int id;
};

struct Woman
{
	Woman(int v) : id(v) {}
	void operator()(void)
	{
		int t = (int) rand() % 10;

		if (flag == N)
		{
			P(sexe);
			flag = W;
			P(occup);
			++number;

			cout << "Woman " << id << " in." << endl;
			this_thread::sleep_for(chrono::seconds(t));

			if (number == 1)
			{
				V(sexe);
				V(occup);
				cout << "Woman " << id << " out." << endl;
				--number;
			}
			else
			{
				V(occup);
				cout << "Woman " << id << " out." << endl;
				--number;
			}

		}
		else if (flag == W)
		{
			P(occup);
			++number;
			
			cout << "Woman " << id << " in." << endl;
			this_thread::sleep_for(chrono::seconds(t));

			if (number == 1)
			{
				V(sexe);
				V(occup);
				cout << "Woman " << id << " out." << endl;
				--number;
			}
			else
			{
				V(occup);
				cout << "Woman " << id << " out." << endl;
				--number;
			}

		}
		else
		{
			P(sexe);
		}
		
	}
	int id;
};

int main()
{


	vector<thread> individuals;

	individuals.push_back(thread(Man(1)));
	individuals.push_back(thread(Man(2)));
	individuals.push_back(thread(Woman(1)));
	individuals.push_back(thread(Man(3)));
	individuals.push_back(thread(Man(4)));
	individuals.push_back(thread(Woman(2)));

	for(auto& i : individuals)
	{
		i.join();
	}

	cout << "Everybody done" << endl;
}