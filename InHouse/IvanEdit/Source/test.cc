#include <iostream>
#include <vector>
#include <iterator>

int main(void)
{
 
 vector<int> VectorArray(0);
 for(int x = 0; x < 10; x++)
  VectorArray.assign(x + 1, 1);

 for(int x = 0; x < 10; x++)
  cout << VectorArray[x] << "\n";
 return 0;
}
