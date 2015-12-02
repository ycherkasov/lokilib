#pragma once
#include <loki/Factory.h>

///////////////////////////////////////////////////////////////////////////////
// FactoryTest
///////////////////////////////////////////////////////////////////////////////

namespace FactoryTestPrivate
{
  class Shape
  {
  public:
    virtual ~Shape() = 0;
  };

  inline Shape::~Shape() {}

  class Polygon : public Shape
  {
  };

  class Line : public Shape
  {
  };

  class Circle : public Shape
  {
  };

  Polygon *createPolygon() { return new Polygon; }
  Line *createLine() { return new Line; }
  Circle *createCircle() { return new Circle; }

  Polygon *clonePolygon(Polygon *) { return new Polygon; }
  Line *cloneLine(Line *) { return new Line; }
  Circle *cloneCircle(Circle *) { return new Circle; }

  typedef Loki::Factory<Shape, int> FactoryType;

  bool testFactory()
  {
    FactoryType factory;

    factory.Register(1, (Shape * (*)()) createPolygon);
    factory.Register(2, (Shape * (*)()) createCircle);
    factory.Register(3, (Shape * (*)()) createLine);

    Shape *s;

    s = factory.CreateObject(1);
    delete s;
    bool test1=s!=NULL;

    s = factory.CreateObject(2);
    delete s;
    bool test2=s!=NULL;

    s = factory.CreateObject(3);
    delete s;
    bool test3=s!=NULL;

    return test1 && test2 && test3;
  }

  typedef Loki::CloneFactory<Shape> CloneFactoryType;

  bool testCloneFactory()
  {
    CloneFactoryType factory;

    factory.Register(Loki::TypeInfo(typeid(Polygon)), (Shape * (*)(const Shape *)) clonePolygon);
    factory.Register(Loki::TypeInfo(typeid(Circle)), (Shape * (*)(const Shape *)) cloneCircle);
    factory.Register(Loki::TypeInfo(typeid(Line)), (Shape * (*)(const Shape *)) cloneLine);

    Polygon p;
    Circle c;
    Line l;

    Shape *s;

    s = factory.CreateObject(&p);
    delete s;
    bool test1=s!=NULL;

    s = factory.CreateObject(&c);
    delete s;
    bool test2=s!=NULL;

    s = factory.CreateObject(&l);
    delete s;
    bool test3=s!=NULL;

    return test1 && test2 && test3;
  }
}

class FactoryTest : public Test
{
public:
  FactoryTest() : Test("Factory.h") {}

  virtual void execute(TestResult &result)
    {
    printName(result);

    bool test1=FactoryTestPrivate::testFactory();

    bool test2=FactoryTestPrivate::testCloneFactory();

    bool r=test1 && test2;

    testAssert("Factory",r,result);

    std::cout << '\n';
    }
} factoryTest;

