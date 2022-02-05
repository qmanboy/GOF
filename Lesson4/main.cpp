#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

//Fabric Method

class Pizza{
public:
   virtual void info() {};
   virtual void bake() {};
   virtual void cut() {};
   virtual void box() {};
   virtual ~Pizza() {};
};

class CheesePizza: public Pizza
{
public:
    void info() override
    {
        std::cout<< "Cheese Pizza" << std::endl;
    };
    void bake() override {};
    void cut() override {};
    void box() override {};
};
class GreekPizza: public Pizza
{
public:
    void info() override  
    {
        std::cout<< "Greek Pizza" << std::endl;
    };
    void bake() override {};
    void cut() override {};
    void box() override {};
};
class PepperoniPizza: public Pizza
{
public:
    void info() override
    {
        std::cout<< "Pepperoni Pizza" << std::endl;
    };
    void bake() override {};
    void cut() override {};
    void box() override {};
};

class PizzaFactory 
{
public:
    virtual Pizza* createPizza() = 0;    
    virtual ~PizzaFactory() {};
};

class CreateCheesePizza: public PizzaFactory
{
public:

    Pizza* createPizza() 
    {
        return new CheesePizza;
    }
};

class CreateGreekPizza: public PizzaFactory
{
public:
    Pizza* createPizza() 
    {
        return new GreekPizza;
    }
};

class CreatePepperoniPizza: public PizzaFactory 
{
public:
    Pizza* createPizza() 
    {
        return new PepperoniPizza;
    }
};

void PrintOrder(Pizza* _pizza) 
{
    _pizza->info();
};

//=======================================================
//Builder

class Events{
public:
   virtual void getEvent() = 0;
   virtual ~Events() {};
};

class Hotel: public Events{
public:
   void getEvent() {
       std::cout << "Booking Hotel" << std::endl;
   }
};

class Park: public Events{
   void getEvent() {
       std::cout << "Park visiting" << std::endl;
   }
};

class Food: public Events{
public:
   virtual void getEvent() = 0;
   virtual ~Food() {};
};

class Dinner: public Food{
public:
   Dinner() : _type("dinner") {}
   void getEvent(){
       std::cout << "Food " << _type << std::endl;
   } ;
private:
   std::string _type;
};

class Lunch: public Food{
public:
   Lunch() : _type("lunch") {}
   void getEvent(){
       std::cout << "Food " << _type << std::endl;
   } ;
private:
   std::string _type;
};

class Breakfast: public Food{
public:
   Breakfast() : _type("breakfast") {}
   void getEvent(){
       std::cout << "Food " << _type << std::endl;
   } ;
private:
   std::string _type;
};

class Special: public Events{
public:
   virtual void getEvent() = 0;
   virtual ~Special() {};
};

class Skating:public Special{
public:
   Skating() : _type("skating") {}
   void getEvent(){
       std::cout << "Special " << _type << std::endl;
   } ;
private:
   std::string _type;
};

class Circus:public Special{
public:
   Circus() : _type("circus") {}
   void getEvent(){
       std::cout  << "Special " << _type << std::endl;
   } ;
private:
   std::string _type;
};

struct Day //структура день, содержащая векторы событий всех типов
{
    size_t m_number;
    std::vector<Events*> events;
    std::vector<Food*> food;
    std::vector<Special*> special;
    void info() 
    {
        std::cout << "\nDay " << m_number << " info:" << std::endl; 
        for (auto& event : events) event->getEvent();        
        for (auto& event : food) event->getEvent();
        for (auto& event : special) event->getEvent();
    }
    ~Day() 
    {
        for (auto& event :  events) delete event;        
        for (auto& event :  food) delete event;
        for (auto& event :  special) delete event;
    }
};

class DayBuilder
{
protected:
    Day* m_day;
public:
    DayBuilder(): m_day(nullptr) {};
    virtual ~DayBuilder() {};
    virtual void createDay() {};
    virtual void planEvents() {};
    virtual void planFood() {};
    virtual void planSpecial() {};
    virtual Day* getDay() { return m_day; }
};

