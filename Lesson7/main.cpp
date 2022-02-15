#include <iostream>
#include <fstream>
#include <vector>
#include "player.h"

struct Properties{
   float hp;
   int speed;
   std::string name;
};

class Memento{
private:
   float hp;
   int speed;
   std::string name;
   Properties properties; //для удобства сериализации данных
public:
    Memento(float hp, int speed, const std::string &name) : hp(hp), speed(speed), name(name) {}
    Memento(const Memento& _mem) : hp(_mem.hp), speed(_mem.speed), name(_mem.name) {}
    Memento() = default;
    
    float getHp() const {
       return hp;
   }

   int getSpeed() const {
       return speed;
   }

   const std::string &getName() const {
       return name;
   }
    void Save(){
        std::ofstream out;
        out.open("./save.bin", std::ios::binary);
        if (out.is_open()) { 
            properties.hp = hp;
            properties.speed = speed;
            properties.name = name;
            out.write((char*)&properties, sizeof(Properties));        
            std::cout << "File saved" <<  std::endl;
            out.close();
        } else 
        std::cout << "Do not save game" << std::endl;

    }

    bool Load()
    {
        std::ifstream in;
        in.open("./save.bin", std::ios::binary);
        if (in.is_open()) {            
            std::cout << "Save file reading..." << std::endl;
            in.read((char*)&properties, sizeof(Properties));
            this->hp = properties.hp;
            this->speed = properties.speed;
            this->name = properties.name;
            in.close();
            return true;
        } else 
        std::cout << "Do not open save file" << std::endl;
        return false;
        
    }
};

class Player::Impl{
public:
   float hp;
   int speed;
   std::string name;

   Impl(float hp, int speed, const std::string &name) : hp(hp), speed(speed), name(name){}

};

Player::~Player() = default;
Player::Player(): pImpl(new Impl(100, 50, "Player")){}

Player::Player(const Player& other) : pImpl(new Impl(*other.pImpl)){};

Player& Player::operator=(Player& other){
    std::swap(pImpl, other.pImpl);
    return other;
};

Player::Player(float hp, int speed, const std::string &name) 
: pImpl(new Impl(hp, speed, name)){};

void Player::changeSpeed(int change){
       pImpl->speed = change;
};

void Player::changeHp(int change){
       pImpl->hp = change;
   }

float Player::getHp() const {
       return  pImpl->hp;
   }

int Player::getSpeed() const {
       return  pImpl->speed;
   }

const std::string &Player::getName() const {
       return  pImpl->name;
   }

Memento Player::createMemento(){
        Memento m( pImpl->hp,  pImpl->speed,  pImpl->name);
        return m;
   }

void  Player::reinstateMemento(Memento& mem){
        pImpl->hp = mem.getHp();
        pImpl->speed = mem.getSpeed();
        pImpl->name = mem.getName();
    }

void  Player::print(){
    std::cout << "Name: " <<  pImpl->name << "\n"
              << "Hp: " <<  pImpl->hp << "\n"
              << "Speed: " <<  pImpl->speed << std::endl;
}




class Game{
private:
    Player main_player;
    Memento m_save;

public:
    Game(){                       
        main_player.print();            
    }

    void changeSpeed(int change){        
        m_save = main_player.createMemento();
        m_save.Save();
        main_player.changeSpeed(change);
        main_player.print();
   }

   void changeHp(int change){
        m_save = main_player.createMemento();
        m_save.Save();
        main_player.changeHp(change);
        main_player.print();
   }

   void load(){       
       if (m_save.Load()) {
            main_player.reinstateMemento(m_save);
            main_player.print();               
       }
    }
    void save(){       //явное сохранение
       m_save = main_player.createMemento();
       m_save.Save();
    }
};

int main(){
   Game game;
   game.load();
   
   game.changeHp(80); 
   game.changeHp(80);    

   game.changeHp(80);   
   game.changeHp(56);
   game.changeHp(42);
   game.save();

   game.load();

   return 0;
}

