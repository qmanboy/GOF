#include <iostream>
#include <memory>
#pragma once

class Memento;

class Player{
private:
   class Impl;   
   std::unique_ptr<Impl> pImpl; //PIMPL
   

public:
   Player(float hp, int speed, const std::string &name);
   Player();

   ~Player();
    Player(const Player& other);
    Player& operator=(Player& other);

   //void setName(const std::string& _name);

   void changeSpeed(int change);

   void changeHp(int change);

   float getHp() const;

   int getSpeed() const;

   const std::string &getName() const;

   Memento createMemento();

   void reinstateMemento(Memento& mem);

    void print();

};