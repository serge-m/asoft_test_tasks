/*
3) ������� �� ����� ��� ������������ ������ abcd (��� ���������
����������, ������� ����� ��������� �� �������� 'a', 'b', 'c', 'd',
����������� ������ ����� 1 ���).
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