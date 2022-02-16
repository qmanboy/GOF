#include <iostream>
#include <vector>
#include <string>

class CoffeeMachine;

class State
{
private:
    std::string m_statename;
public:
    State(const std::string& _name) : m_statename(_name){}

    const std::string& getStateName() const
    {
        return m_statename;
    }

    virtual void insert_coin(CoffeeMachine* _cm){};
    virtual void return_coin(CoffeeMachine* _cm){};
    virtual void button_is_pushed(CoffeeMachine* _cm){};
    virtual void coffee_is_over(CoffeeMachine* _cm){};
    virtual void coffee_a_lot(CoffeeMachine* _cm){};
};

class CoffeeMachine
{
private:
    bool m_buybutton;
    State* m_state;    
    int m_coffeecount;
    
    void CheckCoffeeCount()
    {
        if (m_coffeecount > 0)
            m_state->coffee_a_lot(this);
        else 
            m_state->coffee_is_over(this);
    }

public:
    CoffeeMachine(State* _state, int _coffeecount) : m_state(_state), m_buybutton(false), m_coffeecount(_coffeecount)
    {
        CheckCoffeeCount();
    }

    void InsertCoin() 
    {
        std::cout << "Client inserted coin..." << std::endl;        
        m_state->insert_coin(this);
        if (m_buybutton){
            m_state->button_is_pushed(this);
            CheckCoffeeCount();
        } 
        else {
            std::cout << "Coffee machine returned coin..." << std::endl;   
            m_state->return_coin(this);
        }
    }

    void PushBuyButton()
    {
        std::cout << "Client push buy button..." << std::endl;
        m_buybutton = true;
        m_coffeecount--;
    }

    State* GetState()
    {
        return m_state;
    }

    void SetState(State* _newState)
    {
        std::cout << "Coffee machine: " << _newState->getStateName() << std::endl;
        delete m_state;
        m_state = _newState;
    }

    void Refill(int _coffeecount){
        std::cout << "Coffee machine is refilled..." << std::endl;
        m_coffeecount = _coffeecount;
        CheckCoffeeCount();
    }

    virtual ~CoffeeMachine() 
    {
        delete m_state;
    }
};

class NonCoin : public State
{
public:
    NonCoin() : State("Push button and insert a coin..."){}
    
    void insert_coin(CoffeeMachine*) override;
};

class Coin : public State
{
public:
    Coin() : State("Coin accepted. Please, wait..."){}

    void return_coin(CoffeeMachine*) override;
    void button_is_pushed(CoffeeMachine*) override;

};

class CoffeeSold : public State
{
public:
    CoffeeSold() : State("Take your coffee."){}

    void coffee_is_over(CoffeeMachine*) override;
    void coffee_a_lot(CoffeeMachine*) override;

};

class NonCoffee : public State
{
public:
    NonCoffee() : State("Sorry, coffee is over..."){}

    void coffee_is_over(CoffeeMachine*) override;
    void coffee_a_lot(CoffeeMachine*) override;
};

void NonCoin::insert_coin(CoffeeMachine* _cm)
{
    _cm->SetState(new Coin);
};

void Coin::return_coin(CoffeeMachine* _cm)
{
    _cm->SetState(new NonCoin);
};

void Coin::button_is_pushed(CoffeeMachine* _cm)
{
    _cm->SetState(new CoffeeSold);
};

void CoffeeSold::coffee_a_lot(CoffeeMachine* _cm)
{
    _cm->SetState(new NonCoin);
};

void CoffeeSold::coffee_is_over(CoffeeMachine* _cm)
{

    _cm->SetState(new NonCoffee);
};

void NonCoffee::coffee_a_lot(CoffeeMachine* _cm)
{
    _cm->SetState(new NonCoin);
};

void NonCoffee::coffee_is_over(CoffeeMachine* _cm)
{

    _cm->SetState(new NonCoffee);
};

//========================================================
//Prototype

class Monster{
private:
   int hp;
   int atk;
   int def;
   std::string type;
public:
   Monster(int hp, int atk, int def, const std::string &type) : hp(hp), atk(atk), def(def), type(type) {}
   Monster() : hp(0), atk(0), def(0), type("Unknown") {}
   Monster(Monster& mnstr)
   {
        hp = mnstr.hp;
        atk = mnstr.atk;
        def = mnstr.def;
        type = mnstr.type;
    }
 
   void changeHp(int change){
       hp += change;
   }

   void changeAtk(int change){
       atk += change;
   }

   void changeDef(int change){
       def += change;
   }

   int getHp() const {
       return hp;
   }

   int getAtk() const {
       return atk;
   }

   int getDef() const {
       return def;
   }

   std::string getType() const {
       return type;
   }

   void levelUp(size_t level){
        changeHp(--level * 3);
        changeAtk(level * 3);
        changeDef(level * 3);
        type += "_" + std::to_string(++level);
   }

   Monster* clone(size_t _level = 1){
       Monster* mnst = new Monster(*this);
       mnst->levelUp(_level);   
       return mnst;
   }

   void printInfo() const{
       std::cout << "_______" 
                 << "\nHp: " << this->getHp() 
                 << "\nAtk: " << this->getAtk()
                 << "\nDef: " << this->getDef() 
                 << "\nType: " << this->getType() << std::endl;
   }

};



int main()
{
    CoffeeMachine* pCFMN = new CoffeeMachine(new CoffeeSold, 1);
    pCFMN->PushBuyButton();
    pCFMN->InsertCoin();
    pCFMN->Refill(5);
    pCFMN->PushBuyButton();
    pCFMN->InsertCoin();
    delete pCFMN;

    std::cout << "//==================================================" << std::endl;

    std::vector<Monster*> monsters;

    monsters.push_back(new Monster(10, 5, 4, "Mob"));
    monsters.push_back(monsters[0]->clone());
    monsters.push_back(monsters[0]->clone(6));

    for(const Monster* ptr: monsters){
        ptr->printInfo();
    }

    for(Monster* ptr: monsters){
        delete ptr;
    }

    return 0;
};

