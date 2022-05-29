#pragma once

#include <string>

class Person;

// Наблюдатель за состоянием человека.
class PersonObserver {
public:
    // Этот метод вызывается, когда меняется состояние удовлетворённости человека
    void virtual OnSatisfactionChanged(Person& /*person*/, int /*old_value*/, int /*new_value*/) {
        // Реализация метода базового класса ничего не делает
    }

protected:
    // Класс PersonObserver не предназначен для удаления напрямую
    ~PersonObserver() = default;
};

/*
    Человек.
    При изменении уровня удовлетворённости уведомляет
    связанного с ним наблюдателя
*/
class Person {
public:
    Person(const std::string& name, int age)
        : name_(name)
        , age_(age) {
    }

    int GetSatisfaction() const {
        return satisfaction_;
    }

    const std::string& GetName() const {
        return name_;
    }

    // «Привязывает» наблюдателя к человеку. Привязанный наблюдатель
    // уведомляется об изменении уровня удовлетворённости человека
    // Новый наблюдатель заменяет собой ранее привязанного
    // Если передать nullptr в качестве наблюдателя, это эквивалентно отсутствию наблюдателя
    void SetObserver(PersonObserver* observer) {
        observer_ = observer;
    }

    int GetAge() const {
        return age_;
    }

    // Увеличивает на 1 количество походов на танцы
    // Увеличивает удовлетворённость на 1
    virtual void Dance() {
        SetSatisfaction(satisfaction_ + 1);
        dance_count++;
    }

    int virtual GetDanceCount() const 
    {
        return dance_count;
    }

    // Прожить день. Реализация в базовом классе ничего не делает
    void virtual LiveADay() {
        // Подклассы могут переопределить этот метод
    }
    virtual ~Person() = default;
protected:
    void SetSatisfaction(int value) {
        if (satisfaction_ != value) {
            int old_satisfaction = satisfaction_;
            satisfaction_ = value;
            // Если у человека есть наблюдатель, сообщаем ему об изменении удовлетворённости
            if (observer_) {
                observer_->OnSatisfactionChanged(*this, old_satisfaction, satisfaction_);
            }
        }
    }
    
private:
    std::string name_;
    PersonObserver* observer_ = nullptr;
    int satisfaction_ = 100;
    int age_;
    int dance_count = 0;
};

// Рабочий.
// День рабочего проходит за работой
class Worker : public Person {
public:
    Worker(const std::string& name, int age) 
    : Person(name, age)
    {
    }

    // Рабочий старше 30 лет и младше 40 за танец получает 2 единицы удовлетворённости вместо 1
    
    void Dance() override
    {
        if (GetAge() > 30 && GetAge() < 40)
            SetSatisfaction(GetSatisfaction() + 2);
        else
            SetSatisfaction(GetSatisfaction() + 1);
        dance_count++;
    }
    
    void LiveADay() override
    {
        Work();
    }
    // День рабочего проходит за работой

    // Увеличивает счётчик сделанной работы на 1, уменьшает удовлетворённость на 5
    void Work() {
        word_make++;        
        SetSatisfaction(GetSatisfaction() - 5);
    }

    // Возвращает значение счётчика сделанной работы
    int GetWorkDone() const {
        return word_make;
    }
    
    int GetDanceCount() const override
    {
        return dance_count;
    }
    
    virtual ~Worker() = default;
    
    private:
    int word_make = 0;
    int dance_count = 0;
};

// Студент.
// День студента проходит за учёбой
class Student : public Person {
public:
    Student(const std::string& name, int age) 
    : Person(name, age)
    {
    }

    // День студента проходит за учёбой
    void LiveADay() override
    {
        Study();
    }
    // Учёба увеличивает уровень знаний на 1, уменьшает уровень удовлетворённости на 3
    void Study() {
        knowledge_level++;
        SetSatisfaction(GetSatisfaction() - 3);
    }

    // Возвращает уровень знаний
    int GetKnowledgeLevel() const {
        return knowledge_level;
    }
    
    virtual ~Student() = default;
    
    private:
    int knowledge_level = 0;
};