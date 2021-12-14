#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

pair<bool, double> CalcMedian(vector<double> samples) {
    pair<bool, double> medi;
    double mediana;
    if (!samples.empty()) {
        sort(samples.begin(), samples.end());
        if (samples.size() % 2 == 0) {
            mediana = (samples[samples.size() / 2 - 1] + samples[samples.size() / 2]) / 2;
        }
        else {
            mediana = samples[samples.size()/2];
        }
        medi = make_pair(true, mediana);
    }
    else {
        medi = make_pair(false, 0);
    }
    return medi;
}

int main() {
    int size;
    cin >> size;

    vector<double> samples;
    for (int i = 0; i < size; ++i) {
        double sample;
        cin >> sample;
        samples.push_back(sample);
    }

    pair<bool, double> result = CalcMedian(samples);
    if (result.first) {
        cout << result.second << endl;
    } else {
        cout << "Empty vector"s << endl;
    }
}
