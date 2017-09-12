#include <iostream>
#include <functional>
#include <gtest/gtest.h>

using namespace std;

class Thing {
public:
    int id;
    int number;
    Thing(int i = 0) {id = i;};
};

bool compareThings(const Thing& a, const Thing& b) {
    return a.id < b.id;
}

vector<Thing> randomThings(int count, int max) {
    vector<Thing> stuff;
    for(int i = 0; i < count; i++) {
        Thing h (rand()%max);
        h.number = count - i;
        stuff.push_back(h);
    }
    return stuff;
}

bool isStableSorted(const vector<Thing>& vec) {
    for (int i = 1; i < vec.size(); i++) {
        if (vec[i-1].id == vec[i].id) {
            if (vec[i-1].number > vec[i].number) {
                return false;
            }
        } else {
            if (vec[i-1].id > vec[i].id) {
                return false;
            }
        }
    }
    return true;
}

bool isSorted(const vector<Thing>& vec, function<bool(const Thing& a, const Thing& b)> comp) {
    for(int i = 1; i < vec.size(); i++) {
        if(comp(vec[i], vec[i-1])) {
            return false;
        }
    }
    return true;
}

bool isSorted(const vector<int>& vec) {
    for(int i = 1; i < vec.size(); i++) {
        if(vec[i-1] > vec[i]) {
            return false;
        }
    }
    return true;
}

void mergeSort(vector<Thing>& values, function<bool(const Thing& a, const Thing& b)> comp) {
    if(values.size() != 1) {
        vector<Thing> firstValues;
        vector<Thing> secondValues;
        for(int i = 0; i < values.size(); i++) {
            if(i < values.size()/2) {
                firstValues.push_back(values[i]);
            } else {
                secondValues.push_back(values[i]);
            }
        }
        mergeSort(firstValues, comp);
        mergeSort(secondValues, comp);
        //I start putting them back together from the back instead
        for(int i = values.size(); i > 0; i--) {
            if (!firstValues.empty()) {
                if (!secondValues.empty()) {
                    //these if statements were to check if they were empty. I suppose I could accomplish similar with a switch and case, but that would be obnoxious.
                    if (comp(firstValues.back(), secondValues.back())) {
                        //if the back of the first one is less, then put second in. Basically, pick the bigger one
                        values[i-1] = secondValues.back();
                        secondValues.pop_back();
                    } else {
                        values[i-1] = firstValues.back();
                        firstValues.pop_back();
                    }
                } else {
                    values[i-1] = firstValues.back();
                    firstValues.pop_back();
                }
            } else {
                values[i-1] = secondValues.back();
                secondValues.pop_back();
            }
        }
    }
}

TEST(SortTest, isEmptyVectorSorted) {
    vector<int> v;
    ASSERT_TRUE(isSorted(v));
}

TEST(SortTest, isSingleValueSorted) {
    vector<int> v { 1 };
    ASSERT_TRUE(isSorted(v));
}

TEST(SortTest, isVectorSorted) {
    vector<int> v {1, 3, 5, 7, 7, 9};
    ASSERT_TRUE(isSorted(v));
}

TEST(SortTest, isSortedOddVectorSorted) {
    vector<Thing> v {7, 1, 90, 2, 5, 7, 3, 6, 9};
    mergeSort(v, compareThings);
    ASSERT_TRUE(isSorted(v, compareThings));
}

TEST(SortTest, isSortedEvenVectorSorted) {
    vector<Thing> v {1, 3, 90, 2, 5, 7, 7, 6, 9, 1};
    mergeSort(v, compareThings);
    ASSERT_TRUE(isSorted(v, compareThings));
}

TEST(SortTest, isStableSortedVector) {
    vector<Thing> v = randomThings(40, 30);
    mergeSort(v, compareThings);
    ASSERT_TRUE(isStableSorted(v));
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
