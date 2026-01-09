#include "analyzer.h"
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>

using namespace std;

// Students may use ANY data structure internally

void TripAnalyzer::ingestFile(const string& csvPath) {
    // TODO:
    // - empty file test
    zone.Cpunts.clear();
    slotCounts.clear();
    // - open file
    ifstream file(csvPath);
    if(!file.is_open())
        return;
    // - skip header
    string line;
    getline(file, line);
    
    while(getline(file, line)){
        // - skip empty rows
        if(line.empty()) continue;

        // - parse csv line:
        stringstream ss(line);
        vector<string> tokens;
        string token;
        while(getline(ss, token, ',')){
            tokens.push_back(token);
        }

        //  - skip malformed rows:
        if(tokens.size() != 6) continue;

        // - extract PickupZoneID:
        string zone = tokens[1];

        // - extract pickup hour:
        string datetime = tokens[3];
        int hour = -1;

        size_t spacePos = datetime.find(' ');
        //If there is a space that exists, extract time portion and find the colon seperator
        if(spacePos!=string::npos){         
            string timePart = datetime.substr(spacePos + 1);       
            size_t colonPos = timePart.find(':');
            
            //If colon exists, extract hour as string
            if(colonPos != string::npos){
                string hourStr = timePart.substr(0, colonPos);
                //Convert from string to integer
                try{
                    hour = stoi(hourStr);
                    if(hour<0 || hour>23) continue;    //Invalid hour (hours are between 0-23)
                }
                //Look for errors and skip if the convertion fails
                catch(...){
                    continue;    
                }
            }
        }

        if(hour == -1) continue;   //skip rows that hour extractions have failed

        // - aggregate counts
        zoneCounts[zone]++;
        slotCounts[zone + "_" + to_string(hour)]++;
    }
}

vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    // TODO:
    // - convert from unordered_map to vector 
    vector<ZoneCount> results;
    for(const auto& pair: zoneCounts){
        results.push_back({pair.first, pair.second});
    }
    // - sort by count desc, zone asc:
    sort(results.begin(), results.end(), [](const ZoneCount& a, const ZoneCount& b){
        if(a.count != b.count)
            return a.count > b.count;      //higher count first
        return a.zone < b.zone;           //if counts are equal, then sort alphabetically
    });

    // - return first k:
    if(k > results.size())
        k = results.size();       //Adjust k in case it is not in bound
    return vector<ZoneCount>(results.begin(), results.begin() + k);    //returns first k elements
    
}

vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {
    // TODO:
    // - convert the entries of SlotCounts from map to struct
    vector<SlotCount> results;
    for(const auto& pair : slotCounts){
        const string& key = pair.first;

        size_t pos = key.rfind('_');          //Find the last underscore
        if(pos == string::npos) continue;
    
        try{
            string zone = key.substr(0, pos);     //Zone before the underscore
            int hour = stoi(key.substr(pos+1);   //Time after the underscore
            result.push_back({zone, hour, p.second)};
        }
        catch(...){
            continue;         //skip corrupt entries
        }
    }
    // - sort by count desc, zone asc, hour asc:
    sort(results.begin(), results.end(), [](const SlotCount& a, const SlotCount& b){
        if(a.count != b.count)
            return a.count > b.count;          //higher count first
        if(a.zone != b.zone)
            return a.zone < b.zone;           //if counts are equal, sort zone alphabetically
        return a.hour < b.hour;              //return lower hour first
    });
    // - return first k:
    if(k > (int)results.size())           
        k = results.size();         //adjust k if it is a very large entry
    return vector<SlotCount>(results.begin(), results.begin() + k);           //return top k
}

