#ifdef __INTEL_COMPILER
# pragma warning(disable: 111 193 304 383 444 488 981 1418)
#elif defined(_MSC_VER) && !defined(__MWERKS__)
# pragma warning(disable: 4018 4097 4100 4213 4290 4512 4514 4700 4702 4710 4786 4800 4996)
#endif

// Some platforms might have difficulty with this
// Need to ifdef around those cases.

#include "UnitTest.h"

// static variable defintion, do not remove

Test::tests_type Test::tests;

// Merely comment out any of the following headers to
// prevent thier execution during the test.
//
// A pluggable-factory-like method is used to
// auto-register the test, so all that is needed
// is the header inclusion to execute the correspond
// unit test.

#include "TypelistTest.h"
#include "TypeManipTest.h"
#include "TypeTraitsTest.h"
#include "SmallObjectTest.h"
#include "SingletonTest.h"
#include "SmartPtrTest.h"
#include "FactoryTest.h"
#include "AbstractFactoryTest.h"
#include "AssocVectorTest.h"
#include "FunctorTest.h"
#include "DataGeneratorsTest.h"


struct small_class1 : public Loki::SmallObject<>
{
    int i = 0;
};

struct small_class2 : public Loki::SmallObject<>
{
    int i = 0;
    int j = 1;
};

struct small_class3 : public small_class2
{
    int i1 = 2;
    int j1 = 3;
};

void test_allocation()
{

    std::vector<small_class1*> small_classes1;
    std::vector<small_class2*> small_classes2;

    for (size_t i = 0; i < 2; ++i){
        std::cout << "Created size: " << sizeof(small_class1) << std::endl;
        small_class1* s1 = new small_class1;
        small_classes1.push_back(s1);
    }

    for (size_t i = 0; i < 2; ++i){
        small_class1* s1 = small_classes1[i];
        delete s1;
    }
}



int main()
{
#if 0
    test_allocation();
    return 0;
#else

    int result = Test::run("Loki Unit Test");
#if defined(__BORLANDC__) || defined(__GNUC__) || defined(_MSC_VER)
    system("pause"); // Stop console window from closing if run from IDE.
#endif
    return result;

#endif
}
