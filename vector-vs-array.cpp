// vector-vs-array.cpp
// Copyright 2013 Skand Hurkat

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <vector>
#include <cstdlib>

#include <chrono>
#include <ratio>
#include <ctime>

using namespace std;

int main(void) 
{
  chrono::steady_clock::time_point start, end;
  chrono::duration<double> time_span_cumulative = 
    chrono::duration<double>::zero();

  // Begin C test. Declare array, fill it up, and repeat 10 times to get
  // average time taken to fill up 1000000 integers
  int* array;
  for(int i = 0; i < 1000; i++)
  {
    start = chrono::steady_clock::now();
    array = new int[1000000];
    if(array == nullptr)
    {
      cerr << "Fatal error with memory allocation" << endl;
      return -1;
    }
    register int* arrayitr = array;
    register int j = 0;
    do
    {
      *arrayitr++ = j++;
    }while(j < 1000000);
    end = chrono::steady_clock::now();
    time_span_cumulative += 
      chrono::duration_cast<chrono::duration<double>>(end-start);
    delete[] array;
  }
  cout << "The C like implementation took "
       << time_span_cumulative.count()/1000 << " seconds" << endl;

  // Begin the C++ STL implementation
  time_span_cumulative = chrono::duration<double>::zero();
  vector<int> v;
  for(int i = 0; i < 1000; i++)
  {
    start = chrono::steady_clock::now();
    v.reserve(1000000);
    for(register int j = 0; j < 1000000; j++)
    {
      v.emplace_back(j);
    }
    end = chrono::steady_clock::now();
    time_span_cumulative += 
      chrono::duration_cast<chrono::duration<double>>(end-start);
    if(v.capacity() != 1000000)
    {
      cerr << "Vector capacity changed to " << v.capacity() << endl;
      return 1;
    }
    vector<int>().swap(v);
  }
  cout << "The C++ implementation took "
       << time_span_cumulative.count()/1000 << " seconds" << endl;

  // Now, the so called "fair" C++ implementation using resize instead of
  // reserve
  time_span_cumulative = chrono::duration<double>::zero();
  vector<int>().swap(v);
  for(int i = 0; i < 1000; i++)
  {
    start = chrono::steady_clock::now();
    v.resize(1000000);
    register int j = 0;
    for(auto& e:v)
    {
      e = j++;
    }
    end = chrono::steady_clock::now();
    time_span_cumulative += 
      chrono::duration_cast<chrono::duration<double>>(end-start);
    if(v.capacity() != 1000000)
    {
      cerr << "Vector capacity changed to " << v.capacity() << endl;
      return 1;
    }
    vector<int>().swap(v);
  }
  cout << "The \"fair\" C++ implementation took "
       << time_span_cumulative.count()/1000 << " seconds" << endl;

  // Try to optimise the "fair" C++ implementation, but this is basically a
  // hack to reduce the data structure back to C like array.
  time_span_cumulative = chrono::duration<double>::zero();
  vector<int>().swap(v);
  for(int i = 0; i < 1000; i++)
  {
    start = chrono::steady_clock::now();
    v.resize(1000000);
    register int* iter = v.data();
    register int j = 0;
    do
    {
      *iter++ = j++;
    }while(j < 1000000);
    end = chrono::steady_clock::now();
    time_span_cumulative += 
      chrono::duration_cast<chrono::duration<double>>(end-start);
    if(v.capacity() != 1000000)
    {
      cerr << "Vector capacity changed to " << v.capacity() << endl;
      return 1;
    }
    vector<int>().swap(v);
  }
  cout << "The (un)fair C++ implementation took "
       << time_span_cumulative.count()/1000 << " seconds" << endl;
  return 0;
}
