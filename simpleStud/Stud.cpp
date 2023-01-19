/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>

using namespace std;

class Stud {
    public: 
        Stud(int nr): _nr(nr) {}
        int _nr;
};

void Show(Stud *p) {
    wcout << L"Stud nr:" << p-> _nr << endl;
}

int main()
{
   Stud *ps1 = new Stud(1);
   Stud *ps2 = new Stud(2);
   
   Show(ps1);
   Show(ps2);
   
   delete ps1;
   delete ps2;
}
