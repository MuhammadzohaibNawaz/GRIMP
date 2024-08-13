#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

using namespace std;
namespace fs = std::filesystem;
using CodeTable = std::vector<std::pair<std::vector<int>, int>>;
using Pattern = std::vector<int>;
using UsageCount = int;
using CodeTable = std::vector<std::pair<Pattern, UsageCount>>;

std::vector<int> generateRandomItemset(int, const std::unordered_map<int, int>&);
std::vector<std::vector<int>> MPCrossoverOperator(const std::vector<int>&, const std::vector<int>&);
std::vector<int> standardMutationOperator(const std::vector<int>&, const std::unordered_set<int>&);
std::vector<std::vector<int>> deleteItemset(const std::vector<std::vector<int>>&, const std::vector<int>&, const std::vector<int>&);
std::vector<int> generateCrossoverPoints2(int maxSize);
std::vector<std::vector<int>> performMPCrossover(const std::vector<int>& P1Sol, const std::vector<int>& P2Sol, const std::vector<int>& crossoverPoints);
int getWeightedRandomItem(const std::unordered_map<int, int>& itemFrequency);
int calculateSizeInBits(const std::vector<std::vector<int>>& sequence);
int findBestClass(const std::vector<int>& transaction, const std::vector<std::vector<std::pair<std::vector<int>, int>>>& classCodeTables);
void sortCodeTables(std::vector<CodeTable>& classCodeTables);
std::vector<int> removeDuplicates(const std::vector<int>& original);
//int countIntersection(const std::vector<int>& transaction, const std::vector<int>& pattern);
int calculateScore(const std::vector<int>& transaction, const std::unordered_map<int, int>& itemFrequencies);

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};


int Pat1Count = 0;
int Pat2Count = 0;
const int INT_BITS = 32;