class Day1Builder: public DayBuilder 
{
public:
    void createDay() override { m_day = new Day; m_day->m_number = 1;}
    void planEvents() override 
    {
        m_day->events.push_back(new Hotel);
        m_day->events.push_back(new Park);
        m_day->food.push_back(new Dinner);
    }
};

class Day2Builder: public DayBuilder 
{
public:
    void createDay() override { m_day = new Day; m_day->m_number = 2;}
    void planEvents() override 
    {
        m_day->events.push_back(new Hotel);
        m_day->events.push_back(new Park);
        m_day->food.push_back(new Breakfast);
        m_day->food.push_back(new Dinner);
        m_day->special.push_back(new Skating);
    }
};

class Day3Builder: public DayBuilder 
{
public:
    void createDay() override { m_day = new Day; m_day->m_number = 3;}
    void planEvents() override 
    {
        m_day->events.push_back(new Park);
        m_day->food.push_back(new Breakfast);
        m_day->food.push_back(new Lunch);
        m_day->special.push_back(new Circus);
    }
};

class Planner
{
public:
    Day* createDay(DayBuilder& _builder) 
    {
        _builder.createDay();
        _builder.planEvents();
        _builder.planFood();
        _builder.planSpecial();
        return (_builder.getDay());
    }
};

//=======================================================
//Bridge

class DrawingImplementor {
public:
    virtual void drawRectangle(double) = 0;
    virtual ~DrawingImplementor() {
    }
};

class PencilDrawingImplementor: public DrawingImplementor
{
public:
    PencilDrawingImplementor() = default;
    void drawRectangle(double _borderwidth) 
    {
        std::cout << "Draw Rectangle with pencil. Width = " << _borderwidth << std::endl;
    }
};

class BrushDrawingImplementor: public DrawingImplementor
{
public: 
    BrushDrawingImplementor() = default;
    void drawRectangle(double _borderwidth) 
    {
        std::cout << "Draw Rectangle with brush. Width = " << _borderwidth << std::endl;
    }
};



class Shape 
{
public:
    virtual void draw()= 0; // low-level
    virtual void resize(double pct) = 0; // high-level
    virtual ~Shape() {
    }
};

class Rectangle: public Shape 
{
private:
    double m_borderwidth;
    DrawingImplementor* m_implementor;
public:
    Rectangle(double _borderwidth, DrawingImplementor* _implementor) 
    : m_implementor(_implementor), m_borderwidth(_borderwidth) {};
    virtual ~Rectangle() =default;
    void draw() 
    {
        this->m_implementor->drawRectangle(m_borderwidth);
    }
    void resize(double _pct)
    {
        m_borderwidth *= _pct;
    }
};


int main() 
{
    PizzaFactory* cheesePizza= new CreateCheesePizza;
    PizzaFactory* greekPizza= new CreateGreekPizza;
    PizzaFactory* pepperoniPizza= new CreatePepperoniPizza;

    PizzaFactory* pFactory;
    pFactory = cheesePizza;
    PrintOrder(pFactory->createPizza());
    pFactory = greekPizza;
    PrintOrder(pFactory->createPizza());
    pFactory = pepperoniPizza;
    PrintOrder(pFactory->createPizza());

    delete cheesePizza;
    delete greekPizza;
    delete pepperoniPizza;

    std::cout << "//=======================================================" << std::endl;

    Planner planner;

    Day1Builder day1builder;
    Day2Builder day2builder;
    Day3Builder day3builder;

    Day* day1 = planner.createDay(day1builder);
    Day* day2 = planner.createDay(day2builder);
    Day* day3 = planner.createDay(day3builder);

    day1->info();
    day2->info();
    day3->info();
    
    std::cout << "\n//=======================================================" << std::endl;

    DrawingImplementor* penImpltr = new PencilDrawingImplementor;
    DrawingImplementor* brushImpltr = new BrushDrawingImplementor;

    Rectangle rec1(5.5, penImpltr);
    Rectangle rec2(10.7, brushImpltr);

    rec1.draw();
    rec2.draw();

    delete penImpltr;
    delete brushImpltr;

    return 0;
}