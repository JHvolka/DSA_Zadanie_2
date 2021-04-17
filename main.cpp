#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>
#include <sstream>
#include <array>
#include <utility>
#include <map>
#include <unordered_map>
#include <string>

#include "RB_TREE.h"
#include "HashRobin.h"
#include "AvlTree.h"
#include "HashSeparateChaining.h"

// Batch size = number of operations in single time measurement
// AC = assignment compliant
#define REPEAT_SIZE 5
#define REPEAT_SIZE_AC 5
#define BATCH_SIZE 100

// If NO_FILE_OUT is defined, no csv files will be created and no data will be
// written. Comment out this define if you want csv files to be created.
//#define NO_FILE_OUT

using namespace std;

//==============================================================================
//=========                       Timer                               ==========
//==============================================================================

// Make sure used clock is steady
using clock_type = conditional<chrono::high_resolution_clock::is_steady,
        chrono::high_resolution_clock,
        chrono::steady_clock>::type;

using time_p = chrono::time_point<clock_type>;

template<typename T = chrono::nanoseconds>
class Timer {
public:
    explicit Timer(size_t count_per_measurement = 1,
                   ostream &os = cout,
                   string file_out = "")
            : items(count_per_measurement),
              os(os),
              file(move(file_out)),
              measurement_in_progress("") {}


    /**
     * Start a new measurement group with given name and set it as the current
     * measurement.
     * @param name
     */
    void new_measurement(string name) {
        durations.insert(pair(name, vector<T>()));

        measurement_in_progress = name;
    }

    /**
     * Starts timer
     * Use for timing one event.
     */
    void start() {
        m_Start = clock_type::now();
    }

    /**
     * Stops timer and save measured duration.
     */
    void stop() {
        m_End = clock_type::now();
        if (!measurement_in_progress.empty())
            durations.find(measurement_in_progress)
                    ->second.push_back(duration());
    }

    /**
     * Prints overview of all measurement groups
     */
    void print() {

        // Loop over all measurement groups
        for (auto const&[key, val]: durations) {
            os << key << endl;

            unsigned long total = 0;

            // Get total time measured in measurement group
            for (auto t : val) {
                total += t.count();
            }

            // Calculate average time per measurement
            double avg = (double) total / (double) (durations.size());

            // Print averages
            os << "Average total: " << avg
               << (is_same<chrono::microseconds, T>::value
                   ? "µs;\n"
                   : (is_same<chrono::milliseconds, T>::value
                      ? "ms;\n"
                      : "ns;\n"))
               << "Average per item: " << avg / items
               << (is_same<chrono::microseconds, T>::value
                   ? "µs;\n\n"
                   : (is_same<chrono::milliseconds, T>::value
                      ? "ms;\n"
                      : "ns;\n"))
               << "\n";
        }
    }

    /**
     * Prints all measurements to file in CSV format
     */
    void print_each(ostream &ost) {

#ifdef NO_FILE_OUT
        return;
#endif
        ost << "Operation number";

        // First row (column labels)
        for (auto &[key, val]: durations) {
            ost << "," << key << "_T ";
        }
        ost << "\n";

        size_t pos = 0;

        // Print out measured rurations
        for (size_t i = 0; i < durations.begin()->second.size(); ++i) {
            ost << i;
            for (auto &[key, val]: durations) {
                ost << ", " << val[i].count();
            }
            ++pos;
            ost << "\n";
        }
    }

    /**
     * Overload for void print_each(ostream) with string as file
     */
    void print_each(const string &file_out) {
#ifdef NO_FILE_OUT
        return;
#endif
        ofstream ost;
        ost.open(file_out);
        print_each(ost);
        ost.close();
    }

    ~Timer() {
        // Print data to file, if file was specified
        if (!file.empty())
            print_each(file);
    }

private:

    /**
     * Calculates duration between time points m_Start and m_End
     */
    T duration() {
        return chrono::duration_cast<T>(m_End - m_Start);
    }

    ostream &os; ///< output for print
    size_t items; ///< number of measured events per one measured duration
    string file; ///< name of CSV output file for all measurements
    string measurement_in_progress; ///< name of current measurement group
    map<string, vector<T>> durations; ///< duration data
    time_p m_Start; ///< time point at the beginning of measurement
    time_p m_End;///< time point at the end of measurement
};

//==============================================================================
//=========                       Test                                ==========
//==============================================================================

/**
 * Generates vector of shuffled integers. Generated values are in range
 * (1,count>, if unique_values is set to false
 *
 * Values don't repeat if unique_values is set to true.
 */
