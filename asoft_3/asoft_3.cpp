/*
3) Вывести на экран все перестановки строки abcd (все возможные
комбинации, которые можно составить из символов 'a', 'b', 'c', 'd',
использовав каждый ровно 1 раз).
*/

#include <string>
#include <iostream>
#include <algorithm>

using namespace std;


int main()
{
    std::string s ("abcd");

    do {
        std::cout << s << '\n';
    } while ( std::next_permutation(s.begin(),s.end() ) );

    return 0;
}