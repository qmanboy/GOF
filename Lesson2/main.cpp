#include <iostream>
#include <vector>
#include <algorithm>

class Pizza{
public:
   virtual void info() {};
   virtual void bake() {};
   virtual void cut() {};
   virtual void box() {};
   virtual ~Pizza() {};
};

class CheesePizza:public Pizza
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
class GreekPizza:public Pizza
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

Pizza* orderPizza(PizzaFactory* _factory)
{
    return _factory->createPizza();
};

//=======================================================

enum class Colors {
   RED,
   ORANGE,
   YELLOW,
   GREEN,
   CYAN,
   BLUE,
   VIOLET,
   WHITE
};

std::ostream& operator << (std::ostream& out, const Colors& _color)
{
    switch(_color) {
        case Colors::RED: return (out << "RED");
        case Colors::ORANGE: return (out << "ORANGE");
        case Colors::YELLOW: return (out << "YELLOW");
        case Colors::GREEN: return (out << "GREEN");
        case Colors::CYAN: return (out << "CYAN");
        case Colors::BLUE: return (out << "BLUE");
        case Colors::VIOLET: return (out << "VIOLET");
        case Colors::WHITE: return (out << "WHITE");

   }
   return (out);
}

class Light
{    
private:
   bool state;
   Colors color;

public:
   Light():state(false), color(Colors::WHITE){};
   void On(){
       if(!state){
           state = true;
       }
   };

   void Off(){
       if(state){
           state = false;
       }
   };
  
   void changeColor(Colors newColor){
       color = newColor;
   }
  
   bool getState() const {
       return state;
   }

   Colors getColor() const 
   {
       return color;
   }

};

class Command 
{
protected:
    Light* m_light;
public:
    virtual void Execute() = 0;
    virtual void unExecute() = 0;
    void setLight(Light* _light) 
    {
        m_light = _light;
    }
    virtual ~Command(){};
};


class comLightPower : public Command  
{
public:
    void Execute() override
    { 
        m_light->On();        
        std::cout << "state: " << m_light->getState() << std::endl;
    }
    void unExecute() override
    { 
        m_light->Off();
        std::cout << "undo state: " << m_light->getState() << std::endl;
    }
};

class comLightChangeColor : public Command  
{
private:
    Colors m_color;
    Colors backup_color;
public:
    comLightChangeColor(Colors _color) : m_color(_color)
    {
    }
    void Execute() override
    {         
        backup_color = m_light->getColor();
        m_light->changeColor(m_color);        
        std::cout << "color: " << m_light->getColor() << std::endl;
    }
    void unExecute() override
    { 
        m_light->changeColor(backup_color);
        std::cout << "unexe color: " << m_light->getColor() << std::endl;
    }
};

class Invoker
{
private:
    std::vector<Command*> CommandHistory;
    size_t HistorySize;
    Light m_light;
    Command* m_command;
public:
    Invoker() : m_command(nullptr) {};
    ~Invoker()
    {
        for (auto command : CommandHistory)
        {
            delete command;
        }
    }

    void PowerON() 
    {
        m_command = new comLightPower;
        m_command->setLight(&m_light);
        m_command->Execute();
        CommandHistory.push_back(m_command);
    }

    void ChangeColor(Colors _color) 
    {
        m_command = new comLightChangeColor(_color);
        m_command->setLight(&m_light);
        m_command->Execute();
        CommandHistory.push_back(m_command);
    }

    void Undo() 
    {
        if (CommandHistory.size() > 0) 
        {
            m_command = CommandHistory.back();
            CommandHistory.pop_back();
            m_command->unExecute();
            delete m_command;
        }
        else 
        {
            std::cout << "History is empty" << std::endl;
        }
    }

    void PlayHistory() 
    {
        for (auto command : CommandHistory) 
        {
            command->Execute();
        }
    }

    void PlayReverseHistory() 
    {
        std::for_each(CommandHistory.rbegin(), CommandHistory.rend(),
        [](auto& element)
        {
            return element->Execute();
        });
    }

};

//========================================================

class Beverage{            // Абстрактный класс
public:
   //virtual std::string getDescription() const;
   virtual double cost() const = 0;
   virtual ~Beverage() {};
};

class HouseBlend: public Beverage{   // Молотый
public:
   double cost() const override {
       return 5.5;
   }
};

class DarkRoast: public Beverage{  // Темной обжарки
public:
   double cost() const override {
       return 6.0;
   }
};
 
class Decaf: public Beverage{    // Без кофеина
public:
   double cost() const override {
       return 7.3;
   }
};

class Espresso: public Beverage{  // Эспрессо
public:
   double cost() const override {
       return 3.5;
   }
};

class Decorator : public Beverage 
{
protected:
    Beverage* m_beverage;
public:
    Decorator(Beverage* _beverage) : m_beverage(_beverage){};
    double cost() const override 
    {
        return m_beverage->cost();
    } 
};

class Chocolate : public Decorator 
{
public:
    Chocolate(Beverage* beverage): Decorator(beverage){};
    double cost() const override
    {
        return Decorator::cost() + 1.0;
    }
};

class Сinnamon : public Decorator 
{
public:
    Сinnamon(Beverage* beverage): Decorator(beverage){};
    double cost() const override
    {
        return Decorator::cost() + 0.8;
    }
};

class Cream : public Decorator 
{
public:
    Cream(Beverage* beverage): Decorator(beverage){};
    double cost() const override
    {
        return Decorator::cost() + 1.5;
    }
};

class Sugarless : public Decorator 
{
public:
    Sugarless(Beverage* beverage): Decorator(beverage){};
    double cost() const override
    {
        return Decorator::cost() - 0.5;
    }
};

void Order(Beverage* _beverage)
{
std::cout << "Result price: " << _beverage->cost() << std::endl;
}

int main() 
{
    PizzaFactory* cheesePizza= new CreateCheesePizza;
    PizzaFactory* greekPizza= new CreateGreekPizza;
    PizzaFactory* pepperoniPizza= new CreatePepperoniPizza;

    PrintOrder(orderPizza(cheesePizza));
    PrintOrder(orderPizza(greekPizza));
    PrintOrder(orderPizza(pepperoniPizza));

    delete cheesePizza;
    delete greekPizza;
    delete pepperoniPizza;

    std::cout << "//========================================================" << std::endl;

    Invoker inv;
    inv.PowerON();
    inv.ChangeColor(Colors::GREEN);
    inv.ChangeColor(Colors::RED);
    inv.PowerON();
    inv.PowerON();
    inv.ChangeColor(Colors::BLUE);
    inv.ChangeColor(Colors::VIOLET);
    inv.Undo();
    std::cout << "===================" << std::endl;
    inv.PlayHistory();
    std::cout << "===================" << std::endl;
    inv.PlayReverseHistory();

    std::cout << "//========================================================" << std::endl;

    Beverage* houseblend = new HouseBlend;
    Order(houseblend);
    Beverage* houseblend_with_chocolate = new Chocolate(houseblend);
    Order(houseblend_with_chocolate);
    Beverage* houseblend_with_chocolate_sugarless = new Sugarless(houseblend_with_chocolate);
    Order(houseblend_with_chocolate_sugarless);

    delete houseblend;
    delete houseblend_with_chocolate;
    delete houseblend_with_chocolate_sugarless;

    return 0;
};