// Function to apply GRIMP on given test data
std::vector<std::pair<std::vector<int>, int>> applyGRIMP(const std::vector<std::vector<int>>& testData) {
    int AcS = 0; // Acceptable size threshold for patterns
    int patterns = 100; // Number of patterns to generate
    int numberOfIterations = 5; // Number of iterations per pattern generation

    std::vector<std::pair<std::vector<int>, int>> finalResult; // Modified to hold patterns and their occurrences
    std::vector<std::vector<int>> subSequence(testData); // Copy sequence to subSequence
    std::vector<std::vector<int>> remainingSequence;
    /*for (const auto& row : testData) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }*/
    for (int ol = 1; finalResult.size() <= patterns; ol++) {
        std::unordered_map<int, int> itemFrequency;
        for (const std::vector<int>& transaction : subSequence) {
            for (int item : transaction) {
                itemFrequency[item]++;
            }
        }

        std::unordered_set<int> population;
        for (const std::vector<int>& set : subSequence) {
            population.insert(set.begin(), set.end());
        }

       

        int longestItemSet = 0;
        for (const auto& row : subSequence) {
            if (!row.empty() && row.size() > longestItemSet) {
                longestItemSet = row.size();
            }
        }
        if (longestItemSet <= 2) break;

        Pat1Count = 0;
        Pat2Count = 0;
        std::vector<int> FPat1;
        std::vector<int> FPat2;
        int GP1C = 0;
        int GP2C = 0;
        //cout << "here" << longestItemSet << endl;
        std::vector<int> Pat1 = generateRandomItemset(longestItemSet, itemFrequency);
        std::vector<int> Pat2 = generateRandomItemset(longestItemSet, itemFrequency);
        //cout << "here 2";
        for (int pat = 1; pat <= numberOfIterations; pat++) {
            /*std::cout << "Pat1: ";
            for (int item : Pat1) {
                std::cout << item << " ";
            }
            cout << endl;

            std::cout << "Pat2: ";
            for (int item : Pat2) {
                std::cout << item << " ";
            }
            cout << endl;*/
            
            std::vector<std::vector<int>> crossoverResult = MPCrossoverOperator(Pat1, Pat2);

            Pat1 = crossoverResult[0];
            Pat2 = crossoverResult[1];
            //cout << "Here";
            Pat1 = standardMutationOperator(Pat1, population);
            //cout << "Here 2";
            Pat2 = standardMutationOperator(Pat2, population);
            //cout << "Here 3";
            /*     std::cout << "Pat1: ";
            for (int item : Pat1) {
                std::cout << item << " ";
            }
            cout << endl;

            std::cout << "Pat2: ";
            for (int item : Pat2) {
                std::cout << item << " ";
            }
            cout << endl;*/

            //int a;
            //cin >> a;
            Pat1 = removeDuplicates(Pat1);
            Pat1 = removeDuplicates(Pat1);
            std::vector<std::vector<int>> remainingSequence = deleteItemset(subSequence, Pat1, Pat2);
            //cout << "Update Size is : " << calculateSizeInBits(remainingSequence) << endl;
            subSequence = remainingSequence;
            if (Pat1Count > GP1C) {
                GP1C = Pat1Count;
                FPat1 = Pat1;
            }

            if (Pat2Count > GP2C) {
                GP2C = Pat2Count;
                FPat2 = Pat2;
            }
        
            
        }

        /*for (const auto& row : subSequence) {
            for (const auto& element : row) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }
        int b;
        cout << "Enter : ";
        cin >> b;
        */

        //cout << "Size is : "<<calculateSizeInBits(subSequence) << endl;
        if (FPat1.size() > AcS) {
            
            finalResult.push_back(std::make_pair(FPat1, GP1C));
        }

        if (FPat2.size() > AcS) {
            
            finalResult.push_back(std::make_pair(FPat2, GP2C));
        }
    }

    return finalResult;
}
void crossValidation(const std::vector<std::vector<std::vector<int>>>& allData, int folds) {
    std::vector<int> accuracyResults;

    for (int i = 0; i < folds; ++i) {
        std::vector<std::vector<std::pair<std::vector<int>, int>>> classCodeTables(allData.size());
        std::vector<std::vector<std::vector<int>>> trainSets(allData.size());
        std::vector<std::vector<std::vector<int>>> testSets(allData.size());

        // Prepare data for each class
        for (int c = 0; c < allData.size(); ++c) {
            const auto& data = allData[c];
            int foldSize = data.size() / folds;
            int start = i * foldSize;
            int end = (i == folds - 1) ? data.size() : start + foldSize;

            for (int j = 0; j < data.size(); ++j) {
                if (j >= start && j < end)
                    testSets[c].push_back(data[j]);
                else
                    trainSets[c].push_back(data[j]);
            }

            // Apply GRIMP to generate code table for the current training set of this class
            classCodeTables[c] = applyGRIMP(trainSets[c]);
        }
        sortCodeTables(classCodeTables);
        /*for (size_t classIndex = 0; classIndex < classCodeTables.size(); ++classIndex) {
            std::cout << "Class " << classIndex << " Code Table:" << std::endl;
            for (const auto& entry : classCodeTables[classIndex]) {
                std::cout << "  Pattern: ";
                for (const auto& item : entry.first) {
                    std::cout << item << " ";
                }
                std::cout << "| Usage: " << entry.second << std::endl;
            }
            std::cout << std::endl;
        }*/

        //int a;
        ////cout << "herrre";
        //cin >> a;
        int correctClassifications = 0;
        int totalTransactions = 0;

        // Test classification accuracy using generated code tables
        for (int c = 0; c < allData.size(); ++c) {
            for (const auto& transaction : testSets[c]) {
                int predictedClass = findBestClass(transaction, classCodeTables);
                if (predictedClass == c) {
                    correctClassifications++;
                }
                totalTransactions++;
            }
        }

        // Calculate accuracy for this fold and store it
        int foldAccuracy = static_cast<int>(100.0 * correctClassifications / totalTransactions);
        accuracyResults.push_back(foldAccuracy);
        std::cout << "Fold " << i + 1 << ": Accuracy = " << foldAccuracy << "%\n";
    }

    // Calculate and output the average accuracy
    int totalAccuracy = std::accumulate(accuracyResults.begin(), accuracyResults.end(), 0);
    int averageAccuracy = totalAccuracy / folds;
    std::cout << "Average Accuracy: " << averageAccuracy << "%\n";
}

