#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <string>

using namespace std;

extern void rigidbody_test();

int main(int argc, char** argv) {
    map<string, function<void()>> tests{{"rigidbody", rigidbody_test}};

    if (argc == 1) {
        cout << "Running all tests...\n";
        for (const auto& test : tests) {
            cout << "Running test " << test.first << "...\n";
            test.second();
        }
    } else if (argc == 2) {
        string test_name(argv[1]);
        tests[test_name]();
    }
    return 0;
}
