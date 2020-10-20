#include <vector>
#include "analyzer.h"

int main(int argc, char** argv)
{
	if (argc <= 1)
		throw "Please input at least one file you want to analyze";
	
	vector<Analyzer*> analyzeList;

	mapInit();

	for (int i = 0; i < argc - 1; i++)
	{
		Analyzer* temp = new Analyzer(argv[i + 1], i + 1);
		if (!temp->status)
		{
			cout << "File " << i + 1 << " is not valid!" << endl;
			delete temp;
		}
		else
			analyzeList.push_back(temp);
	}
	
	for (int i = 0; i < analyzeList.size(); i++)
	{
		bool flag = true;
		do
		{
			Token token = analyzeList[i]->getToken();
			if (flag = analyzeList[i]->printToken(token))
				break;
		} 
		while(true);
		if (!flag)
			cout << "There is something wrong with file " << analyzeList[i]->no;
		analyzeList[i]->printInfo();
	}
}
