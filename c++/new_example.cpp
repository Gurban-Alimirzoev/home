#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class Tower
{
public:
    // конструктор и метод SetDisks нужны, чтобы правильно создать башни
    Tower(int disks_num)
    {
        FillTower(disks_num);
    }

    int GetDisksNum() const
    {
        return disks_.size();
    }

    void SetDisks(int disks_num)
    {
        FillTower(disks_num);
    }

    // добавляем диск на верх собственной башни
    // обратите внимание на исключение, которое выбрасывается этим методом
    void AddToTop(int disk)
    {
        int top_disk_num = disks_.size() - 1;
        if (0 != disks_.size() && disk >= disks_[top_disk_num])
        {
            throw invalid_argument("Невозможно поместить большой диск на маленький");
        }
        else
        {
            disks_.push_back(disk);
        }
    }

    void EraseTopDisk()
    {
        if (disks_.empty())
        {
            throw invalid_argument("Невозможно снять диск с пустой башни");
        }
        else
        {
            disks_.pop_back();
        }
    }
    int TopDiskInTower()
    {
        return *end(disks_);
    }

private:
    vector<int> disks_;

    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int disks_num)
    {
        for (int i = disks_num; i > 0; i--)
        {
            disks_.push_back(i);
        }
    }
};

void SolveHanoi(vector<Tower> &towers)
{

    if (towers[0].GetDisksNum() != 0)
    {
        towers[0].EraseTopDisk();
        towers[2].AddToTop(towers[0].TopDiskInTower());
        towers[0].EraseTopDisk();
        towers[1].AddToTop(towers[0].TopDiskInTower());
        towers[2].EraseTopDisk();
        towers[1].AddToTop(towers[2].TopDiskInTower());
        towers[0].EraseTopDisk();
        towers[2].AddToTop(towers[0].TopDiskInTower());
        towers[1].EraseTopDisk();
        towers[0].AddToTop(towers[1].TopDiskInTower());
        towers[1].EraseTopDisk();
        towers[2].AddToTop(towers[1].TopDiskInTower());
        towers[0].EraseTopDisk();
        towers[2].AddToTop(towers[0].TopDiskInTower());
        SolveHanoi(towers);
    }
}

int main()
{
    int towers_num = 3;
    int disks_num = 8;
    vector<Tower> towers;
    // добавим в вектор три пустые башни
    for (int i = 0; i < towers_num; ++i)
    {
        towers.push_back(0);
    }
    // добавим на первую башню три кольца
    towers[0].SetDisks(disks_num);

    SolveHanoi(towers);
}