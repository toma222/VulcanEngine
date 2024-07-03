
#include "kon/types/String.hpp"
#include "kon/types/ArrayList.hpp"


// Required headers, or just use #include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <ctime>


// CPlusPlusUnit - C++ Unit testing TDD framework (github.com/cppunit/cppunit)
class Cppunit { public:
    #define CHECK(a,b)  check<long long>(a, b, #a, #b, __FILE__, __LINE__, __FUNCTION__);
    #define CHECKT(a)   check<bool>(a, true, #a, "true", __FILE__, __LINE__, __FUNCTION__);
    #define CHECKS(a,b) check<cs>(a, b, #a, #b, __FILE__, __LINE__, __FUNCTION__);
    typedef const std::string& cs;
    int checks, fails; std::ostringstream serr; std::istringstream *in;
    Cppunit() { checks = fails = 0;}
    void test_cin(cs s){ in = new std::istringstream(s); std::cin.rdbuf(in->rdbuf()); }
    void fail_hdr(cs stra, cs strb, cs file, int line, cs func) {
        serr << "==================================================" << std::endl;
        serr << "FAIL: " << func << std::endl;
        serr << "--------------------------------------------------" << std::endl;
        serr << "File \"" << file << "\", line " << line << " in " << func << std::endl;
        serr << "  Checking " << stra << " == " << strb << std::endl;
    }
    template <typename T> void check(T a, T b, cs stra, cs strb, cs file, int line, cs func) {
        checks++; if (a == b) { std::cout << "."; return; }
        fails++; std::cout << "F"; fail_hdr(stra, strb, file, line, func);
        serr << "  Error: \"" << a << "\" ! = \"" << b << "\"" << std::endl << std::endl;
    }
    virtual void single_test() {}
    virtual void test_list() { single_test(); }
    double dclock() { return double(clock()) / CLOCKS_PER_SEC; }
    int status() {
        std::cout << std::endl; if (fails) std::cout << serr.str();
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Ran " << checks << " checks in " << dclock() << "s" << std::endl << std::endl;
        if (fails) std::cout << "FAILED (failures=" << fails << ")"; else std::cout << "OK" << std::endl;
        return fails > 0;
    }
    int run() { std::streambuf* ocin = std::cin.rdbuf(); test_list(); std::cin.rdbuf(ocin); return status(); }
};
// end of cppunit

class CoreUnitTest : public Cppunit
{
    void single_test() override
    {
        using namespace kon;
        CHECK(2 + 2, 4);

        // Vectors
        ArrayList<int> vec;
        vec.Add(4);
        CHECK(vec.Get(0), 4);
        vec.Add(2);
        vec.Add(1);
        CHECK(vec.Get(2), 1);
    }
};

#include <kon/resource/Resource.hpp>
#include <kon/resource/ResourceModel.hpp>

int main(int argc, char const *argv[])
{
	kon::String asdfasd;
	asdfasd = "what is up";

	asdfasd = kon::String("hello").c_str();

    kon::ResourceModel model;
    model.LoadResource("models/viking_room.obj");

    return 0; //(new CoreUnitTest)->run();
}

