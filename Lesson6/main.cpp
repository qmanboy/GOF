#include <iostream>
#include <vector>

//Composite
class MenuItem
{
public:
    virtual int getPrice() = 0;
    virtual std::string getName() = 0;
    virtual ~MenuItem() = default;
};

class Pancake: public MenuItem{
private:
   std::string name;
   float price;
public:
   Pancake(const std::string &name, float price) : name(name), price(price) {}
   int getPrice(){
       return price;
   }
   std::string getName(){
       return name;
   }
};

class Icecream: public MenuItem{
private:
   std::string name;
   float price;
public:
   Icecream(const std::string &name, float price) : name(name), price(price) {}
   int getPrice(){
       return price;
   }
   std::string getName(){
       return name;
   }
};

class Beverage: public MenuItem{
private:
   std::string name;
   float price;
public:
   Beverage(const std::string &name, float price) : name(name), price(price) {}
   int getPrice(){
       return price;
   }
   std::string getName(){
       return name;
   }
};

class CompositeMenuItem: public MenuItem{
private:
    std::vector<MenuItem*> m_items;
public:
    void addItem(MenuItem* _item){
        m_items.push_back(_item);
    }

    int getPrice() override{
        int total = 0;
        for (const auto& item : m_items){
            total += item->getPrice();
        }
        return total;
    }

    std::string getName() override{
        std::string total = "";
        for (const auto& item : m_items){
            total += item->getName() + "\n";
        }
        return total;
    }

    void showinfo(){
        std::cout << "Items: \n" << this->getName() << "Total Price: " << this->getPrice() << std::endl;
    }

    virtual ~CompositeMenuItem(){
        for (const auto& item : m_items){
            delete item;
        }
    }

};

CompositeMenuItem* breakfastCombo()
{
    CompositeMenuItem* bfCombo = new CompositeMenuItem;
    bfCombo->addItem(new Pancake("Morning pancake", 240.0f));
    bfCombo->addItem(new  Icecream("Banana Split", 260));
    bfCombo->addItem(new Beverage("Coffee", 80));
    return bfCombo;
};

CompositeMenuItem* lunchCombo()
{
    CompositeMenuItem* lCombo = new CompositeMenuItem;
    lCombo->addItem(new Pancake("Lunch pancake", 280.0f));
    lCombo->addItem(new  Icecream("Hot Brauni", 310));
    lCombo->addItem(new Beverage("Pepsi", 120));
    return lCombo;
};

CompositeMenuItem* lastCombo()
{
    CompositeMenuItem* lstCombo = new CompositeMenuItem;
    lstCombo->addItem(new Pancake("Potato pancake", 210.0f));
    lstCombo->addItem(new  Icecream("Hot Brauni", 310));
    lstCombo->addItem(new Beverage("Pepsi", 120));
    return lstCombo;
};

CompositeMenuItem* doubleCombo(){
    CompositeMenuItem* dblCombo = new CompositeMenuItem;
    dblCombo->addItem(breakfastCombo());
    dblCombo->addItem(lunchCombo());
    return dblCombo;
};

//============================================================
//Mediator

class Calendar{
private:
    bool state = true;
public:
   bool checkCalendar(){
       return state;  // Упрощенная реализация, должен возвращать true, если рабочий день.
   }
   void changeDays()
   {
       state = !state;
   }

};

class Device
{
public:
    virtual void doWork() = 0;
    virtual ~Device() = default;
};

void notFound(std::string _devicename) 
{
    std::cout << _devicename << " not found." << std::endl;
}
class Coffee;
class TeaPot;
class Alarm;

