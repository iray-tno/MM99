// C++11
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#ifdef LOCAL_DEBUG
class PlaySlots {
  public:
    static int quickPlay(int machineNumber, int times);
    static vector<string> notePlay(int machineNumber, int times);
};
#endif

static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
inline unsigned int randxor()
{
    unsigned int t;
    t = (x ^ (x << 11));
    x = y;
    y = z;
    z = w;
    return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

constexpr unsigned int RAND_COEFF = 100000000;

class MachinePicker {
  private:
    int n;
    vector<int> counts;
    vector<int> sumOfScores;
    int sumOfSum = 0;

  public:
    MachinePicker(int numMachines_): n(numMachines_) {
        counts = vector<int>(n, 0);
        sumOfScores = vector<int>(n, 0);
    }

    void addResult(int id, int score) {
        counts[id] += 1;
        sumOfScores[id] += score;
        sumOfSum += score;
    }

    float getAve(int id) {
        if (sumOfScores[id] == 0) {
            return 0;
        }
        return (float)sumOfScores[id] / sumOfSum / counts[id];
    }
    int greedyPick(float tau) {
        int hoge = (int) round(RAND_COEFF *tau);
        if (randxor() < hoge) {
            return randxor()%n;
        }

        float maxAve = 0;
        int maxId = 0;
        for(int i = 0; i < n; ++i) {
            float ave = getAve(i);
            if (maxAve < ave) {
                maxAve = ave;
                maxId = i;
            }
        }
        return maxId;
    }

    int pick(float tau) {
        // cerr << "TAU" << tau << endl;
        vector<float> members(n, 0);
        float sumOfMembers = 0;
        for(int i = 0; i < n; ++i) {
            members[i] = exp(getAve(i)/tau);
            sumOfMembers += members[i];
        }


        vector<int> upperList(n+1, 0);

        float sumOfUppers = 0;
        for (int i = 0; i < n; ++i) {
            upperList[i] = round(sumOfUppers);
            float prob = members[i] / sumOfMembers;
            sumOfUppers += prob * RAND_COEFF;
        }
        upperList[n] = round(sumOfUppers);

        int randResult = randxor() % (int)round(sumOfUppers);
        for (int i = 0; i < n; ++i)
        {
            if (upperList[i] <= randResult && randResult < upperList[i+1])
            {
                return i;
            }
        }
    }

    void dump() {
        for (int i = 0; i < n; ++i)
        {
            cerr << i << ":" << counts[i] << " " << sumOfScores[i] << endl;;
        }
        cerr << endl;
    }
};

class BrokenSlotMachines
{
  public:
    int playSlots(int coins, int maxTime, int noteTime, int numMachines)
    {
        if (maxTime < numMachines * 100) {
            return 0;
        }
        int currentTime = 0;
        MachinePicker mp(numMachines);
        for(int j = 0; j < 100; ++j) {

            for (int i = 0; i < numMachines; ++i)
            {
                int score = PlaySlots::quickPlay(i, 1);
                currentTime++;
                mp.addResult(i, score);
                // cerr << i << " " << score << endl;
            }
        }

        while(currentTime < maxTime) {
            int id = mp.greedyPick(exp(1.0f - (float)currentTime / maxTime));
            int score = PlaySlots::quickPlay(id, 1);
            mp.addResult(id, score);
            currentTime++;
        }

        mp.dump();

        return 0;
    }
};

#ifdef LOCAL_DEBUG
// -------8<------- end of solution submitted to the website -------8<-------
int PlaySlots::quickPlay(int machineNumber, int times){
    cout << "quick" << endl;
    cout << machineNumber << endl;
    cout << times << endl;
    int ret = 0;
    cin >> ret;
    return ret;
}

vector<string> PlaySlots::notePlay(int machineNumber, int times)
{
    cout << "note" << endl;
    cout << machineNumber << endl;
    cout << times << endl;

    vector<string> ret(times + 1);
    cin >> ret[0];
    for (int i = 0; i < times; i++)
    {
        cin >> ret[i + 1];
    }
    return ret;
    }

int main(int, char **)
{
    cin.tie(0); ios::sync_with_stdio(false);

    int coins, maxTime, noteTime, numMachines;
    cin >> coins;
    cin >> maxTime;
    cin >> noteTime;
    cin >> numMachines;

    BrokenSlotMachines ins;
    int ret = ins.playSlots(coins, maxTime, noteTime, numMachines);

    cout << ret << endl;
    return 0;
}
#endif
