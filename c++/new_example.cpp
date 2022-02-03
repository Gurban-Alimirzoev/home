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
void swapTwoDisk(vector<Tower> &towers, int outTower, int inTower, int mediumTower)
{
    towers[outTower].EraseTopDisk();
    towers[mediumTower].AddToTop(towers[outTower].TopDiskInTower());
    towers[outTower].EraseTopDisk();
    towers[inTower].AddToTop(towers[outTower].TopDiskInTower());
    towers[mediumTower].EraseTopDisk();
    towers[inTower].AddToTop(towers[mediumTower].TopDiskInTower());
}

void swapOneDisk(vector<Tower> &towers, int outTower, int inTower)
{
    towers[outTower].EraseTopDisk();
    towers[inTower].AddToTop(towers[outTower].TopDiskInTower());
}

void SolveHanoi(vector<Tower> &towers)
{
    
    int disks_num = towers[0].GetDisksNum();
    if (towers[0].GetDisksNum() > 1 && towers[2].GetDisksNum() == 0)
    {
        swapTwoDisk(towers, 0, 1, 2);
    }
    if (towers[2].GetDisksNum() % 2 == 0 && towers[0].GetDisksNum() != 0)
    {
        for (int i = 0; i < towers[1].GetDisksNum() / 2; i++)
        {
            swapTwoDisk(towers, 1, 2, 0);
        }
    }
    else
    {
        swapTwoDisk(towers, 2, 1, 0);
        swapOneDisk(towers, 2, 0);
        swapTwoDisk(towers, 1, 2, 0);
        swapOneDisk(towers, 0, 1);
        swapTwoDisk(towers, 2, 1, 0);
        swapOneDisk(towers, 0, 2);
        swapTwoDisk(towers, 1, 0, 2);
        swapOneDisk(towers, 1, 2);
        swapTwoDisk(towers, 0, 2, 1);
    }
    if (towers[0].GetDisksNum() != 0)
    {
        SolveHanoi(towers);
    }
    if (disks_num == towers[2].GetDisksNum())
    {
        cout << "Все диски на 3 башне в нужном порядке" << endl;
    }
}

int main()
{
    int towers_num = 3;
    int disks_num = 3;
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