#include <iostream>
#include <chrono>
#include <random>
#include <array>
#include <utility>
#include <vector>
#include <algorithm>
#include <climits>
#include <fstream>

#include "RB_TREE.h"
#include "HashRobin.h"

#define REPEAT_COUNT 10

using namespace std;

template<typename T = chrono::microseconds>
class Timer {
public:
    explicit Timer(string msg = "", size_t count = 1, ostream &os = cout)
            : message(std::move(msg)),
              items(count), os(os) {}

    virtual ~Timer() {
        print();
    }

    void start() {
        m_Start = chrono::high_resolution_clock::now();
    }

    void stop() {
        m_End = chrono::high_resolution_clock::now();
        durations.push_back(duration());
    }

    void print() {
        long total = 0;
        os << message;
        for (auto d : durations) {
#ifdef PRINT_TIMES
            cout << d.count() << "µs; ";
#endif
            total += d.count();
        }

        double avg = (double) total / (double) (durations.size());

        os << "Average total: " << avg
           << (is_same<chrono::microseconds, T>::value ? "µs;\n"
                                                       : (is_same<chrono::milliseconds, T>::value
                                                          ? "ms;\n" : "ns;\n"))
           << "Average per item: " << avg / items
           << (is_same<chrono::microseconds, T>::value ? "µs;\n\n"
                                                       : (is_same<chrono::milliseconds, T>::value
                                                          ? "ms;\n" : "ns;\n"));
    }

    void print_each(ostream &ost) {
        size_t pos = 0;
        ost << "Operation order, time of execution\n";
        for (auto d : durations) {
            ost << pos++ << ", " << d.count() << "\n";
        }
    }
    
    static void print_each (ostream &ost, vector<vector<T>> duration_vectors){
        size_t pos = 0;
        for
    }

    vector<T> * get_durations (){return &durations;}

private:

    T duration() {
        return chrono::duration_cast<T>(m_End - m_Start);
    }

    ostream &os;
    size_t items;
    string message;
    vector<T> durations;
    chrono::time_point<chrono::high_resolution_clock> m_Start;
    chrono::time_point<chrono::high_resolution_clock> m_End;
};

/**
 * Generates vector of shuffled integers. Generated values are in range
 * (1,count>, if unique_values is set to false
 *
 * Values don't repeat if unique_values is set to true.
 */
vector<int> *random_values(int count, bool unique_values = true) {
    auto values = new vector<int>(count);

    // Random number generator initialization
    std::random_device rd;
    std::mt19937 rng(rd());
    uniform_int_distribution<> dst(1, count);

    if (unique_values) {
        // Fill vector with values and shuffle
        generate(values->begin(), values->end(),
                 [=]()mutable { return count--; });
        shuffle(values->begin(), values->end(), rng);
    }
    else {
        // Generate random values
        generate(values->begin(), values->end(),
                 [=]()mutable { return dst(rng); });
    }

    return values;
}

template<typename S>
void random_insert(size_t count, int repeats, bool unique_values = true) {
    vector<int> *values;

    // Test label
    stringstream ss_insert;
    ss_insert << "Random Insert of " << count
              << (unique_values ? " different values" : "values") << " with "
              << repeats
              << " sampling runs into " << typeid(S).name() << "\n";

    // Initialize timers
    auto *timer_insert = new Timer<chrono::microseconds>(ss_insert.str(),
                                                         count);
    auto *timer_find = new Timer<chrono::nanoseconds>(
            "Search of each inserted value:\n", count);

    // Run test _repeats_ times
    for (int i = 0; i < repeats; ++i) {
        // Structure has lifetime of one iteration
        S structure;

        // Get random values
        values = random_values(count, unique_values);

        // Time insertions
        timer_insert->start();
        for (auto v : *values) {
            structure.Insert(v, nullptr);
        }
        timer_insert->stop();

        for (auto v : *values) {
            timer_find->start();
            structure.Find(v);
            timer_find->stop();
        }
        delete values;
    }
    delete timer_insert;
    delete timer_find;
}

template<typename S>
void ordered_insert(size_t count, int repeats) {
    // Test label
    stringstream ss_insert;
    ss_insert << "Ordered Insert of " << count
              << " valuds with " << repeats
              << " sampling runs into " << typeid(S).name() << "\n";

    // Initialize timers
    auto *timer_insert = new Timer<chrono::microseconds>(ss_insert.str(),
                                                         count);
    auto *timer_find = new Timer<chrono::nanoseconds>(
            "Search of each inserted value:\n", count);

    // Run test _repeats_ times
    for (int i = 0; i < repeats; ++i) {
        // Structure has lifetime of one iteration
        S structure;

        // Time insertions
        timer_insert->start();
        for (int j = 0; i < count; ++i) {
            structure.Insert(i, nullptr);
        }
        timer_insert->stop();

        timer_find->start();
        for (int j = 0; i < count; ++i) {
            structure.Find(j);
        }
        timer_find->stop();
    }
    delete timer_insert;
    delete timer_find;
}

int main() {
    /*ordered_insert<RB_TREE>(100000, 1);
    *//*for (size_t count = 1; count <= 1048576; count *= 16) {
        random_insert<RB_TREE>(count, REPEAT_COUNT);
        ordered_insert<RB_TREE>(count, REPEAT_COUNT);
        cout << "-----------------------------------------------------" << endl;
    }*/



    /*for (int j = 0; j < 3; ++j) {
        HashRobin<int, int> table;

        size_t val = 1000000;
        {
            Timer<chrono::nanoseconds> timer1("1st ");
            for (size_t i = 0; i < val; ++i) {
                timer1.start();
                table.Insert(i, i + 10);
                timer1.stop();
            }
        }
        {
            Timer<chrono::nanoseconds> timer2("2nd ");
            for (size_t i = val; i < 2 * val; ++i) {
                timer2.start();
                table.Insert(i, i + 10);
                timer2.stop();
            }
        }
        {
            Timer<chrono::nanoseconds> timer3("3rd ");
            for (size_t i = 2 * val; i < 3 * val; ++i) {
                timer3.start();
                table.Insert(i, i + 10);
                timer3.stop();
            }
        }
    }*/

    HashRobin<int, int> table;
    RB_TREE tree;
    vector<chrono::nanoseconds> * durations_hash, *durations_tree;

    size_t val = 1000000;
    {
        Timer<chrono::nanoseconds> timer("hash ");
        for (size_t i = 0; i < val; ++i) {
            timer.start();
            table.Insert(i, i + 10);
            timer.stop();
        }
        /*cout << "Find: " << table.Find(600000) << endl;
        ofstream os;
        os.open("out_hash.csv");
        timer.print_each(os);
        os.close();*/
        durations_hash = timer.get_durations();
    }
    {
        Timer<chrono::nanoseconds> timer("tree ");
        for (size_t i = 0; i < val; ++i) {
            timer.start();
            tree.Insert(i, nullptr);
            timer.stop();
        }
        /*cout << "Find: " << table.Find(600000) << endl;
        ofstream os;
        os.open("out_tree.csv");
        timer.print_each(os);
        os.close();*/
        durations_tree = timer.get_durations();
    }

    //table.DebugPrint();

    return 0;
}
