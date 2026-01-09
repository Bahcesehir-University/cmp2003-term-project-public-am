#pragma once
#include <string>
#include <vector>
#include<unordered_map>

using namespace std;

struct ZoneCount {
    string zone;
    long long count;
};

struct SlotCount {
    string zone;
    int hour;              // 0–23
    long long count;
};

class TripAnalyzer {
private:
    unordered_map<string, long long> zoneCounts;    //total trips per zone
    unordered_map<string, long long> slotCounts;    //total trips per (zone, hour)

public:
    // Parse Trips.csv, skip dirty rows, never crash
    void ingestFile(const std::string& csvPath);

    // Top K zones: count desc, zone asc
    vector<ZoneCount> topZones(int k = 10) const;

    // Top K slots: count desc, zone asc, hour asc
    vector<SlotCount> topBusySlots(int k = 10) const;
};
