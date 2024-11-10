#include "debug.h"
#include "Data.h"
#include <vector>

bool verificaConstrucao(Data &data, Solution &s)
{
    bool c1 = false;
    bool c2 = false;
    bool c3 = true;

    /* cout << "Tamanho de s " << s.sequence.size() << endl;
    cout << "Tamanho da dimensão " << data.getDimension() << endl; */

    if (s.sequence.size() == data.getDimension() + 1)
    {
        c1 = true;
    }

    int last = data.getDimension();

    /*    cout << "Primeiro " << s.sequence[0] << endl;
       cout << "ultimo " << s.sequence[last] << endl; */

    if (s.sequence[0] == 1 && s.sequence[last] == 1)
    {
        c2 = true;
    }

    vector<bool> teste(data.getDimension(), false);

    for (int n = 0; n < data.getDimension(); n++)
    {
        int number = s.sequence[n];
        if (teste[number] == true)
        {
            c3 = false;
        }
        else
        {
            teste[number] = true;
        }
    }
    /*
        cout << c1 << endl;
        cout << c2 << endl;
        cout << c3 << endl; */

    return c1 == true && c2 == true && c3 == true;
}
