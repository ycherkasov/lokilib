#pragma once
#include <loki/Functor.h>

///////////////////////////////////////////////////////////////////////////////
// FunctorTest
///////////////////////////////////////////////////////////////////////////////

class FunctorTest : public Test
{
public:
  FunctorTest() : Test("Functor.h") {}

  virtual void execute(TestResult &result)
    {
    printName(result);

    using namespace Loki;

    bool r;

    TestFunctor testFunctor;
    TestClass testClass;

    Functor<void,TYPELIST_1(bool &)> function(testFunction);
    Functor<void,TYPELIST_1(bool &)> functor(testFunctor);
    Functor<void,TYPELIST_1(bool &)> classFunctor(&testClass,&TestClass::member);
    Functor<void,TYPELIST_1(bool &)> functorCopy(function);
    Functor<void,NullType> bindFunctor(BindFirst(function,testResult));
    Functor<void> chainFunctor(Chain(bindFunctor,bindFunctor));

    testResult=false;
    function(testResult);
    bool functionResult=testResult;

    testResult=false;
    functor(testResult);
    bool functorResult=testResult;

    testResult=false;
    classFunctor(testResult);
    bool classFunctorResult=testResult;

    testResult=false;
    functorCopy(testResult);
    bool functorCopyResult=testResult;

    testResult=false;
    bindFunctor();
    bool bindFunctorResult=testResult;

    testResult=false;
    chainFunctor();
    bool chainFunctorResult=testResult;

    r=functionResult && functorResult && classFunctorResult && functorCopyResult && bindFunctorResult &&
      chainFunctorResult;

    testAssert("Functor",r,result);

    std::cout << '\n';
    }

private:
  static bool testResult;

  static void testFunction(bool &result)
    {
    result=true;
    }

  class TestFunctor
  {
  public:
    void operator()(bool &result)
      {
      result=true;
      }
  };

  class TestClass
  {
  public:
    void member(bool &result)
      {
      result=true;
      }
  };
} functorTest;

bool FunctorTest::testResult;

