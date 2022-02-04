#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <string>

class Strategy
{
public:
    virtual ~Strategy() = default;
    virtual std::string Divide() = 0;
};

class DivideByScreenWidth: public Strategy
{
public:
    std::string Divide()
    {
        return "DivideByScreenWidth by screen width";
    }
};

class  DivideBySentence: public Strategy
{
public:
    std::string Divide()
    {
        return "Divided by Sentences";
    }
};

class DivideByUserWidth: public Strategy
{
public:
    std::string Divide()
    {
        return "DivideByScreenWidth by user width";
    }
};



class TextWork
{
protected:
    Strategy* m_strategy;
public:
    virtual void Divide() = 0;
    virtual void SetStrategy(Strategy* _strategy)
    {
        m_strategy = _strategy;
    }
    virtual ~TextWork()
    {
        delete m_strategy;
    };
};

class TextEditor: public TextWork{
private:
    size_t EditorWidth;
    std::string all_text;
public:
    TextEditor(){};
    TextEditor(const std::string &text) : all_text(text) {};

    TextEditor(std::ifstream& file) {
        std::string line;
        if (file.is_open()){
            while (getline(file, line))
            {
                all_text+=line;
            }
        }
    }

    void PrintText() const{
        std::cout << all_text << std::endl;
    }

    void Divide() 
    {
       std::cout << m_strategy->Divide() << std::endl;
    }
};

//=======================================================

enum class Position //перечисление для установки позиции
{
    begin,
    end
};

template <class T>
class IIterator {
public:
	virtual bool hasNext() = 0;
	virtual bool hasPrev() = 0;
    virtual T * getNext() = 0;
	virtual T * getPrev() = 0;
    virtual void setPosition(Position) = 0;  
	
};

template <class T>
class VectorIterator: public IIterator<T> 
{
private:
    std::vector<T>& m_vector;
    size_t m_pos;
public:
    VectorIterator(std::vector<T>& _vector) 
    : m_vector(_vector), m_pos(0) {};
    bool hasNext() override { return (m_pos < m_vector.size()); }
    bool hasPrev() override { return  (m_pos != 0); }
    T* getNext() override 
    {
        if (this->hasNext())
            {
                return &*(std::next(m_vector.begin(), m_pos++));
            }
        return nullptr;
    }
    T* getPrev() override 
    {
        if (this->hasPrev()) 
            return &*(std::next(m_vector.begin(), --m_pos));
        return nullptr;
    }

    void setPosition(Position _pos) override
    {
        switch (_pos)
        {
            case Position::begin:
                m_pos = 0;
                break;
            case Position::end:
                m_pos = m_vector.size();
                break;
        }
    }
};

template <class T>
class ArrayIterator: public IIterator<T> 
{
private:
    std::array<T , 50>& m_array;
    size_t m_pos;
public:
    ArrayIterator(std::array<T, 50>& _array) 
    : m_array(_array), m_pos(0) {};
    bool hasNext() override { return (m_pos < m_array.size()); }
    bool hasPrev() override { return  (m_pos != 0); }
    T* getNext() override 
    {
        if (this->hasNext())
            {
                return &m_array[m_pos++];
            }
        return nullptr;
    }
    T* getPrev() override 
    {
        if (this->hasPrev()) 
            return &m_array[--m_pos];
        return nullptr;
    }

    void setPosition(Position _pos) override
    {
        switch (_pos)
        {
            case Position::begin:
                m_pos = 0;
                break;
            case Position::end:
                m_pos = m_array.size();
                break;
        }
    }
};

class SomeData 
{
private:
    std::string m_string;
public:
    SomeData(size_t _num) 
    {
        m_string = "Some Data: " + std::to_string(_num);
    }
    std::string getData() const 
    {
        return m_string;
    }

    SomeData operator*()
    {
        return *this;
    }
};

std::ostream& operator<<(std::ostream& _out, const SomeData& _data)
{
    return _out << _data.getData() << std::endl;
};

//=======================================================

