#include <iostream>
#include <vector>
#include <string>

//Facade

class Device{
public:
   std::string type;

   Device(const std::string &type) : type(type) {}
   virtual void On(){
       std::cout << type <<" is on" << std::endl;
   }
   virtual void Off(){
       std::cout << type <<" is off" << std::endl;
   }


};

class MediaPlayer: public Device{
private:
   std::string currentMovie;
public:
   MediaPlayer(const std::string &type) : Device(type) {}
   void play(std::string newMovie) {
       currentMovie = newMovie;
       std::cout << "Movie " << currentMovie << " started" << std::endl;

   }
};

class Amplifier: public Device{
   Device* streamingPLayer;
   int volume;
public:
   Amplifier(const std::string &type) : Device(type), volume(0) {}
   void setStreamingPlayer(Device* d){
       streamingPLayer = d;
       std::cout << "Streaming player is " << d->type << std::endl;
   }

   void setSurroundSound(){
       std::cout << "Surround Sound Mode" << std::endl;
   };

   void setVolume(int v){
       volume = v;
   }

};

class Projector: public Device{
private:
   Amplifier* _amplifier;
   Device* _input;
public:
   Projector(const std::string &type) : Device(type) {}

   void setInput(Device* d){
       _input = d;
       std::cout << "Projector input is " << d->type << std::endl;
   }
   void wideScreenMode(){
       std::cout << "Wide Screen Mode" << std::endl;
   };
};

class Tuner{
private:
   Amplifier* _amplifier;
public:
   void on(){
       std::cout << "Tuner on" << std::endl;
   }

   void off(){
       std::cout << "Tuner off" << std::endl;
   }
};

class Popper: public Device{
public:
   Popper(const std::string &type) : Device(type) {}

   void pop(){
       std::cout << "Popcorn will be ready soon" << std::endl;
   }
};

class Lights: public Device{
private:
   int currentLight;
public:
   Lights(const std::string &type) : Device(type), currentLight(100) {}
   void dim(int light){
       currentLight = light;
   }
};

class Screen: public Device{
public:
   Screen(const std::string &type) : Device(type) {}
   void down(){
       std::cout << "Screen is down" << std::endl;
   };
   void up(){
       std::cout << "Screen is up" << std::endl;
   };
};

class DeviceFacade 
{
private:
    MediaPlayer* m_mp;
    Popper* m_popper;
    Lights* m_light;
    Projector* m_projector;
    Screen* m_screen;
    Amplifier* m_amp;
    bool m_state = false;
public:
    DeviceFacade()
    {
        m_mp = new MediaPlayer("media player");
        m_popper = new Popper("popper");
        m_light = new Lights("light");
        m_projector = new Projector("projector");
        m_screen = new Screen("screen");
        m_amp = new Amplifier("amplifier");
    }

    ~DeviceFacade() 
    {
        delete m_mp;
        delete m_popper;
        delete m_light;
        delete m_projector;
        delete m_screen;
        delete m_amp;
    }

    void On()
    {
        m_state = true;
    }

    void WatchFilm(std::string _filmName)
    {
        if (m_state) // если домашний кинотеатр не включен, не будет никакой реакции
        {             
            m_state = true;
            m_popper->On();                    // Включаем попкорн машину
            m_popper->pop();                   // Готовим попкорн
            
            m_light->dim(0);              // Глушим свет
            m_screen->down();                  // Опускаем экран
            
            m_projector->On();                 // Включаем проектор
            m_projector->setInput(m_mp);       // Выбираем вход проектора
            m_projector->wideScreenMode();     // Широкоэкранный режим
            
            m_amp->On();                       // Включаем усилитель
            m_amp->setStreamingPlayer(m_mp);   // Выбираем куда отправлять звук
            m_amp->setSurroundSound();         // Режим объемного звука
            m_amp->setVolume(5);             // Средняя громкость
            
            m_mp->On();                       // Включаем медиаплеер            
            m_mp->play(_filmName);   
        }
    }

    void Off()
    {
        if (m_state) 
        {
            m_mp->Off();
            m_amp->Off();
            m_projector->Off();
            m_screen->up();
            m_light->On();
            m_popper->Off();
        }
    }
};

//==========================================================
//Visitor

class Ingredient;

class Visitor{
public:
    virtual void getState(class MenuItem* m) = 0;
    virtual void getState(class Ingredient* i) = 0;
    virtual ~Visitor() = default;
};

class Menu{
public:
    virtual ~Menu() = default;
    virtual void AcceptVisitor(Visitor*) = 0;
};

class MenuItem: public Menu{
private:
   std::string name;
   std::string description;
   float price;
   std::vector<Ingredient*> ingredients;
public:
   MenuItem(const std::string &name, const std::string &description, float price) : name(name),
                                                                                    description(description),
                                                                                    price(price) {}

   const std::string &getName() const {
       return name;
   }

   const std::string &getDescription() const {
       return description;
   }

