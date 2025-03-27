#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <random>
#include <algorithm>
#include <set>
#include <numeric> // Include this header for std::iota
#include <chrono>

using namespace std;
using namespace std::chrono;

struct VectorHash
{
    size_t operator()(const std::vector<int> &v) const
    {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v)
        {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

int Pat1Count = 0;
int Pat2Count = 0;
const int INT_BITS = 32;

// std::vector<std::vector<int>> readFromFile(const std::string& filename);
std::vector<std::vector<int>> readFromFile(const std::string &);
int findMinValue(const std::vector<std::vector<int>> &);
int getWeightedRandomItem(const std::unordered_map<int, int> &);
std::vector<int> generateRandomItemset(int longestItemSet, const std::unordered_map<int, int> &itemFrequency);
std::vector<std::vector<int>> performMPCrossover(const std::vector<int> &P1Sol, const std::vector<int> &P2Sol, const std::vector<int> &crossoverPoints);
std::vector<std::vector<int>> MPCrossoverOperator(const std::vector<int> &P1Sol, const std::vector<int> &P2Sol);
std::vector<int> standardMutationOperator(const std::vector<int> &C, const std::unordered_set<int> &allItems);
std::vector<int> removeDuplicates(const std::vector<int> &original);
std::vector<std::vector<int>> deleteItemset(const std::vector<std::vector<int>> &sequence, const std::vector<int> &Pat1, const std::vector<int> &Pat2);
int calculateSizeInBits(const std::vector<std::vector<int>> &sequence);
std::vector<int> generateCrossoverPoints(int maxSize);
int deleteAndCalculateSizeInBits(const std::vector<std::vector<int>> &sequence, const std::vector<std::vector<int>> &finalResult);
int deleteAndCalculateSizeInBits2(const std::vector<std::vector<int>> &sequence, const std::vector<std::vector<int>> &finalResult);
int deleteAndCalculateSizeInBits2Unsorted(const std::vector<std::vector<int>> &sequence, const std::vector<std::vector<int>> &finalResult);
bool containsAll(const std::vector<int> &transaction, const std::vector<int> &pattern);
std::vector<std::vector<int>> SPcrossoverOperator2(const std::vector<int> &P1Sol, const std::vector<int> &P2Sol);
std::vector<std::vector<int>> performSPCrossover2(const std::vector<int> &P1Sol, const std::vector<int> &P2Sol, int cutoffPoint);
std::vector<std::vector<int>> uniformCrossoverAdaptiveSwap(const std::vector<int> &parent1, const std::vector<int> &parent2, double crossoverRate);
// std::vector<std::vector<int>> deleteItemsetSorted(const std::vector<std::vector<int>>& sequence, const std::vector<int>& Pat1, const std::vector<int>& Pat2);
std::vector<int> multiPointMutation(std::vector<int> &C, int mutationPoints, const std::unordered_map<int, int> &itemFrequency);
std::vector<int> generateRandomItemsett(int longestItemSet, const std::vector<int> &population);
std::vector<int> generateCrossoverPoints2(int maxSize);

int main()
{
    int randPatterns = 8;
    int minimumItemValue = 0;
    int maximumItemValue = 0;
    int numberOfIterations = 5;
    int numberOfMutationPoints = 2;

    int AcS = 2; // minimum acceptable resultant pattern size, can be changed for experiment purposes
    string crossOverOperator = "single", mutationOperator = "single";
    std::string inpp = "sample.txt";
    int patterns = 1;
    std::vector<std::vector<int>> sequence;
    sequence = readFromFile(inpp);

    for (int i = 2; i <= 2; i++) // loop for variant of GA
    {

        switch (i)
        {
        case 1:
            crossOverOperator = "single"; // Single Point Crossover
            mutationOperator = "single";  // Single Point Mutation
            break;
        case 2:
            crossOverOperator = "multi"; // Multi Point Crossover
            mutationOperator = "single"; // Single Point Mutation
            break;
        case 3:
            crossOverOperator = "uniform"; // Uniform Crossover
            mutationOperator = "single";   // Single Point Mutation
            break;
        case 4:
            crossOverOperator = "single"; // Single Point Crossover
            mutationOperator = "multi";   // Multi Point Mutations
            break;
        case 5:
            crossOverOperator = "multi"; // Multi Point Crossover
            mutationOperator = "multi";  // Multi Point Mutations
            break;
        case 6:
            crossOverOperator = "uniform"; // Uniform Crossover
            mutationOperator = "multi";    // Multi Point Mutations
            break;
        default:
            // std::cout << "Invalid input. Please enter a number between 1 and 6." << std::endl;
            break;
        }

        std::cout << "Crossover: " << crossOverOperator;
        std::cout << ", Mutation : " << mutationOperator << ", ";
        std::vector<std::vector<int>> subbSequence(sequence); // Copy sequence to subSequence

        // Create an unordered set to store unique items
        std::unordered_set<int> populationSet;

        // Iterate through each transaction in subSequence to add unique items to the set
        for (const std::vector<int> &transaction : subbSequence)
        {
            for (int item : transaction)
            {
                populationSet.insert(item); // Insert items into the set (duplicates are ignored)
            }
        }

        // Convert the unordered set back to a vector if needed
        std::vector<int> population(populationSet.begin(), populationSet.end());
        // ... existing code ...
        std::vector<int> populationn(populationSet.begin(), populationSet.end());
std::cout << "Number of unique items in population: " << populationn.size() << std::endl; // Print the number of unique items
        
       
        std::vector<std::vector<int>> subSequence(sequence); // Copy sequence to subSequence
        std::vector<std::vector<int>> finalResult;           // Create an empty vector of vectors
        int sub = finalResult.size();
        auto startTime = high_resolution_clock::now();
        std::vector<std::vector<int>> remainingSequence;

        std::chrono::nanoseconds totalTimeTaken(0);

        for (int ol = 1; finalResult.size() <= patterns; ol++)
        { // loop for CODE TABLE CT size

            // if (finalResult.size() % 5 == 0) { //&& sub != finalResult.size()) {
            //     sub = finalResult.size();
            //     cout << finalResult.size() << ", ";
            // }

            std::unordered_map<int, int> itemFrequency;
            for (const std::vector<int> &transaction : subSequence)
            {
                for (int item : transaction)
                {
                    itemFrequency[item]++;
                }
            }

            // Print the elements of itemFrequency
            /*for (const auto& pair : itemFrequency) {
                std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
            }*/

            std::unordered_set<int> population;
            for (const std::vector<int> &set : subSequence)
            {
                population.insert(set.begin(), set.end());
            }

            // Print the elements of population
            /*for (const int& elem : population) {
                std::cout << elem << " ";
            }*/
            minimumItemValue = std::numeric_limits<int>::max();
            maximumItemValue = std::numeric_limits<int>::lowest();
            int longestItemSet = 0;

            for (const auto &row : subSequence)
            {
                if (!row.empty())
                {
                    // Update minValue if the first element in the current row is smaller
                    // if (row[0] < minimumItemValue) {
                    //    minimumItemValue = row[0];
                    //}
                    //// Update maxValue if the last element in the current row is larger
                    // if (row.back() > maximumItemValue) {
                    //     maximumItemValue = row.back();
                    // }
                    //  Update longestSize if the current row size is larger
                    if (row.size() > longestItemSet)
                    {
                        longestItemSet = row.size();
                        // cout << longestItemSet << endl;
                    }
                }
            }
            if (longestItemSet == 2)
                break;

            std::vector<int> Pat1;
            std::vector<int> Pat2;
            std::vector<int> FPat1;
            std::vector<int> FPat2;
            int GP1C = 0;
            int GP2C = 0;
            Pat1Count = 0;
            Pat2Count = 0;
            if (AcS)
            {
                Pat1 = generateRandomItemsett(longestItemSet, populationn);
                Pat2 = generateRandomItemsett(longestItemSet, populationn);
            }
            else
            {
                Pat1 = generateRandomItemset(longestItemSet, itemFrequency);
                Pat2 = generateRandomItemset(longestItemSet, itemFrequency);
            }
            for (int pat = 1; pat <= numberOfIterations; pat++) // loop for iterations
            {

                // Sort the vectors
                // std::sort(Pat1.begin(), Pat1.end());
                // std::sort(Pat2.begin(), Pat2.end());

                std::vector<std::vector<int>> crossoverResult;
             

                FPat1 = Pat1;
                FPat2 = Pat2;
                if (crossOverOperator.empty())
                {
                    std::cout << "Wrong crossover operator!" << std::endl;
                    return 0;
                }
                else
                {
                    std::vector<std::vector<int>> crossoverResult;
                    if (crossOverOperator == "single")
                    {
                        crossoverResult = SPcrossoverOperator2(Pat1, Pat2);
                    }
                    else if (crossOverOperator == "multi")
                    {
                        // cout << "Before" << endl;
                        crossoverResult = MPCrossoverOperator(Pat1, Pat2);
                        // cout << "After" << endl;
                    }
                    else if (crossOverOperator == "uniform")
                    {
                        crossoverResult = uniformCrossoverAdaptiveSwap(Pat1, Pat2, 0.5);
                    }
                    else
                    {
                        std::cout << "Wrong crossover operator!" << std::endl;
                        return 0;
                    }
                    Pat1 = crossoverResult[0];
                    Pat2 = crossoverResult[1];
                }

               

                if (mutationOperator.empty())
                {
                    std::cout << "Wrong mutation operator!" << std::endl;
                    return 0;
                }
                else
                {
                    if (mutationOperator == "single")
                    {
                        // cout << "Before" << endl;
                        Pat1 = standardMutationOperator(Pat1, population);
                        Pat2 = standardMutationOperator(Pat2, population);
                        // cout << "After" << endl;
                    }
                    else if (mutationOperator == "multi")
                    {
                        Pat1 = multiPointMutation(Pat1, numberOfMutationPoints, itemFrequency);
                        Pat2 = multiPointMutation(Pat2, numberOfMutationPoints, itemFrequency);
                    }
                    else
                    {
                        std::cout << "Wrong mutation operator!" << std::endl;
                        return 0;
                    }
                    // Pat1 = removeDuplicates(Pat1);
                    // Pat2 = removeDuplicates(Pat2);
                }
                //
               

                std::sort(Pat1.begin(), Pat1.end());
                std::sort(Pat2.begin(), Pat2.end());

                auto patStartTime = high_resolution_clock::now();

                remainingSequence = deleteItemset(subSequence, Pat1, Pat2);
                // cout << "Now Here";
                auto patEndTime = high_resolution_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(patEndTime - patStartTime);
                totalTimeTaken += duration;

                // int a;
                // cin >> a;

                if (Pat1Count > GP1C)
                {
                    GP1C = Pat1Count;
                    FPat1 = Pat1; // Assuming tPat1 is of type std::vector<int> and is already defined.
                }

                if (Pat2Count > GP2C)
                {
                    GP2C = Pat2Count;
                    FPat2 = Pat2; // Assuming tPat2 is of type std::vector<int> and is already defined.
                }

         
            // remainingSequence = deleteItemset(subSequence, FPat1, FPat2);
            subSequence = remainingSequence;

            if (FPat1.size() > AcS)
            {
                /*cout << "Pat 1 : ";
                for (int element : FPat1) {
                    std::cout << element << " ";
                }
                std::cout << std::endl;*/
                finalResult.push_back(FPat1); // Add FPat1 to finalResult if its size is greater than AcS
            }

            if (FPat2.size() > AcS)
            {
                /*  cout << "Pat 2 : ";
                  for (int element : FPat2) {
                      std::cout << element << " ";
                  }
                  std::cout << std::endl;*/
                finalResult.push_back(FPat2); // Add FPat2 to finalResult if its size is greater than AcS
            }

  
        } // loop for CODE TABLE CT size
        // std::cout << ": \n";
        for (const auto &subVec : finalResult)
        {
            for (int value : subVec)
            {
                //       std::cout << value << " ";
            }
            // std::cout << std::endl;
        }

        /*std::vector<std::vector<int>> finalResultt = {
        {8,18,56},
        {54, 61},
        {7},
        {10, 40},
        {21,42,57},
        {1, 32, 46}
        };*/

        double totalTimeSeconds = std::chrono::duration<double>(totalTimeTaken).count();

        auto endTime = high_resolution_clock::now();
        // cout << "Time : " << duration_cast<microseconds>(endTime - startTime).count() / 1000000.0;
        int originalDatabaseSize = calculateSizeInBits(sequence);
        
        int compressSizeInBits2 = deleteAndCalculateSizeInBits2Unsorted(sequence, finalResult);
        // float compression = static_cast<float>(compressSizeInBits) / originalDatabaseSize * 100;
        float compressionWithCodeTable = static_cast<float>(compressSizeInBits2) / originalDatabaseSize * 100;

        // std::cout << "\nCompression: " << compression << "%" << std::endl;
        std::cout << "Compression with code table: " << compressionWithCodeTable << endl; //"%" << "Items: " << finalResult.size() << std::endl;
        std::cout << "Total time taken: " << totalTimeSeconds << " seconds";

    } // loop for variant of GA
    return 0;
}

std::vector<int> generateRandomItemsett(int longestItemSet, const std::vector<int> &population)
{
    std::vector<int> randomSet;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Check if population is empty or longestItemSet is less than 2
    if (population.empty() || longestItemSet <= 2)
    {
        return randomSet; // Return an empty set if conditions are not met
    }

    // Random size between 2 and the minimum of longestItemSet - 1 and population size
    int maxSize = std::min(longestItemSet - 1, static_cast<int>(population.size()));
    std::uniform_int_distribution<> dis(2, maxSize);
    int randomItemsetSize = dis(gen);

    while (randomSet.size() < randomItemsetSize)
    {
        // Select a random index from the population
        std::uniform_int_distribution<> indexDis(0, population.size() - 1);
        int randomIndex = indexDis(gen);
        int randomNumber = population[randomIndex];

        // Ensure no repetition
        if (std::find(randomSet.begin(), randomSet.end(), randomNumber) == randomSet.end())
        {
            randomSet.push_back(randomNumber);
        }
    }

    return randomSet;
}
std::vector<std::vector<int>> readFromFile(const std::string &filename)
{
    std::vector<std::vector<int>> sequence;
    std::ifstream file(filename);

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<int> row;
            std::istringstream iss(line);
            int num;
            while (iss >> num)
            {
                row.push_back(num);
            }
            sequence.push_back(row);
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return sequence;
}
int findMinValue(const std::vector<std::vector<int>> &subSequence)
{
    // Check if subSequence is empty
    if (subSequence.empty())
    {
        throw std::runtime_error("subSequence is empty");
    }

    // Initialize minValue to the largest possible integer value
    int minValue = std::numeric_limits<int>::max();

    // Iterate through each row in the 2D vector
    for (const auto &row : subSequence)
    {
        if (!row.empty() && row[0] < minValue)
        {
            minValue = row[0];
        }
    }

    return minValue;
}

std::vector<int> generateRandomItemset(int longestItemSet, const std::unordered_map<int, int> &itemFrequency)
{
    std::vector<int> randomSet;
    std::random_device rd;
    std::mt19937 gen(rd());
    //    cout << longestItemSet - 1 << ", ";
    std::uniform_int_distribution<> dis(2, longestItemSet - 1); // Random size between 2 and longestItemSet - 1
    int randomItemsetSize = dis(gen);

    while (randomSet.size() < randomItemsetSize)
    {
        int randomNumber = getWeightedRandomItem(itemFrequency);
        if (std::find(randomSet.begin(), randomSet.end(), randomNumber) == randomSet.end())
        { // Ensure no repetition
            randomSet.push_back(randomNumber);
        }
    }

    return randomSet;
}

int getWeightedRandomItem(const std::unordered_map<int, int> &itemFrequency)
{
    // Calculate the total weight
    int totalWeight = 0;
    for (const auto &pair : itemFrequency)
    {
        totalWeight += pair.second;
    }

    // Ensure that the total weight is greater than zero
    if (totalWeight == 0)
    {
        throw std::invalid_argument("Total weight must be greater than zero.");
    }

    // Generate a random number between 0 and (totalWeight - 1)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, totalWeight - 1);
    int randomNumber = dis(gen);

    // Iterate over items to find the selected one based on weight
    int cumulativeWeight = 0;
    for (const auto &pair : itemFrequency)
    {
        cumulativeWeight += pair.second;
        if (randomNumber < cumulativeWeight)
        {
            return pair.first;
        }
    }

    // Fallback: Should not occur unless no items are present
    throw std::logic_error("No item selected. Check the input data.");
}

std::vector<std::vector<int>> MPCrossoverOperator(const std::vector<int> &P1Sol, const std::vector<int> &P2Sol)
{
    int smallestSize = std::min(P1Sol.size(), P2Sol.size());
    if (smallestSize <= 3)
    {
        // Not enough size to perform crossover with the given rules
        return {P1Sol, P2Sol}; // return parents as is, no crossover possible
    }
    std::vector<int> crossoverPoints = generateCrossoverPoints2(smallestSize);
    /*for (int item : crossoverPoints) {
        std::cout << item << " ";
    }*/
    return performMPCrossover(P1Sol, P2Sol, crossoverPoints);
}

std::vector<int> generateCrossoverPoints(int maxSize)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> crossoverPoints;

    if (maxSize > 2)
    { // Ensure there is room for two distinct points

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

std::vector<std::vector<int>> performMPCrossover(const std::vector<int> &P1Sol, const std::vector<int> &P2Sol, const std::vector<int> &crossoverPoints)
{
    // Initialize vectors to store the crossover results
    std::vector<std::vector<int>> crossoverResult;
    std::vector<int> child1, child2;
    std::vector<int> crossoverPointss = {2, 3};

    std::vector<int> P1Soll = {1, 21, 16, 10, 11, 17, 62, 39};
    std::vector<int> P2Soll = {65, 56, 55, 29, 46};
    /*cout << "Points : ";
    for (int item : crossoverPoints) {
        std::cout << item << " ";
    }
    cout << endl;*/

    // Initialize variables to keep track of which parent is currently being used
    bool useParent1 = true;

    // Perform crossover using the generated crossover points
    int startIndex = 0;
    for (int crossoverPoint : crossoverPointss)
    {
        const std::vector<int> &currentParent = useParent1 ? P1Sol : P2Sol;
        std::vector<int> &currentChild = useParent1 ? child1 : child2;
        currentChild.insert(currentChild.end(), currentParent.begin() + startIndex, currentParent.begin() + crossoverPoint);

        startIndex = crossoverPoint;
        useParent1 = !useParent1;
    }

    // Copy remaining elements from the last parent
    const std::vector<int> &lastParent = useParent1 ? P1Sol : P2Sol;
    std::vector<int> &lastChild = useParent1 ? child1 : child2;
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

// std::vector<std::vector<int>> performMPCrossover(const std::vector<int>& P1Sol, const std::vector<int>& P2Sol, const std::vector<int>& crossoverPoints) {
//     std::vector<std::vector<int>> crossoverResult;
//     std::vector<int> child1, child2;
//
//     bool useParent1 = true;
//     int startIndex = 0;
//
//     for (int crossoverPoint : crossoverPoints) {
//         if (useParent1) {
//             child1.insert(child1.end(), P1Sol.begin() + startIndex, P1Sol.begin() + crossoverPoint);
//             child2.insert(child2.end(), P2Sol.begin() + startIndex, P2Sol.begin() + crossoverPoint);
//         }
//         else {
//             child1.insert(child1.end(), P2Sol.begin() + startIndex, P2Sol.begin() + crossoverPoint);
//             child2.insert(child2.end(), P1Sol.begin() + startIndex, P1Sol.begin() + crossoverPoint);
//         }
//
//         startIndex = crossoverPoint;
//         useParent1 = !useParent1;
//     }
//
//     // Add the remaining segments
//     if (useParent1) {
//         child1.insert(child1.end(), P1Sol.begin() + startIndex, P1Sol.end());
//         child2.insert(child2.end(), P2Sol.begin() + startIndex, P2Sol.end());
//     }
//     else {
//         child1.insert(child1.end(), P2Sol.begin() + startIndex, P2Sol.end());
//         child2.insert(child2.end(), P1Sol.begin() + startIndex, P1Sol.end());
//     }
//
//     crossoverResult.push_back(child1);
//     crossoverResult.push_back(child2);
//
//     return crossoverResult;
// }

std::vector<int> standardMutationOperator(const std::vector<int> &C, const std::unordered_set<int> &allItems)
{
    // Check if C or allItems is empty
    if (C.empty() || allItems.empty())
    {
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
    do
    {
        std::uniform_int_distribution<> dis2(0, static_cast<int>(allItemsList.size() - 1));
        newItem = allItemsList[dis2(gen)];
    } while (newItem == itemToReplace);

    // Replace the selected item in C with the new item
    std::vector<int> mutatedC = C;
    mutatedC[randomIndex] = newItem;

    return mutatedC;
}

std::vector<int> removeDuplicates(const std::vector<int> &original)
{
    std::unordered_set<int> uniqueItems(original.begin(), original.end());
    std::vector<int> result(uniqueItems.begin(), uniqueItems.end());
    return result;
}

//
// std::vector<std::vector<int>> deleteItemset(const std::vector<std::vector<int>>& sequence, const std::vector<int>& Pat1, const std::vector<int>& Pat2) {
//
//    std::vector<std::vector<int>> modifiedSequence;
//    int pat1Count = 0;
//    int pat2Count = 0;
//
//    // Convert Pat1 and Pat2 to unordered_set for fast look-up
//    std::unordered_set<int> Pat1Set(Pat1.begin(), Pat1.end());
//    std::unordered_set<int> Pat2Set(Pat2.begin(), Pat2.end());
//
//    for (const auto& transaction : sequence) {
//        // Create a copy of the transaction to modify
//        std::vector<int> modifiedTransaction = transaction;
//
//        // Check if transaction contains all items in Pat1
//        bool containsPat1 = std::all_of(Pat1Set.begin(), Pat1Set.end(),
//            [&transaction](int item) {
//                return std::find(transaction.begin(), transaction.end(), item) != transaction.end();
//            });
//
//        // Count and remove items in Pat1 from the transaction
//        if (containsPat1) {
//            pat1Count++;
//            modifiedTransaction.erase(
//                std::remove_if(modifiedTransaction.begin(), modifiedTransaction.end(),
//                    [&Pat1Set](int item) { return Pat1Set.find(item) != Pat1Set.end(); }),
//                modifiedTransaction.end());
//        }
//
//        // Check if transaction contains all items in Pat2
//        bool containsPat2 = std::all_of(Pat2Set.begin(), Pat2Set.end(),
//            [&transaction](int item) {
//                return std::find(transaction.begin(), transaction.end(), item) != transaction.end();
//            });
//
//        // Count and remove items in Pat2 from the transaction
//        if (containsPat2) {
//            pat2Count++;
//            modifiedTransaction.erase(
//                std::remove_if(modifiedTransaction.begin(), modifiedTransaction.end(),
//                    [&Pat2Set](int item) { return Pat2Set.find(item) != Pat2Set.end(); }),
//                modifiedTransaction.end());
//        }
//
//        // Add the modified transaction to the new sequence
//        modifiedSequence.push_back(modifiedTransaction);
//    }
//
//    Pat1Count = pat1Count;
//    Pat2Count = pat2Count;
//
//    return modifiedSequence;
//}

std::vector<std::vector<int>> deleteItemset(const std::vector<std::vector<int>> &sequence, const std::vector<int> &Pat1, const std::vector<int> &Pat2)
{

    std::vector<std::vector<int>> modifiedSequence;
    int pat1Count = 0;
    int pat2Count = 0;

    // Convert Pat1 and Pat2 to unordered_set for fast look-up
    std::unordered_set<int> Pat1Set(Pat1.begin(), Pat1.end());
    std::unordered_set<int> Pat2Set(Pat2.begin(), Pat2.end());

    for (const auto &transaction : sequence)
    {
        // Create a copy of the transaction to modify
        std::vector<int> modifiedTransaction = transaction;

        // Check if transaction contains all items in Pat1
        bool containsPat1 = std::all_of(Pat1Set.begin(), Pat1Set.end(),
                                        [&transaction](int item)
                                        {
                                            return std::find(transaction.begin(), transaction.end(), item) != transaction.end();
                                        });

        // Count and remove items in Pat1 from the transaction
        if (containsPat1)
        {
            pat1Count++;
            modifiedTransaction.erase(
                std::remove_if(modifiedTransaction.begin(), modifiedTransaction.end(),
                               [&Pat1Set](int item)
                               { return Pat1Set.find(item) != Pat1Set.end(); }),
                modifiedTransaction.end());
        }

        // Check if transaction contains all items in Pat2
        bool containsPat2 = std::all_of(Pat2Set.begin(), Pat2Set.end(),
                                        [&transaction](int item)
                                        {
                                            return std::find(transaction.begin(), transaction.end(), item) != transaction.end();
                                        });

        // Count and remove items in Pat2 from the transaction
        if (containsPat2)
        {
            pat2Count++;
            modifiedTransaction.erase(
                std::remove_if(modifiedTransaction.begin(), modifiedTransaction.end(),
                               [&Pat2Set](int item)
                               { return Pat2Set.find(item) != Pat2Set.end(); }),
                modifiedTransaction.end());
        }

        // Add the modified transaction to the new sequence
        modifiedSequence.push_back(modifiedTransaction);
    }

    Pat1Count = pat1Count;
    Pat2Count = pat2Count;

    return modifiedSequence;
}

int calculateSizeInBits(const std::vector<std::vector<int>> &sequence)
{
    int size = 0;
    for (const std::vector<int> &transaction : sequence)
    {
        for (int item : transaction)
        {
            size += INT_BITS; // Size of int in bits
        }
    }
    return size;
}

int deleteAndCalculateSizeInBits(const std::vector<std::vector<int>> &sequence, const std::vector<std::vector<int>> &finalResult)
{
    std::vector<std::vector<int>> modifiedSequence;

    // Process each transaction in the sequence
    for (const auto &transaction : sequence)
    {
        std::vector<int> modifiedTransaction(transaction);

        // Iterate through transactions in finalResult
        for (const auto &finalTransaction : finalResult)
        {
            // Check if all items in finalTransaction exist in modifiedTransaction
            bool allExist = std::all_of(finalTransaction.begin(), finalTransaction.end(),
                                        [&modifiedTransaction](int item)
                                        {
                                            return std::find(modifiedTransaction.begin(), modifiedTransaction.end(), item) != modifiedTransaction.end();
                                        });

            if (allExist)
            {
                // If all items match, remove the finalTransaction items from modifiedTransaction
                for (int item : finalTransaction)
                {
                    modifiedTransaction.erase(std::remove(modifiedTransaction.begin(), modifiedTransaction.end(), item), modifiedTransaction.end());
                }
            }
        }

        // Add the modified transaction to the result
        modifiedSequence.push_back(modifiedTransaction);
    }

    // Calculate the size in bits of the modified sequence
    int totalSizeInBits = 0;
    for (const auto &transaction : modifiedSequence)
    {
        totalSizeInBits += transaction.size() * INT_BITS;
    }

    return totalSizeInBits;
}

int deleteAndCalculateSizeInBits2Sorted(const std::vector<std::vector<int>> &sequence, const std::vector<std::vector<int>> &finalResult)
{
    std::vector<std::vector<int>> modifiedSequence;
    std::unordered_map<std::vector<int>, int, VectorHash> patternCount;

    // Initialize pattern counts to zero
    for (const auto &pattern : finalResult)
    {
        patternCount[pattern] = 0;
    }

    // Process each transaction in sequence
    for (const auto &transaction : sequence)
    {
        std::vector<int> modifiedTransaction(transaction);

        for (const auto &finalTransaction : finalResult)
        {
            if (std::includes(transaction.begin(), transaction.end(), finalTransaction.begin(), finalTransaction.end()))
            {
                // Increment the count for the pattern
                patternCount[finalTransaction]++;

                // Remove the finalTransaction items from modifiedTransaction
                for (const int item : finalTransaction)
                {
                    modifiedTransaction.erase(std::remove(modifiedTransaction.begin(), modifiedTransaction.end(), item), modifiedTransaction.end());
                }
            }
        }

        // Add the modified transaction to the result
        modifiedSequence.push_back(modifiedTransaction);
    }

    // Calculate the size in bits of the modified sequence
    int totalSizeInBits = 0;
    for (const auto &transaction : modifiedSequence)
    {
        totalSizeInBits += transaction.size() * 32; // Assuming 32 bits per integer
    }

    // Add the size of the transactions in finalResult multiplied by their counts
    for (const auto &entry : patternCount)
    {
        int patternSize = 0;
        if (entry.second != 0)
        {
            int keySize = entry.first.size();
            // Calculate the bit size for entry.second using logarithm base 2
            int valueBitSize = static_cast<int>(std::ceil(std::log2(entry.second + 1)));
            int patternBitSize = keySize * 32;
            patternSize = valueBitSize + patternBitSize;
        }
        totalSizeInBits += patternSize;
    }

    return totalSizeInBits;
}
std::vector<int> generateCrossoverPoints2(int maxSize)
{
    std::vector<int> crossoverPoints;
    std::random_device rd;
    std::mt19937 gen(rd());

    if (maxSize > 3)
    { // Ensure there's enough space for two distinct points
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

int deleteAndCalculateSizeInBits2Unsorted(const std::vector<std::vector<int>> &sequence, const std::vector<std::vector<int>> &finalResult)
{
    std::vector<std::vector<int>> modifiedSequence;
    std::unordered_map<std::vector<int>, int, VectorHash> patternCount;

    // Initialize pattern counts to zero
    for (const auto &pattern : finalResult)
    {
        patternCount[pattern] = 0;
    }

    // Process each transaction in sequence
    for (const auto &transaction : sequence)
    {
        std::vector<int> modifiedTransaction(transaction);

        for (const auto &finalTransaction : finalResult)
        {
            if (containsAll(transaction, finalTransaction))
            {
                // Increment the count for the pattern
                patternCount[finalTransaction]++;

                // Remove the finalTransaction items from modifiedTransaction
                for (const int item : finalTransaction)
                {
                    modifiedTransaction.erase(std::remove(modifiedTransaction.begin(), modifiedTransaction.end(), item), modifiedTransaction.end());
                }
            }
        }

        // Add the modified transaction to the result
        modifiedSequence.push_back(modifiedTransaction);
    }

    // Calculate the size in bits of the modified sequence
    int totalSizeInBits = 0;
    for (const auto &transaction : modifiedSequence)
    {
        totalSizeInBits += transaction.size() * 32; // Assuming 32 bits per integer
    }

    // Add the size of the transactions in finalResult multiplied by their counts
    for (const auto &entry : patternCount)
    {
        int patternSize = 0;
        if (entry.second != 0)
        {
            int keySize = entry.first.size();
            // Calculate the bit size for entry.second using logarithm base 2
            int valueBitSize = static_cast<int>(std::ceil(std::log2(entry.second + 1)));
            int patternBitSize = keySize * 32;
            patternSize = valueBitSize + patternBitSize;
        }
        totalSizeInBits += patternSize;
    }

    return totalSizeInBits;
}

// Helper function to check if all elements of a pattern are in a transaction
bool containsAll(const std::vector<int> &transaction, const std::vector<int> &pattern)
{
    std::unordered_set<int> transactionSet(transaction.begin(), transaction.end());
    for (int item : pattern)
    {
        if (transactionSet.find(item) == transactionSet.end())
        {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<int>> SPcrossoverOperator2(const std::vector<int> &P1Sol, const std::vector<int> &P2Sol)
{
    //  std::cout << "here" << endl;

    int smallestSize = std::min(P1Sol.size(), P2Sol.size());
    if (smallestSize <= 2)
    {
        // Not enough size to perform crossover with the given rules
        return {P1Sol, P2Sol}; // return parents as is, no crossover possible
    }
    // cout << "Now here : " << smallestSize<<endl;
    std::random_device rd;                                    // Seed for the random number engine
    std::mt19937 gen(rd());                                   // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, smallestSize - 1); // Range is [2, smallestSize-1]

    int cutoffPoint = dis(gen); // Randomly generate the cutoff point within the allowed range
    // cout << cutoffPoint << endl;
    return performSPCrossover2(P1Sol, P2Sol, cutoffPoint);
}

std::vector<std::vector<int>> performSPCrossover2(const std::vector<int> &P1Sol, const std::vector<int> &P2Sol, int cutoffPoint)
{
    std::vector<int> child1(P1Sol.begin(), P1Sol.begin() + cutoffPoint);
    child1.insert(child1.begin(), P2Sol.begin() + cutoffPoint, P2Sol.end());

    std::vector<int> child2(P2Sol.begin(), P2Sol.begin() + cutoffPoint);
    // child2.insert(child2.begin(), P1Sol.begin() + cutoffPoint, P1Sol.end());
    child2.insert(child2.begin(), P1Sol.begin() + cutoffPoint, P1Sol.begin() + cutoffPoint);

    std::vector<std::vector<int>> crossoverResult = {child1, child2};
    return crossoverResult;
}

std::vector<std::vector<int>> uniformCrossoverAdaptiveSwap(const std::vector<int> &parent1, const std::vector<int> &parent2, double crossoverRate)
{
    // Determine which is the shorter and which is the longer parent
    const std::vector<int> &shorter = parent1.size() <= parent2.size() ? parent1 : parent2;
    const std::vector<int> &longer = parent1.size() > parent2.size() ? parent1 : parent2;

    std::vector<int> shorterCopy = shorter; // Copy of the shorter vector for modifications
    std::vector<int> longerCopy = longer;   // Copy of the longer vector for modifications

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    // Create a list of indices for the longer parent and shuffle them
    std::vector<int> indices(longer.size());
    std::iota(indices.begin(), indices.end(), 0); // Fill indices with 0, 1, 2, ..., longer.size() - 1
    std::shuffle(indices.begin(), indices.end(), gen);

    // Perform swaps based on crossoverRate, only considering the size of the shorter parent
    for (size_t i = 0; i < shorter.size(); ++i)
    {
        if (dis(gen) < crossoverRate)
        {
            // Perform a swap between the shorter and a selected index from the longer
            int indexFromLonger = indices[i];
            std::swap(shorterCopy[i], longerCopy[indexFromLonger]);
        }
    }

    // Prepare the crossover result containing the updated parents
    std::vector<std::vector<int>> crossoverResult;
    crossoverResult.push_back(shorterCopy); // Clone parent1 to avoid external modifications
    crossoverResult.push_back(longerCopy);  // Clone parent2 to avoid external modifications

    return crossoverResult;
}

std::vector<int> multiPointMutation(std::vector<int> &C, int mutationPoints, const std::unordered_map<int, int> &itemFrequency)
{
    /*std::cout << "Pat before mutation: ";
for (int item : C) {
    std::cout << item << " ";
}
std::cout << std::endl;*/
    if (mutationPoints <= 0 || C.empty() || C.size() < mutationPoints)
    {
        return C;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::set<int> usedIndices;
    for (int i = 0; i < mutationPoints; i++)
    {
        int randomIndex;
        do
        {
            std::uniform_int_distribution<> dis(0, C.size() - 1);
            randomIndex = dis(gen);
        } while (usedIndices.find(randomIndex) != usedIndices.end());
        usedIndices.insert(randomIndex);
        int itemToReplace = C[randomIndex];
        int newItem;
        do
        {
            newItem = getWeightedRandomItem(itemFrequency);
        } while (newItem == itemToReplace);
        C[randomIndex] = newItem;
    }
    /*  std::cout << "Pat after mutation: ";
      for (int item : C) {
          std::cout << item << " ";
      }
      cout << endl;*/
    return C;
}

