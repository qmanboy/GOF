#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

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

template <class T>
class IIterator
{
public:
    virtual const T* begin() = 0;
    virtual const T* end() = 0;

};

template <class T>
class VectorIterator: public IIterator<T> 
{
private:
    std::vector<T>& m_vector;
    T* m_ptr;
public:

    VectorIterator(std::vector<T>& _vector) 
    : m_vector(_vector), m_ptr(nullptr) {};

    const T* begin() 
    {
        if (m_vector.size() > 0)
            ptr = &m_vector[0];
        return ptr;
    }

    const T* end() 
    {
        if (m_vector.size() > 0)
            ptr = this->begin() + m_vector.size();
        return ptr;
    }

    const T* rbegin() 
    {
        return this->end() - 1;
    }

    const T* rend() 
    {
        return this->begin() - 1;
    }

    VectorIterator& operator++() 
    {
        return ptr--;
    }

};

template <class T>
class ArrayIterator: public IIterator<T> 
{
private:
    std::array<T, 5>& m_array;
public:

    ArrayIterator(std::array<T, 5>&  _array) 
    :  m_array(_array) {};

    const T* begin() 
    {
        return &m_array[0];
    }

    const T* end() 
    {
        return (this->begin() + m_array.size());
    }
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

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8};
    VectorIterator vecIt(vec);
    std::for_each(vecIt.begin(), vecIt.end(),[](auto _el){std::cout << _el << " ";});
    std::cout << std::endl;
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    ArrayIterator arrIt(arr);
    std::for_each(arrIt.begin(), arrIt.end(),[](auto _el){std::cout << _el << " ";});

    std::cout << "\n//=======================================================" << std::endl;

    Text text("Hello, world!", 13, 0.0, false);
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