class IShape{
public:
   virtual void scale(double scale_percentage) = 0;// Масштабируем фигуру
   virtual void rotate(double angle) = 0;          // Поворачиваем фигуру
   virtual void flip() = 0;                        // Отражаем фигуру
};

class Figure: IShape{
private:
   int width;
   int height;
   double angle;
   bool isFlipped;
public:
   Figure(int width, int height) : width(width), height(height), angle(0.0), isFlipped(false) {}

   void scale(double scale_percentage) override {
       width *= scale_percentage;
       height *= scale_percentage;
   }

   void rotate(double angle) override {
       angle += angle;
   }

   void flip() override {
       isFlipped = !isFlipped;
   }
};

class IText{
   virtual void newSize(int size) = 0;   // Изменяем размер шрифта текста
   virtual void rotate(double angle) = 0;   // Поворачиваем текст
   virtual void reverse() = 0;              // Изменяем направление текста
};

class Text: IText{
private:
   int size;
   bool isReversed;
   double angle;
   std::string text;
public:
   Text(const std::string text, int size, double angle, bool isReversed) : text(text), size(size), angle(0.0), isReversed(false) {}

public:
    int* getSize()
    {
        return &size;
    }
   void newSize(int newSize) override {
       size = newSize;
   }

   void rotate(double newAngle) override {
       angle = newAngle;
   }

   void reverse() override {
       for (int i = 0; i < text.size()/2; ++i) {
           char c = text[i];
           text[i] = text[text.size() - 1 - i];
           text[text.size() - 1 - i] = c;
           isReversed = !isReversed;
       }
   }
};

class TextAdapter: public Figure
{
private:
    Text m_text;
public:
    TextAdapter(const Text& _text, int _size): m_text(_text), Figure(_size, _size) //нет конструктора по умолчанию
    {
        m_text.newSize(_size);
    };
    
    void scale(double scale_percentage) override {
       m_text.newSize(*(m_text.getSize()) *= scale_percentage);
   }

   void rotate(double angle) override {
       angle += angle;
   }

   void flip() override {
       m_text.reverse();
   }
};

int main() 
{
    Strategy* screenstrategy = new DivideByScreenWidth;
    Strategy* sentencestrategy = new DivideBySentence;
    Strategy* userstrategy = new DivideByUserWidth;
    TextWork* editor = new TextEditor;
    editor->SetStrategy(screenstrategy);
    editor->Divide();
    editor->SetStrategy(sentencestrategy);
    editor->Divide();
    editor->SetStrategy(userstrategy);
    editor->Divide();    

    std::cout << "//=======================================================" << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8}; //вектор int
    VectorIterator vecIt(vec);
    vecIt.setPosition(Position::end);
    while (vecIt.hasPrev()) 
    {
        std::cout << *vecIt.getPrev() << " ";
    }
    std::cout << std::endl;
    vecIt.setPosition(Position::begin);
    while (vecIt.hasNext()) 
    {
        std::cout << *vecIt.getNext() << " ";
    }
    std::cout << std::endl;
    std::array<SomeData*,50> Data; //пользовательский тип данных в массиве
    for (size_t i = 0; i < 50; i++) 
    {
        Data[i] = new SomeData(i);
    }
    
    ArrayIterator dataIt(Data);
    dataIt.setPosition(Position::begin);
    while (dataIt.hasNext()) 
    {
        std::cout << **dataIt.getNext() << " ";
    }
    std::cout << std::endl;
    while (dataIt.hasPrev()) 
    {
        delete *dataIt.getPrev();
    }


    std::cout << "\n//=======================================================" << std::endl;

    Text text("Hello world!", 13, 0.0, false);
    Figure figure(14, 15);

    Figure* textAdapter = new TextAdapter(text, 14);

    textAdapter->flip();
    figure.flip();
    textAdapter->rotate(15.4);
    figure.rotate(15.4);
    textAdapter->scale(15);
    figure.scale(15);

    delete screenstrategy;
    delete sentencestrategy;
    delete userstrategy;
    delete editor;

    delete textAdapter;


    return 0;
}