vector<size_t> generate_random_values(int count, bool unique_values = true) {
    vector<size_t> values(count);

    // Random number generator initialization
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<> dst(1, count);

    if (unique_values) {
        // Fill vector with values and shuffle
        generate(values.begin(), values.end(),
                 [=]()mutable { return count--; });
        shuffle(values.begin(), values.end(), rng);
    }
    else {
        // Generate random values
        generate(values.begin(), values.end(),
                 [=]()mutable { return dst(rng); });
    }

    return values;
}


/**
 * Benchmarking function that measures consecutive runs of input function.
 *
 * @tparam T Data structure on which benchmarking will be performed
 * @param timer For measuring and storing run durations
 * @param tested_func Function that will be timed
 * @param insert_func Function used to fill up structure before search is tested
 * @param values List of values used when calling test_func
 * @param name Name of measurement. Will be stored in timer
 * @param count Number of iterations tested_func will be called
 * @param repeats Number of times, the whole test is to be repeated.
 *        Each time new measurement will be created
 * @param measuring_insert Used to differentiate between insert and search.
 * @param batch_size Number of tested_func calls in one timed duration. Only
 *        in non assignment compliant measurements
 * @param assignment_compliant Set to true, if AC benchmark is required.
 */
template<class T>
void
test(Timer<chrono::nanoseconds> &timer,
     function<void(T &, size_t)> &tested_func,
     function<void(T &, size_t)> &insert_func,
     const vector<size_t> &values, const string &name, size_t count,
     int repeats,
     bool measuring_insert = true, int batch_size = 1,
     bool assignment_compliant = true) {


    if (assignment_compliant) {
        for (int r = 0; r < 5; ++r) {
            T structure;
            stringstream ss;
            ss << name << " iter:" << r;
            timer.new_measurement(move(ss.str()));

            // Fill data, if search is measured
            if (!measuring_insert) {
                for (size_t i = 0; i < count; ++i) {
                    insert_func(structure, values.at(i));
                }
            }

            // Measure first batch runs
            timer.start();
            for (size_t i = 0; i < count; ++i) {
                tested_func(structure, values.at(i));
            }
            timer.stop();

            // Measure 25% more runs if insert
            size_t count_bigger;
            if (measuring_insert) {
                count_bigger = (count * 125) / 100;

                timer.start();
                for (size_t i = count; i < count_bigger; ++i) {
                    tested_func(structure, values.at(i));
                }
                timer.stop();
            }
                // Measure 5% if search
            else {
                timer.start();
                for (size_t i = 0; i < count; i += 20) {
                    tested_func(structure, values.at(i));
                }
                timer.stop();
            }
        }

    }
    else {
        for (int r = 0; r < repeats; ++r) {
            T structure;
            stringstream ss;
            ss << name << "_" << r;
            timer.new_measurement(move(ss.str()));

            // Fill data, if search is measured
            if (!measuring_insert) {
                for (size_t i = 0; i < count; ++i) {
                    insert_func(structure, values.at(i));
                }
            }

            // Measure function call duration in batches
            for (size_t i = 0; i < count;) {
                timer.start();
                for (size_t j = i + batch_size; i < j && i < count; ++i) {
                    tested_func(structure, values.at(i));
                }
                timer.stop();
            }
        }
    }
}

//==============================================================================
//=========                       Main                                ==========
//==============================================================================

