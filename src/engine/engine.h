#ifndef CITYFLOW_ENGINE_H
#define CITYFLOW_ENGINE_H

#include "flow/flow.h"
#include "roadnet/roadnet.h"
#include "engine/archive.h"
#include "utility/barrier.h"

#include <mutex>
#include <thread>
#include <set>
#include <random>
#include <fstream>



namespace CityFlow {
using namespace std;

struct SimulationMetrics {
    // totals
    double totalTravelTime;
    double totalWaitTime;
    double totalDelay;
    // averages
    double meanTravelTime;
    double meanWaitTime;
    double meanDelay;
    // counts
    int totalVehicles;
    int throughput;
    // queue
    int maxQueueLength;
};

    class Engine {
        friend class Archive;
    private:
        static bool vehicleCmp(const pair<Vehicle *, double> &a, const pair<Vehicle *, double> &b) {
            return a.second > b.second;
        }

        map<int, pair<Vehicle *, int>> vehiclePool;
        map<string, Vehicle *> vehicleMap;
        vector<set<Vehicle *>> threadVehiclePool;
        vector<vector<Road *>> threadRoadPool;
        vector<vector<Intersection *>> threadIntersectionPool;
        vector<vector<Drivable *>> threadDrivablePool;
        vector<Flow> flows;
        RoadNet roadnet;
        int threadNum;
        double interval;
        bool saveReplay;
        bool saveReplayInConfig; // saveReplay option in config json
        bool warnings;
        vector<pair<Vehicle *, double>> pushBuffer;
        vector<Vehicle *> laneChangeNotifyBuffer;
        set<Vehicle *> vehicleRemoveBuffer;
        rapidjson::Document jsonRoot;
        string stepLog;

        size_t step = 0;
        size_t activeVehicleCount = 0;
        int seed;
        mutex lock;
        Barrier startBarrier, endBarrier;
        vector<thread> threadPool;
        bool finished = false;
        string dir;
        ofstream logOut;

        bool rlTrafficLight;
        bool laneChange;
        int manuallyPushCnt = 0;

        int finishedVehicleCnt = 0;
        double cumulativeTravelTime = 0;
        double cumulativeWaitTime = 0;
        double cumulativeDelay = 0;
        int maxQueueLength = 0;

    private:
        void vehicleControl(Vehicle &vehicle, vector<pair<Vehicle *, double>> &buffer);

        void planRoute();

        void getAction();

        void updateAction();

        void updateLocation();

        void updateLeaderAndGap();

        void planLaneChange();


        void threadController(set<Vehicle *> &vehicles, 
                              vector<Road *> &roads,
                              vector<Intersection *> &intersections,
                              vector<Drivable *> &drivables);

        void threadPlanRoute(const vector<Road *> &roads);

        void threadGetAction(set<Vehicle *> &vehicles);

        void threadUpdateAction(set<Vehicle *> &vehicles);

        void threadUpdateLeaderAndGap(const vector<Drivable *> &drivables);

        void threadUpdateLocation(const vector<Drivable *> &drivables);

        void threadNotifyCross(const vector<Intersection *> &intersections);

        void threadInitSegments(const vector<Road *> &roads);

        void threadPlanLaneChange(const set<Vehicle *> &vehicles);

        void handleWaiting();

        void updateLog();

        bool checkWarning();

        bool loadRoadNet(const string &jsonFile);

        bool loadFlow(const string &jsonFilename);

        vector<const Vehicle *> getRunningVehicles(bool includeWaiting=false) const;

        void scheduleLaneChange();

        void insertShadow(Vehicle *vehicle);

    public:
        mt19937 rnd;

        Engine(const string &configFile, int threadNum);

        double getInterval() const { return interval; }

        bool hasLaneChange() const { return laneChange; }

        bool loadConfig(const string &configFile);

        void notifyCross();

        void nextStep();

        bool checkPriority(int priority);

        void pushVehicle(Vehicle *const vehicle, bool pushToDrivable = true);

        void setLogFile(const string &jsonFile, const string &logFile);

        void initSegments();

        ~Engine();

        // RL related api

        void pushVehicle(const map<string, double> &info, const vector<string> &roads);

        size_t getVehicleCount() const;

        vector<string> getVehicles(bool includeWaiting = false) const;

        map<string, list<Vehicle *>> getLaneVehiclesForIntersection(const string &id) const ;

        SimulationMetrics getFinalMetrics() const;

        map<string, int> getLaneVehicleCount() const;

        map<string, int> getLaneWaitingVehicleCount() const;

        map<string, vector<string>> getLaneVehicles();

        map<string, double> getVehicleSpeed() const;

        map<string, double> getVehicleDistance() const;

        string getLeader(const string &vehicleId) const;

        double getCurrentTime() const;

        size_t getCurrentStep() const;

        double getAverageTravelTime() const;

        vector<string> getIntersectionIds() ;

        int getTotalPhasesForIntersection(const string &id);

        Intersection * getIntersection(const string &id);

        void setTrafficLightPhase(const string &id, int phaseIndex);

        void setReplayLogFile(const string &logFile);

        void setSaveReplay(bool open);

        void setVehicleSpeed(const string &id, double speed);

        void setRandomSeed(int seed) { rnd.seed(seed); }
        
        void reset(bool resetRnd = false);

        // archive
        void load(const Archive &archive) { archive.resume(*this); }
        Archive snapshot() { return Archive(*this); }
        void loadFromFile(const char *fileName);

        bool setRoute(const string &vehicle_id, const vector<string> &anchor_id);

        map<string, string> getVehicleInfo(const string &id) const;
    };

}

#endif //CITYFLOW_ENGINE_H
