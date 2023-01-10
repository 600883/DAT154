#include<iostream>
#include<string>

using namespace std;

int main() {
    wchar_t s[] = L"Its a nice day, my house is on fire";
    int n = 0;
    for(wchar_t *p = 0; *p != 0; *p++) {
        if(*p == 'e') 
            n++;
    }
    wcout << L"Number of e: " << n << endl;
    return 0;
}
