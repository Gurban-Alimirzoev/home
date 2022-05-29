#pragma once

#include "people.h"

/*
    Надзиратель за уровнем удовлетворённости.
    Способен наблюдать за состоянием человека
    Если уровень удовлетворённости человека опустится ниже минимального уровня, Надзиратель
    побуждает человека танцевать до тех пор, пока уровень уровень удовлетворённости
    не станет больше или равен максимальному значению
*/
class SatisfactionSupervisor : public PersonObserver {
public:
    // Конструктор принимает значение нижнего и верхнего уровня удовлетворённости
    SatisfactionSupervisor(int min_satisfaction_, int max_satisfaction_) 
        : min_satisfaction(min_satisfaction_), max_satisfaction(max_satisfaction_)
    {
    }
    
    int GetMax() const
    {
        return max_satisfaction;
    }
    
    int GetMin() const
    {
        return min_satisfaction;
    }
    
    void OnSatisfactionChanged(Person& person, int old_value, int new_value) override {
        if (old_value > new_value)
        {
            if (new_value < min_satisfaction)
            {
                while (new_value < max_satisfaction)
                {
                    person.Dance();
                    new_value = person.GetSatisfaction();
                }
            }
        }
    }

    private:
    int min_satisfaction, max_satisfaction;
};