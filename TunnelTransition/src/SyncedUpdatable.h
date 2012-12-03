#pragma once

class SyncedUpdatable {
protected:
	int iteration, targetFramerate;
	
	virtual void frameUpdate(float clock) {
	}
	virtual void syncUpdate(float clock) {
	}

public:
	SyncedUpdatable()
	:iteration(0)
	,targetFramerate(60) {
	}
	void update(float clock) {
		frameUpdate(clock);
		int targetIteration = targetFramerate * clock;
		while(iteration < targetIteration) {
			syncUpdate((float) iteration / targetFramerate);
			iteration++;
		}
	}
};