   float getPrice() const {
       return price;
   }

    void AcceptVisitor(Visitor* _visitor) override{
        _visitor->getState(this);
    }
};

class Ingredient: public Menu{
private:
   std::string name;
   int healthRating;
   float Calories = 420;
   float Protein;
   float Carbs;
   float Fats;
public:
    float getCalories() const {
       return Calories;
   }
    void AcceptVisitor(Visitor* _visitor) override{
        _visitor->getState(this);
    }
};

class Client: public Visitor{
public:
    void getState(MenuItem* _item) override {
        std::cout << _item->getName() << "\n"
                  << _item->getDescription() << "\n"
                  <<_item->getPrice() << "\n" 
                  << std::endl;
    }
    void getState(Ingredient* _ingredient) override{
        std::cout << "Calories: " 
                  << _ingredient->getCalories() 
                  << std::endl;
    }
};

//==========================================================
//Observer

class IObserver {
 public:
  virtual ~IObserver(){};
  virtual void listen(int _voicePower, const std::string &message_from_subject) = 0;
};

class ISubject {
 public:
  virtual ~ISubject(){};
  virtual void AttachObserver(IObserver *observer) = 0;
  virtual void DetachObserver(IObserver *observer) = 0;
  virtual void Notify() = 0;
};

class ICitizen{
public:
   virtual void hearing() = 0;
   virtual int getHearingPower() = 0;
};

class Herald : public ISubject{
private:
   int voicePower;
   std::string announcement;
   std::vector<IObserver*> observers;

public:

   void AttachObserver(IObserver* _observer) override
   {
       observers.push_back(_observer);
   };

   void DetachObserver(IObserver* _observer) override
   {
        for(auto it = observers.begin(); it != observers.end(); ++it)
        {
            if(*it == _observer) observers.erase(it);
        }
   };

   std::string getAnnouncement() { return announcement; }
   void setVoicePower(int _voicePower)
   {
       voicePower = _voicePower;
   }
   void setAnnouncement(std::string _announcement)
   {
       std::cout << "Herald anounced: " << _announcement << std::endl;
       announcement = _announcement;
       Notify();
   }
   void Notify()
   {
       for (const auto& observer : observers) 
       {
           observer->listen(voicePower, announcement);
       }
   }
};

class Doctor: public ICitizen, IObserver {
private:
   int hearingPower = 100;
   std::string message;
   ISubject* herald;
public:
    Doctor(ISubject* _herald) : herald(_herald) 
    {
        _herald->AttachObserver(this);
    }
   void listen(int voicePower, const std::string& _message) override 
   {
       if (voicePower > hearingPower) 
       {
            message = _message;
            this->hearing();
       }
   }
   int getHearingPower() 
   {
       return hearingPower;
   }
   void hearing()
   {
        std::cout << "Doctor heard: " << message << std::endl;
   }
};

class Blacksmith: public ICitizen, IObserver {
private:
   int hearingPower = 30;
   std::string message;
   ISubject* herald;
public:
    Blacksmith(ISubject* _herald) : herald(_herald) 
    {
        _herald->AttachObserver(this);
    }
   void listen(int voicePower, const std::string& _message) override 
   {
       if (voicePower > hearingPower) 
       {
            message = _message;
            this->hearing();
       }
   }
   int getHearingPower() 
   {
       return hearingPower;
   }
   void hearing()
   {
        std::cout << "Blacksmith heard: " << message << std::endl;
   }
};

class Child: public ICitizen, IObserver {
private:
   int hearingPower = 50;
   std::string message;
   ISubject* herald;
public:
    Child(ISubject* _herald) : herald(_herald) 
    {
        _herald->AttachObserver(this);
    }
   void listen(int voicePower, const std::string& _message) override 
   {
       if (voicePower > hearingPower) 
       {
            message = _message;
            this->hearing();
       }
   }
   int getHearingPower() 
   {
       return hearingPower;
   }
   void hearing()
   {
        std::cout << "Child heard: " << message << std::endl;
   }
};

int main() {

   DeviceFacade HomeTheatre;
   HomeTheatre.On();
   HomeTheatre.WatchFilm("Star Wars");
   HomeTheatre.Off();

   std::cout << "//=========================================" << std::endl;

    MenuItem* menu = new MenuItem("Item1", "1", 10.5);
    Client* client = new Client;
    menu->AcceptVisitor(client);
    Ingredient* ingredient = new Ingredient;
    ingredient->AcceptVisitor(client);

    delete ingredient;
    delete client;
    delete menu;
    
   std::cout << "//=========================================" << std::endl;

    Herald* herald = new Herald;
    Doctor* doctor = new Doctor(herald);
    Blacksmith* blacksmith = new Blacksmith(herald);
    Child* child = new Child(herald);
    herald->setVoicePower(45);
    herald->setAnnouncement("Good news, we are freedom!");

    delete child;
    delete blacksmith;
    delete doctor;
    delete herald;    

   return 0;
}