//this is the simple one time check one
//int findBestClass(const std::vector<int>& transaction, const std::vector<CodeTable>& classCodeTables) {
//    int bestClass = -1;
//    int maxOccurrences = 0; // Initialize to 0 since we're adding up positive numbers
//
//    // Debug output
//    /*std::cout << "Transaction: ";
//    for (const auto& item : transaction) {
//        std::cout << item << " ";
//    }
//    std::cout << std::endl;*/
//
//    // Iterate over each class's code table
//    for (size_t classIndex = 0; classIndex < classCodeTables.size(); ++classIndex) {
//        int occurrences = 0;
//
//        // Iterate over each pattern in the class's code table
//        for (const auto& [pattern, count] : classCodeTables[classIndex]) {
//            // Check each item in the transaction if it exists in the current pattern
//            for (const auto& item : transaction) {
//                if (std::find(pattern.begin(), pattern.end(), item) != pattern.end()) {
//                    occurrences += count; // Add the pattern's usage count if the item is found
//                }
//            }
//        }
//
//        // Update the best class if the current class has more occurrences
//        if (occurrences > maxOccurrences) {
//            maxOccurrences = occurrences;
//            //maxOccurrences++;
//            bestClass = static_cast<int>(classIndex);
//        }
//    }
//
//    return bestClass;
//}


int findBestClass(const std::vector<int>& transaction, const std::vector<std::vector<std::pair<std::vector<int>, int>>>& classCodeTables) {
    // Vector to store the item frequencies for each class code table
    std::vector<std::unordered_map<int, int>> classItemFrequencies(classCodeTables.size());

    // Populate the classItemFrequencies with counts from each code table
    for (size_t classIndex = 0; classIndex < classCodeTables.size(); ++classIndex) {
        for (const auto& [pattern, count] : classCodeTables[classIndex]) {
            for (int item : pattern) {
                classItemFrequencies[classIndex][item] += count;
            }
        }
    }

    int bestClass = -1;
    int maxScore = -1;

    // Evaluate the transaction against each class's item frequencies
    for (size_t classIndex = 0; classIndex < classItemFrequencies.size(); ++classIndex) {
        int score = 0;
        // Sum the frequencies of the transaction items found in the class's item frequencies
        for (int item : transaction) {
            if (classItemFrequencies[classIndex].find(item) != classItemFrequencies[classIndex].end()) {
                score += classItemFrequencies[classIndex][item];
            }
        }

        // Update the best class if this class's score is higher than the current max
        if (score > maxScore) {
            maxScore = score;
            bestClass = classIndex;
        }
    }

    return bestClass; // Return the index of the best class
}


//int countIntersection(const std::vector<int>& transaction, const std::vector<int>& pattern) {
//    std::vector<int> transSorted = transaction;
//    std::vector<int> patternSorted = pattern;
//
//    std::sort(transSorted.begin(), transSorted.end());
//    std::sort(patternSorted.begin(), patternSorted.end());
//
//    std::vector<int> intersection;
//
//    std::set_intersection(transSorted.begin(), transSorted.end(), patternSorted.begin(), patternSorted.end(), std::back_inserter(intersection));
//
//    return intersection.size();
//}
//
//int findBestClass(const std::vector<int>& transaction, const std::vector<std::vector<std::pair<std::vector<int>, int>>>& classCodeTables) {
//    int bestClass = -1;
//    int maxScore = -1;
//
//    for (size_t classIndex = 0; classIndex < classCodeTables.size(); ++classIndex) {
//        int score = 0;
//        for (const auto& [pattern, occurrences] : classCodeTables[classIndex]) {
//            int count = countIntersection(transaction, pattern);
//            score += count * occurrences;
//        }
//
//        if (score > maxScore) {
//            maxScore = score;
//            bestClass = static_cast<int>(classIndex);
//        }
//    }
//
//    return bestClass;
//}




