/*

*/

#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
//#include <streambuf>
#include <hash_map>
#include <map>

using namespace std;
using namespace stdext;

struct Output 
{          
    void operator() (const std::pair<string,unsigned> & p) const
    { 
        std::cout << p.first << '=' << p.second << "\n";
    }
} ;


int main()
{
    std::ifstream t("file.txt");
    
    typedef hash_map<string, unsigned> Map;
    
    Map counter;
    string word;

    while( t >> word )
    {
        counter[word] ++;
    }

    for_each( counter.begin(), counter.end(), Output() );

    return 0;
}