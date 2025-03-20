#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Use: all-tests [num-test]\n";
    return 1;
  }
  cout << "Running tests...\n";
  return 0;
}
