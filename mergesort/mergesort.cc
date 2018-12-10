#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

void LOG_VECTOR(const std::string& msg, const std::vector<int>& predicted, const std::vector<int>& expected) {
  int m=expected.size();
  bool pass=true;
  if (expected.size() != predicted.size())
    pass=false;
  else {
    for (int i=0;i<m;++i) {
      if (predicted[i]!=expected[i]) {
        pass=false;
        break;
      }
    }
  }
  cout << msg << ": ";
  if (pass)
    cout << "PASS" << endl;
  else
    cout << "FAIL" << endl;
}

std::vector<int> mergesort_merge(const std::vector<int>& a, const std::vector<int>& b) {
  std::vector<int> tmp;
  int idxa=0;
  int idxb=0;

  while ( (idxa < a.size()) && (idxb < b.size()) ) {
    if (a[idxa] < b[idxb]) {
      tmp.push_back(a[idxa]);
      idxa++;
    } else {
      tmp.push_back(b[idxb]);
      idxb++;
    }
  }
  while (idxa < a.size()) {
    tmp.push_back(a[idxa]);
    idxa++;
  }
  while (idxb < b.size()) {
    tmp.push_back(b[idxb]);
    idxb++;
  }
  return tmp;
  
}

std::vector<int>  mergesort_splitmerge(const std::vector<int>& v) {
  int m=v.size();
  int sizea=m/2;
  int sizeb=m-sizea;
  std::vector<int> left;
  std::vector<int> right;

  if (m < 2)
    return v;

  left.assign(v.begin(), v.begin()+sizea);
  right.assign(v.begin()+sizea, v.end());

  left=mergesort_splitmerge(left);
  right=mergesort_splitmerge(right);

  return mergesort_merge(left, right);
}

void mergesort(const std::vector<int>& dataarray, std::vector<int>& sortedarray) {
  sortedarray=mergesort_splitmerge(dataarray);  
}


bool bubblesort_helper(std::vector<int>& v) {
  bool swapped = false;
  int m=v.size();
  int tmp=0;

  for (int i=0;i<(m-1);++i) {
    if (v[i] > v[i+1]) {
      tmp=v[i];
      v[i]=v[i+1];
      v[i+1]=tmp;
      swapped = true;
    }
  }
  return swapped;
}

void bubblesort(const std::vector<int>& dataarray, std::vector<int>& sortedarray) {
  sortedarray.assign(dataarray.begin(), dataarray.end());
  while (bubblesort_helper(sortedarray)) {};
}

void gen_vector(int m, std::vector<int>& v, std::vector<int>& randarray) {
  auto rng = std::default_random_engine {};
  v.resize(m);
  for (int i=0;i<m;i++) {
    v[i]=i;
  }
  randarray.assign(v.begin(), v.end());

  std::shuffle(randarray.begin(), randarray.end(), rng);
}


int main(int argc, char** argv) {
  std::vector<int> randarray={20,4,5,6,1,2,8,19,5,5,3,14,13};
  std::vector<int> expectedarray={1,2,3,4,5,5,5,6,8,13,14,19,20};
  std::vector<int> sortedarray;
  int m=20000;
  std::clock_t    start;
  double mergetime=0.0;
  double bubbletime=0.0;

  gen_vector(m, expectedarray, randarray);

  sortedarray.clear();
  start = std::clock();
  mergesort(randarray, sortedarray);
  mergetime=(std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
  std::cout << "Time: " << mergetime << " ms" << std::endl;

  LOG_VECTOR("TEST MERGE SORT", sortedarray, expectedarray);

  sortedarray.clear();
  start = std::clock();
  bubblesort(randarray, sortedarray);
  bubbletime=(std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
  std::cout << "Time: " << bubbletime << " ms" << std::endl;
  LOG_VECTOR("TEST BUBBLE SORT", sortedarray, expectedarray);

  cout << "SPEED UP: " << bubbletime/mergetime << endl;

  return 0;
}