class Mediator
{
private:
    Calendar* cal;
    Device* alarm;
    Device* coffee;
    Device* teapot;
public:
    Mediator(Calendar* _cal) : cal(_cal), alarm(nullptr), coffee(nullptr), teapot(nullptr){}
    void addAlarm(Device* _alarm){
        alarm = _alarm;
    }
    void addCoffee(Device* _coffee){
        coffee = _coffee;
    }
    void addTeapot(Device* _teapot){
        teapot = _teapot;
    }
    void Work()
    {   
        if (alarm != nullptr) 
        {
            alarm->doWork(); 
            if (cal->checkCalendar())
            {

                (coffee != nullptr) ? coffee->doWork() : notFound("Coffee");
            }
            else 
            {
                (teapot != nullptr) ? teapot->doWork() : notFound("Teapot");
            }
        } else 
            notFound("Alarm");
    }
};

class Coffee: public Device{
private:
   Mediator* pMediator;
public:
   Coffee(Mediator* pMed) : pMediator(pMed) {}

   void doWork() override{
       std::cout << "Starting coffee" << std::endl;
   }
   void activate(){
        pMediator->Work();
   }
    
};

class TeaPot: public Device{
private:
   Mediator* pMediator;
public:
   TeaPot(Mediator* pMed) : pMediator(pMed) {}

   void doWork() override {
       std::cout << "Starting tea" << std::endl;
   }

    void activate(){
        pMediator->Work();
    }
};

class Alarm: public Device{
private:      
    Mediator* pMediator;
public:
   Alarm(Mediator* pMed) : pMediator(pMed) {}

   void doWork() override{
       std::cout << "Rise and shine, Mr. Freeman" << std::endl;
   }
   
   void activate(){
        pMediator->Work();
    }
};

//============================================================
//Template_method

struct GraphElement{
   int x;
   int y;
   std::string property;
   void draw(){
       std::cout << "("<< x << ", " << y << ", " << property << ") "; //отрисовка пискселя/фигуры
   }
};

struct Pixel : public GraphElement{};

struct Figure : public GraphElement{};

class GraphicEditor{
private:
   std::vector<GraphElement*> elements;     // это пиксели/фигуры, которые хранятся в файле
   std::vector<GraphElement> buffer;
public:
    void openFile(const std::string& filename){
       std::cout << "Open file " << filename << std::endl;
   }

   void readFile(){
       for (const auto element: elements) {
           buffer.push_back(*element);      // Имитируем чтение пикселей/фигур из файла в буфер
       }
   };

   void draw(){
       for (auto element: buffer) {
           element.draw();      // Рисуем каждый пиксель/фигуру из буфера
       }
   }
  
   void final(){
       std::cout << "Drawing image complete" << std::endl;
   }

   void drawImage(const std::string& filename){
       openFile(filename);
       readFile();
       draw();
       final();
   };
};

class BitmapImage: public GraphicEditor{};        // Растровое изображение

class VectorImage: public GraphicEditor{};       // Векторное изображение


int main(){
   
   CompositeMenuItem* bfCombo = new CompositeMenuItem;
   CompositeMenuItem* lCombo = new CompositeMenuItem;
   CompositeMenuItem* lstCombo = new CompositeMenuItem;
   CompositeMenuItem* dblCombo = new CompositeMenuItem;

   bfCombo->addItem(breakfastCombo());
   lCombo->addItem(lunchCombo());
   lstCombo->addItem(lastCombo());
   dblCombo->addItem(doubleCombo());

   bfCombo->showinfo();
   lCombo->showinfo();
   lstCombo->showinfo();
   dblCombo->showinfo();

   delete bfCombo;
   delete lCombo;
   delete lstCombo;
   delete dblCombo;

   std::cout << "//=====================================================" << std::endl;

   Calendar cal;
   Mediator med(&cal);
   Alarm alarm(&med);
   Coffee coffee(&med);
   TeaPot teapot(&med);

   med.addAlarm(&alarm);
   med.addCoffee(&coffee);
   med.addTeapot(&teapot);
   med.Work();
   cal.changeDays();
   med.Work();

   std::cout << "//=====================================================" << std::endl;
   
    BitmapImage bitmap;
    bitmap.drawImage("bitmap.jpg");
    VectorImage vector;
    vector.drawImage("vector.svg");

   return 0;
}