std::vector<std::vector<int>> readData(const std::string& filePath) {
    std::vector<std::vector<int>> data;
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> transaction;
        int num;
        while (iss >> num) {
            transaction.push_back(num);
        }
        data.push_back(transaction);
    }
    return data;
}

std::vector<std::vector<std::vector<int>>> loadDataFromDirectory(const std::string& directoryPath) {
    std::vector<std::vector<std::vector<int>>> allData;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        auto filePath = entry.path().string();
        std::cout << "Reading data from: " << filePath << std::endl;
        auto data = readData(filePath);
        allData.push_back(data);
    }
    return allData;
}


void sortCodeTables(std::vector<CodeTable>& classCodeTables) {
    for (auto& codeTable : classCodeTables) {
        std::sort(codeTable.begin(), codeTable.end(),
            [](const std::pair<std::vector<int>, int>& a, const std::pair<std::vector<int>, int>& b) {
                // Sort by usage count descending
                if (a.second != b.second) return a.second > b.second;
                // If usage count is the same, sort by pattern length ascending
                return a.first.size() < b.first.size();
            }
        );
    }
}





std::vector<int> generateRandomItemset(int longestItemSet, const std::unordered_map<int, int>& itemFrequency) {
    std::vector<int> randomSet;
    std::random_device rd;
    std::mt19937 gen(rd());
    //cout << longestItemSet - 1 << ", ";
    std::uniform_int_distribution<> dis(2, longestItemSet - 1); // Random size between 2 and longestItemSet - 1
    int randomItemsetSize = dis(gen);

    while (randomSet.size() < randomItemsetSize) {
        int randomNumber = getWeightedRandomItem(itemFrequency);
        if (std::find(randomSet.begin(), randomSet.end(), randomNumber) == randomSet.end()) { // Ensure no repetition
            randomSet.push_back(randomNumber);
        }
    }

    return randomSet;
}

std::vector<std::vector<int>> MPCrossoverOperator(const std::vector<int>& P1Sol, const std::vector<int>& P2Sol) {
    int smallestSize = std::min(P1Sol.size(), P2Sol.size());
    if (smallestSize <= 3) {
        // Not enough size to perform crossover with the given rules
        return { P1Sol, P2Sol }; // return parents as is, no crossover possible
    }
    std::vector<int> crossoverPoints = generateCrossoverPoints2(smallestSize);
    /*for (int item : crossoverPoints) {
        std::cout << item << " ";
    }*/
//    cout << "here one";
    return performMPCrossover(P1Sol, P2Sol, crossoverPoints);
}

std::vector<int> generateCrossoverPoints(int maxSize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> crossoverPoints;

    if (maxSize > 2) { // Ensure there is room for two distinct points

        int midPoint = maxSize / 2;

        // Generate the first crossover point between 2 and midPoint (inclusive)
        std::uniform_int_distribution<> dis1(0, (midPoint - 2));
        int firstPoint = dis1(gen);
        firstPoint += 2;
        crossoverPoints.push_back(firstPoint);

        // Generate the second crossover point between midPoint (inclusive) and maxSize - 1
        std::uniform_int_distribution<> dis2(0, (maxSize - midPoint - 2));
        int secondPoint = dis2(gen);
        secondPoint += midPoint;
        crossoverPoints.push_back(secondPoint);

        /*std::uniform_int_distribution<> dis(1, maxSize - 2);
        int firstPoint = dis(gen);
        crossoverPoints.push_back(firstPoint);

        std::uniform_int_distribution<> dis2(firstPoint, maxSize - 1);
        int secondPoint = dis2(gen);
        crossoverPoints.push_back(secondPoint);*/
    }

    return crossoverPoints;
}

