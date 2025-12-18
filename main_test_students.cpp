/**
 * main_test_hard.cpp
 * Hard Test Suite for ArcadiaEngine
 * Includes edge cases, stress tests, and large inputs.
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <functional>
#include "ArcadiaEngine.h" 
using namespace std;

// ==========================================
// FACTORY FUNCTIONS (LINKING)
// ==========================================
extern "C" {
    PlayerTable* createPlayerTable();
    Leaderboard* createLeaderboard();
    AuctionTree* createAuctionTree();
}

// ==========================================
// TEST UTILITIES
// ==========================================
class HardTestRunner {
    int count = 0;
    int passed = 0;
    int failed = 0;

public:
    void runTest(string testName, bool condition) {
        count++;
        cout << "TEST: " << left << setw(60) << testName;
        if (condition) {
            cout << "[ PASS ]";
            passed++;
        }
        else {
            cout << "[ FAIL ]";
            failed++;
        }
        cout << endl;
    }

    void printSummary() {
        cout << "\n==========================================" << endl;
        cout << "SUMMARY: Passed: " << passed << " | Failed: " << failed << endl;
        cout << "TOTAL TESTS: " << count << endl;
        cout << "==========================================" << endl;
    }
};

HardTestRunner runner;

// ==========================================
// PART A: DATA STRUCTURES - HARD TESTS
// ==========================================
void test_PartA_DataStructures_Hard() {
    cout << "\n--- Part A: Data Structures (Hard) ---" << endl;

    // --- PlayerTable Edge Cases ---
    PlayerTable* table = createPlayerTable();
    runner.runTest("PlayerTable: search non-existent ID", table->search(9999) == "");

    table->insert(1, "Alice");
    table->insert(1, "AliceUpdated"); // test replace
    runner.runTest("PlayerTable: insert duplicate ID", table->search(1) == "AliceUpdated");

    try {
        // fill table to force full exception (assuming table size 101)
        for (int i = 2; i <= 102; i++) table->insert(i, "P" + to_string(i));
        runner.runTest("PlayerTable: overflow table exception", false);
    }
    catch (...) {
        runner.runTest("PlayerTable: overflow table exception", true);
    }
    delete table;

    // --- Leaderboard Edge Cases ---
    Leaderboard* board = createLeaderboard();
    board->addScore(10, 500);
    board->addScore(20, 500);
    board->addScore(15, 500);
    vector<int> top = board->getTopN(3);
    runner.runTest("Leaderboard: tie-breaking by ID", top[0] == 10 && top[1] == 15 && top[2] == 20);

    // remove non-existent
    board->removePlayer(999);
    runner.runTest("Leaderboard: remove non-existent player no crash", true);

    // stress test: insert 1000 players
    for (int i = 100; i < 1100; i++) board->addScore(i, i % 1000);
    runner.runTest("Leaderboard: insert 1000 players", board->getTopN(1).size() == 1);

    delete board;

    // --- AuctionTree Edge Cases ---
    AuctionTree* tree = createAuctionTree();
    tree->insertItem(1, 100);
    tree->insertItem(2, 100); // same price
    runner.runTest("AuctionTree: insert items with same price", true);

    tree->deleteItem(999); // non-existent
    runner.runTest("AuctionTree: delete non-existent item no crash", true);

    // stress insert 1000 nodes
    for (int i = 3; i < 1003; i++) tree->insertItem(i, i * 2);
    runner.runTest("AuctionTree: insert 1000 nodes", true);

    delete tree;
}

// ==========================================
// PART B: INVENTORY SYSTEM - HARD TESTS
// ==========================================
void test_PartB_Inventory_Hard() {
    cout << "\n--- Part B: Inventory System (Hard) ---" << endl;

    // LootSplit edge cases
    vector<int> coinsEmpty;
    runner.runTest("LootSplit: empty coins", InventorySystem::optimizeLootSplit(0, coinsEmpty) == 0);

    vector<int> coinsAllSame(10, 5);
    runner.runTest("LootSplit: all coins same value", InventorySystem::optimizeLootSplit(10, coinsAllSame) == 0);

    // Knapsack edge cases
    vector<pair<int, int>> itemsAllHeavy = { {100, 10}, {200,20} };
    runner.runTest("Knapsack: items too heavy", InventorySystem::maximizeCarryValue(50, itemsAllHeavy) == 0);

    vector<pair<int, int>> itemsNormal = { {1,10}, {2,20}, {3,30}, {4,40}, {5,50} };
    runner.runTest("Knapsack: capacity 10", InventorySystem::maximizeCarryValue(10, itemsNormal) == 100);

    // String decoding
    runner.runTest("StringDecoder: empty string", InventorySystem::countStringPossibilities("") == 1);
    runner.runTest("StringDecoder: contains 'w'", InventorySystem::countStringPossibilities("w") == 0);

    // Long string stress test
    string longU(1000, 'u');
    runner.runTest("StringDecoder: long 'u' string", InventorySystem::countStringPossibilities(longU) > 0);
}

// ==========================================
// PART C: WORLD NAVIGATOR - HARD TESTS
// ==========================================
void test_PartC_Navigator_Hard() {
    cout << "\n--- Part C: World Navigator (Hard) ---" << endl;

    // PathExists
    vector<vector<int>> edgesEmpty;
    runner.runTest("PathExists: empty graph", !WorldNavigator::pathExists(3, edgesEmpty, 0, 2));
    runner.runTest("PathExists: source=dest", WorldNavigator::pathExists(3, edgesEmpty, 1, 1));

    // MinBribeCost
    vector<vector<int>> disconnected = { {0,1,5,0}, {2,3,10,0} };
    runner.runTest("MinBribeCost: disconnected graph", WorldNavigator::minBribeCost(4, 2, 1, 1, disconnected) == -1);

    // Stress test for MST
    vector<vector<int>> bigRoads;
    int n = 1000;
    for (int i = 0; i < n - 1; i++) bigRoads.push_back({ i,i + 1,1,0 });
    runner.runTest("MinBribeCost: large linear graph", WorldNavigator::minBribeCost(n, n - 1, 1, 1, bigRoads) == n - 1);

    // BinarySum
    vector<vector<int>> roads = { {0,1,1}, {1,2,2} };
    runner.runTest("BinarySum: line graph -> '110'", WorldNavigator::sumMinDistancesBinary(3, roads) == "110");
}

// ==========================================
// PART D: SERVER KERNEL - HARD TESTS
// ==========================================
void test_PartD_Kernel_Hard() {
    cout << "\n--- Part D: Server Kernel (Hard) ---" << endl;

    vector<char> tasksEmpty;
    runner.runTest("Scheduler: empty tasks", ServerKernel::minIntervals(tasksEmpty, 2) == 0);

    vector<char> tasksAllSame(10, 'A');
    runner.runTest("Scheduler: all same task, n=2", ServerKernel::minIntervals(tasksAllSame, 2) == (10 - 1) * (2 + 1) + 1);

    vector<char> tasksMixed = { 'A','A','A','B','B','C' };
    runner.runTest("Scheduler: mixed tasks n=2", ServerKernel::minIntervals(tasksMixed, 2) == 8);

    // Stress test large number of tasks
    vector<char> tasksLarge(1000, 'A');
    runner.runTest("Scheduler: 1000 same tasks n=5", ServerKernel::minIntervals(tasksLarge, 5) > 0);
}

// ==========================================
// MAIN
// ==========================================
int main() {
    cout << "Arcadia Engine - Hard Test Suite" << endl;
    cout << "-----------------------------------------" << endl;

    test_PartA_DataStructures_Hard();
    test_PartB_Inventory_Hard();
    test_PartC_Navigator_Hard();
    test_PartD_Kernel_Hard();

    runner.printSummary();
    return 0;
}