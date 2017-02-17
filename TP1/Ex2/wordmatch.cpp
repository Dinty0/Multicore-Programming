#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

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
	vector<string> matched;

	if (argc == 2)
	{
		ifstream file;
		file.open("sowpods.txt");
		string line;

		if (file.is_open())
		{
			while(getline(file,line))
			{
				if (matches(argv[1],line))
				{
					matched.push_back(line);
				}
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