std::vector<std::vector<int>> performMPCrossover(const std::vector<int>& P1Sol, const std::vector<int>& P2Sol, const std::vector<int>& crossoverPoints) {
    // Initialize vectors to store the crossover results
    std::vector<std::vector<int>> crossoverResult;
    std::vector<int> child1, child2;
    std::vector<int> crossoverPointss = { 2, 3 };


    std::vector<int> P1Soll = { 1, 21, 16, 10, 11, 17, 62, 39 };
    std::vector<int> P2Soll = { 65, 56, 55, 29, 46 };
    /*cout << "Points : ";
    for (int item : crossoverPoints) {
        std::cout << item << " ";
    }
    cout << endl;*/


    // Initialize variables to keep track of which parent is currently being used
    bool useParent1 = true;

    // Perform crossover using the generated crossover points
    int startIndex = 0;
    for (int crossoverPoint : crossoverPointss) {
        const std::vector<int>& currentParent = useParent1 ? P1Sol : P2Sol;
        std::vector<int>& currentChild = useParent1 ? child1 : child2;
        currentChild.insert(currentChild.end(), currentParent.begin() + startIndex, currentParent.begin() + crossoverPoint);

        startIndex = crossoverPoint;
        useParent1 = !useParent1;
    }

    // Copy remaining elements from the last parent
    const std::vector<int>& lastParent = useParent1 ? P1Sol : P2Sol;
    std::vector<int>& lastChild = useParent1 ? child1 : child2;
    lastChild.insert(lastChild.end(), lastParent.begin() + startIndex, lastParent.end());

    // Add the crossover results to the list
    crossoverResult.push_back(child1);
    crossoverResult.push_back(child2);

    // Print the vector
    /*for (const auto& row : crossoverResult) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    int a;
    cin>> a;*/


    return crossoverResult;



}
std::vector<int> generateCrossoverPoints2(int maxSize) {
    std::vector<int> crossoverPoints;
    std::random_device rd;
    std::mt19937 gen(rd());

    if (maxSize > 3) { // Ensure there's enough space for two distinct points
        int midPoint = maxSize / 2;

        // Generate the first crossover point between 2 and midPoint (inclusive)
        std::uniform_int_distribution<> dis1(2, midPoint);
        int firstPoint = dis1(gen);

        // Generate the second crossover point between midPoint (inclusive) and maxSize - 1
        std::uniform_int_distribution<> dis2(midPoint, maxSize - 1);
        int secondPoint = dis2(gen);

        crossoverPoints.push_back(firstPoint);
        crossoverPoints.push_back(secondPoint);
    }

    return crossoverPoints;
}


int getWeightedRandomItem(const std::unordered_map<int, int>& itemFrequency) {
    // Calculate the total weight
    int totalWeight = 0;
    for (const auto& pair : itemFrequency) {
        totalWeight += pair.second;
    }

    // Ensure that the total weight is greater than zero
    if (totalWeight == 0) {
        throw std::invalid_argument("Total weight must be greater than zero.");
    }

    // Generate a random number between 0 and (totalWeight - 1)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, totalWeight - 1);
    int randomNumber = dis(gen);

    // Iterate over items to find the selected one based on weight
    int cumulativeWeight = 0;
    for (const auto& pair : itemFrequency) {
        cumulativeWeight += pair.second;
        if (randomNumber < cumulativeWeight) {
            return pair.first;
        }
    }

    // Fallback: Should not occur unless no items are present
    throw std::logic_error("No item selected. Check the input data.");
}

