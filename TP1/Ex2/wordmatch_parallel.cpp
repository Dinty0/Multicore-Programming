
#include <vector>
#include <fstream>
#include <iostream>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

using namespace std;
using namespace tbb;

typedef blocked_range<int> range;

class wordMatcher()
{
	public:
		wordMatcher(vector<string>& a, string w) : words(a), toMatch(w) {}
		void operator()(const range& r) const
		{
			for (int i = r.begin(); i!=r.end(); ++i)
			{
				if (matches(words[i],toMatch))
				{
					// use concurrent vector global to push back matched word
				}
			}
		}

	private:
		vector<string> words;
		string toMatch;
}


bool matches(string a, string b)
{
	bool matched = true;

	if (a.length() != b.length())
	{
		matched = false;
	}
	else
	{
		for (int i = 0; i<a.length(); i++)
		{
			if ((a[i] != b[i]) && (a[i] != '.'))
			{
				matched = false;
			}
		}
	}

	return matched;
}


int main(int argc, char* argv[])
{
	vector<string> words;
	// concurent vector matched

	if (argc == 2)
	{
		ifstream file;
		file.open("sowpods.txt");
		string line;

		if (file.is_open())
		{
			while(getline(file,line))
			{
				words.push_back(line);
			}
		}

		file.close();


		cout << "Found " << matched.size() << " matches for " << argv[1] << endl;

		for (int i=0; i<matched.size(); i++)
		{
			cout << matched.at(i) << endl;
		}
	}
	else
	{
		cout << "Use one paramater only" << endl;
	}
	
}