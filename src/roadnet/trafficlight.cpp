#include "roadnet/trafficlight.h"
#include "roadnet/roadnet.h"
#include "vehicle/vehicle.h"

namespace CityFlow {

    void TrafficLight::init(int initPhaseIndex) {
        if (intersection->isVirtual)
            return;
        this->curPhaseIndex = initPhaseIndex;
        this->remainDuration = phases[initPhaseIndex].time;
    }

    int TrafficLight::getCurrentPhaseIndex() {
        return this->curPhaseIndex;
    }

    LightPhase &TrafficLight::getCurrentPhase() {
        return this->phases.at(this->curPhaseIndex);
    }

    Intersection &TrafficLight::getIntersection() {
        return *this->intersection;
    }

    std::vector<LightPhase> &TrafficLight::getPhases() {
        return phases;
    }

    void TrafficLight::passTime(double seconds) {
        if(intersection->isVirtual)
            return;
        remainDuration -= seconds;
        while (remainDuration <= 0.0) {
            curPhaseIndex = (curPhaseIndex + 1) % (int) phases.size();
            remainDuration += phases[curPhaseIndex].time;
        }
    }

    void TrafficLight::setPhase(int phaseIndex) {
        curPhaseIndex = phaseIndex;
    }

    void TrafficLight::reset() {
        init(0);
    }

    void TrafficLight::updateMaxQueue() {
    	if (!maxQueueLength) return;
    	for (Road *road : intersection->roads) {
        	if (road->getEndIntersection().getId() == intersection->getId()) {
            	for (Lane &lane : road->getLanes()) {
    				int stopped = 0;
    				for (const Vehicle *vehicle : lane.getVehicles()) {
        				if (vehicle->getSpeed() < 0.1) stopped++;
        				else break; // moving vehicle found, queue ends here
    				}
    				*maxQueueLength = std::max(*maxQueueLength, stopped);
				}
        	}
    	}
	}

}