std::vector<int> standardMutationOperator(const std::vector<int>& C, const std::unordered_set<int>& allItems) {
    // Check if C or allItems is empty
    if (C.empty() || allItems.empty()) {
        return C;
    }

    // Generate a random index to select an item from C for mutation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(C.size() - 1));
    int randomIndex = dis(gen);
    int itemToReplace = C[randomIndex];

    // Convert allItems to a vector
    std::vector<int> allItemsList(allItems.begin(), allItems.end());

    // Ensure the new item is different from the itemToReplace
    int newItem;
    do {
        std::uniform_int_distribution<> dis2(0, static_cast<int>(allItemsList.size() - 1));
        newItem = allItemsList[dis2(gen)];
    } while (newItem == itemToReplace);

    // Replace the selected item in C with the new item
    std::vector<int> mutatedC = C;
    mutatedC[randomIndex] = newItem;

    return mutatedC;
}

std::vector<int> removeDuplicates(const std::vector<int>& original) {
    std::unordered_set<int> uniqueItems(original.begin(), original.end());
    std::vector<int> result(uniqueItems.begin(), uniqueItems.end());
    return result;
}


std::vector<std::vector<int>> deleteItemset(const std::vector<std::vector<int>>& sequence, const std::vector<int>& Pat1, const std::vector<int>& Pat2) {

    std::vector<std::vector<int>> modifiedSequence;
    int pat1Count = 0;
    int pat2Count = 0;

    // Convert Pat1 and Pat2 to unordered_set for fast look-up
    std::unordered_set<int> Pat1Set(Pat1.begin(), Pat1.end());
    std::unordered_set<int> Pat2Set(Pat2.begin(), Pat2.end());

    for (const auto& transaction : sequence) {
        // Create a copy of the transaction to modify
        std::vector<int> modifiedTransaction = transaction;

        // Check if transaction contains all items in Pat1
        bool containsPat1 = std::all_of(Pat1Set.begin(), Pat1Set.end(),
            [&transaction](int item) {
                return std::find(transaction.begin(), transaction.end(), item) != transaction.end();
            });

        // Count and remove items in Pat1 from the transaction
        if (containsPat1) {
            pat1Count++;
            modifiedTransaction.erase(
                std::remove_if(modifiedTransaction.begin(), modifiedTransaction.end(),
                    [&Pat1Set](int item) { return Pat1Set.find(item) != Pat1Set.end(); }),
                modifiedTransaction.end());
        }

        // Check if transaction contains all items in Pat2
        bool containsPat2 = std::all_of(Pat2Set.begin(), Pat2Set.end(),
            [&transaction](int item) {
                return std::find(transaction.begin(), transaction.end(), item) != transaction.end();
            });

        // Count and remove items in Pat2 from the transaction
        if (containsPat2) {
            pat2Count++;
            modifiedTransaction.erase(
                std::remove_if(modifiedTransaction.begin(), modifiedTransaction.end(),
                    [&Pat2Set](int item) { return Pat2Set.find(item) != Pat2Set.end(); }),
                modifiedTransaction.end());
        }

        // Add the modified transaction to the new sequence
        modifiedSequence.push_back(modifiedTransaction);
    }

    Pat1Count = pat1Count;
    Pat2Count = pat2Count;

    return modifiedSequence;
}


int calculateSizeInBits(const std::vector<std::vector<int>>& sequence) {
    int size = 0;
    for (const std::vector<int>& transaction : sequence) {
        for (int item : transaction) {
            size += INT_BITS; // Size of int in bits
        }
    }
    return size;
}



int main() {
    std::string directoryPath = "./classifyWaveform";
    auto allData = loadDataFromDirectory(directoryPath);

    crossValidation(allData, 5);
    //for (const auto& group : allData) {
    //    std::cout << "New Group:" << std::endl;
    //    for (const auto& sequence : group) {
    //        for (int value : sequence) {
    //            std::cout << value << " ";
    //        }
    //        std::cout << std::endl; // End of a sequence
    //    }
    //    std::cout << std::endl; // Separate groups visually
    //}

    //int cc = 1;
    //for (auto& classData : allData) {
    //    cout << "Class is " << cc++ << endl;
    //    std::cout << "Performing cross-validation on class data" << std::endl;
    //    crossValidation(classData, 10);
    //}

    return 0;
}