#include <string>
#include <iostream>
#include <algorithm>
using namespace std;


int main()
{
    {
        string s = "abcdefg";

        cout << s << endl;


        s.swap( string( s.rbegin(), s.rend() ) );

        cout << s << endl;
    }

    {
        string s = "abcdefg";

        cout << s << endl;

        reverse( s.begin(), s.end() );
        cout << s << endl;
    }

    return 0;
}