int main() {


    // Create input values. All test in single run of this program will be
    // performed on the same set of input values.
    vector<size_t> values_random;
    vector<size_t> values_ordered(1500000);

    // Generate random values
    values_random = generate_random_values(1500000);
    // Generate values from 0 in ascending order
    iota(values_ordered.begin(), values_ordered.end(), 0);


    // Create lambdas as wrappers for all functions
    // My hash implementation
    function<void(HashRobin<size_t, size_t> &, size_t)> insert_HashRobin = [](
            HashRobin<size_t, size_t> &hash_table,
            size_t val) { hash_table.Insert(val, val+10); };
    function<void(HashRobin<size_t, size_t> &, size_t)> search_HashRobin = [](
            HashRobin<size_t, size_t> &hash_table,
            size_t val) {
        volatile size_t abc;
        abc = hash_table.Find(val);
    };

    // My red-black tree implementation
    function<void(RB_TREE &, size_t)> insert_RBTree = [](
            RB_TREE &tree, size_t val) {
        tree.Insert(val, reinterpret_cast<void *>(val+10));
    };
    function<void(RB_TREE &, size_t)> search_RBTree = [](
            RB_TREE &tree, size_t val) {
        volatile void *abc;
        abc = tree.Find(val);
    };

    // Hash implementation taken from Github
    function<void(HashSC::HashTable &, size_t)> insert_HashSC = [](
            HashSC::HashTable &table, size_t val) {
        table.Insert(HashSC::Node(val, "a"));
    };
    function<void(HashSC::HashTable &, size_t)> search_HashSC = [](
            HashSC::HashTable &table, size_t val) {
        table.Search(val);
    };

    // AVL tree implementation taken from Github
    function<void(AVLTree<size_t, size_t> &, size_t)> insert_AVLTree = [](
            AVLTree<size_t, size_t> &tree, size_t val) {
        tree.Insert(val+10);
    };
    function<void(AVLTree<size_t, size_t> &, size_t)> search_AVLTree = [](
            AVLTree<size_t, size_t> &tree, size_t val) {
        tree.find(val);
    };

    // Run assignment required tests for all required operation counts
    size_t test_counts[3] = {1000, 25000, 100000};
    for (auto test_count : test_counts) {

        stringstream ss;

        // Initialize timers for all tests
        ss << "times_HashRobin_assignment_compliant_" << test_count
           << ".csv";
        Timer timer_HashRobin_assignment_compliant
                (REPEAT_SIZE_AC, cout, ss.str());
        ss.str("");

        ss << "times_RBTree_assignment_compliant_" << test_count << ".csv";
        Timer timer_RBTree_assignment_compliant(REPEAT_SIZE_AC, cout, ss.str());
        ss.str("");

        ss << "times_HashSC_assignment_compliant_" << test_count
           << ".csv";
        Timer timer_HashSC_assignment_compliant
                (REPEAT_SIZE_AC, cout, ss.str());
        ss.str("");

        ss << "times_AVLTree_assignment_compliant_" << test_count
           << ".csv";
        Timer timer_AVLTree_assignment_compliant
                (REPEAT_SIZE_AC, cout, ss.str());
        ss.str("");

        // Format: XY_AC
        // X: I = input | S = search
        // Y: R = random | O = ordered
        // AC = assignment compliant
        array<string, 8> descriptions = {"IR_AC", "IO_AC", "SR_AC", "SO_AC"};
        int desc_pos = 0;

        // Tests for my implementation of hash table
        // Inserts
        test<HashRobin<size_t, size_t>>
                (timer_HashRobin_assignment_compliant, insert_HashRobin,
                 insert_HashRobin,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, true);

        test<HashRobin<size_t, size_t>>
                (timer_HashRobin_assignment_compliant, insert_HashRobin,
                 insert_HashRobin,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, true);
        //Searches
        test<HashRobin<size_t, size_t>>
                (timer_HashRobin_assignment_compliant, search_HashRobin,
                 insert_HashRobin,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, false);

        test<HashRobin<size_t, size_t>>
                (timer_HashRobin_assignment_compliant, search_HashRobin,
                 insert_HashRobin,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, false);


        // Tests for my implementation of binary search tree
        // Inserts
        desc_pos = 0;
        test<RB_TREE>
                (timer_RBTree_assignment_compliant, insert_RBTree,
                 insert_RBTree,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, true);

        test<RB_TREE>
                (timer_RBTree_assignment_compliant, insert_RBTree,
                 insert_RBTree,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, true);
        //Searches
        test<RB_TREE>
                (timer_RBTree_assignment_compliant, search_RBTree,
                 insert_RBTree,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, false);

        test<RB_TREE>
                (timer_RBTree_assignment_compliant, search_RBTree,
                 insert_RBTree,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, false);


        // Tests for implementation of binary search tree taken from github
        // Inserts
        desc_pos = 0;
        test<AVLTree<size_t, size_t>>
                (timer_AVLTree_assignment_compliant, insert_AVLTree,
                 insert_AVLTree,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, true);

        test<AVLTree<size_t, size_t>>
                (timer_AVLTree_assignment_compliant, insert_AVLTree,
                 insert_AVLTree,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, true);
        //Searches
        test<AVLTree<size_t, size_t>>
                (timer_AVLTree_assignment_compliant, search_AVLTree,
                 insert_AVLTree,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, false);

        test<AVLTree<size_t, size_t>>
                (timer_AVLTree_assignment_compliant, search_AVLTree,
                 insert_AVLTree,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, false);


        // Tests for implementation of hash table taken from github
        // Inserts
        desc_pos = 0;
        test<HashSC::HashTable>
                (timer_HashSC_assignment_compliant, insert_HashSC,
                 insert_HashSC,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, true);

        test<HashSC::HashTable>
                (timer_HashSC_assignment_compliant, insert_HashSC,
                 insert_HashSC,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, true);
        //Searches
        test<HashSC::HashTable>
                (timer_HashSC_assignment_compliant, search_HashSC,
                 insert_HashSC,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, false);

        test<HashSC::HashTable>
                (timer_HashSC_assignment_compliant, search_HashSC,
                 insert_HashSC,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE_AC, false);

        cout << "Hash moja implementacia\n";
        timer_HashRobin_assignment_compliant.print();
        cout << "--------------------\n";
        cout << "BST moja implementacia\n";
        timer_RBTree_assignment_compliant.print();
        cout << "--------------------\n";
        cout << "BST prevzata implementacia\n";
        timer_AVLTree_assignment_compliant.print();
        cout << "--------------------\n";
        cout << "Hash prevzata implementacia\n";
        timer_HashSC_assignment_compliant.print();
    }

    // Run additional tests for all required operation counts
    size_t test_counts_additional[1] = {1000000};
    for (auto test_count : test_counts_additional) {

        stringstream ss;

        // Initialize timers for all tests
        ss << "times_HashRobin_" << test_count << ".csv";
        Timer timer_HashRobin(REPEAT_SIZE, cout, ss.str());
        ss.str("");


        ss << "times_RBTree_" << test_count << ".csv";
        Timer timer_RBTree(REPEAT_SIZE, cout, ss.str());
        ss.str("");


        ss << "times_HashSC_" << test_count << ".csv";
        Timer timer_HashSC(REPEAT_SIZE, cout, ss.str());
        ss.str("");


        ss << "times_AVLTree_" << test_count << ".csv";
        Timer timer_AVLTree(REPEAT_SIZE, cout, ss.str());
        ss.str("");


        array<string, 4> descriptions = {"IR", "IO", "SR", "SO",};
        int desc_pos = 0;

        // Tests for my implementation of hash table
        // Inserts
        test<HashRobin<size_t, size_t>>
                (timer_HashRobin, insert_HashRobin, insert_HashRobin,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, true,
                 BATCH_SIZE, false);

        test<HashRobin<size_t, size_t>>
                (timer_HashRobin, insert_HashRobin, insert_HashRobin,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, true,
                 BATCH_SIZE, false);
        //Searches
        test<HashRobin<size_t, size_t>>
                (timer_HashRobin, search_HashRobin, insert_HashRobin,
                 values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, false,
                 BATCH_SIZE, false);

        test<HashRobin<size_t, size_t>>
                (timer_HashRobin, search_HashRobin, insert_HashRobin,
                 values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, false,
                 BATCH_SIZE, false);


        // Tests for my implementation of binary search tree
        // Inserts
        desc_pos = 0;
        test<RB_TREE>
                (timer_RBTree, insert_RBTree, insert_RBTree, values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, true,
                 BATCH_SIZE, false);

        test<RB_TREE>
                (timer_RBTree, insert_RBTree, insert_RBTree, values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, true,
                 BATCH_SIZE, false);
        //Searches
        test<RB_TREE>
                (timer_RBTree, search_RBTree, insert_RBTree, values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, false,
                 BATCH_SIZE, false);

        test<RB_TREE>
                (timer_RBTree, search_RBTree, insert_RBTree, values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, false,
                 BATCH_SIZE, false);


        // Tests for implementation of binary search tree taken from github
        // Inserts
        desc_pos = 0;
        test<AVLTree<size_t, size_t>>
                (timer_AVLTree, insert_AVLTree, insert_AVLTree, values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, true,
                 BATCH_SIZE, false);

        test<AVLTree<size_t, size_t>>
                (timer_AVLTree, insert_AVLTree, insert_AVLTree, values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, true,
                 BATCH_SIZE, false);
        //Searches
        test<AVLTree<size_t, size_t>>
                (timer_AVLTree, search_AVLTree, insert_AVLTree, values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, false,
                 BATCH_SIZE, false);

        test<AVLTree<size_t, size_t>>
                (timer_AVLTree, search_AVLTree, insert_AVLTree, values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, false,
                 BATCH_SIZE, false);


        // Tests for implementation of hash table taken from github
        // Inserts
        desc_pos = 0;
        test<HashSC::HashTable>
                (timer_HashSC, insert_HashSC, insert_HashSC, values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, true,
                 BATCH_SIZE, false);

        test<HashSC::HashTable>
                (timer_HashSC, insert_HashSC, insert_HashSC, values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, true,
                 BATCH_SIZE, false);

        //Searches
        test<HashSC::HashTable>
                (timer_HashSC, search_HashSC, insert_HashSC, values_random,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, false,
                 BATCH_SIZE, false);

        test<HashSC::HashTable>
                (timer_HashSC, search_HashSC, insert_HashSC, values_ordered,
                 descriptions[desc_pos++], test_count, REPEAT_SIZE, false,
                 BATCH_SIZE, false);

//        timer_HashRobin.print();
//        cout << "--------------------\n";
//        timer_RBTree.print();
//        cout << "--------------------\n";
//        timer_AVLTree.print();
//        cout << "--------------------\n";
//        timer_HashSC.print();
    }

    return 0;
}
