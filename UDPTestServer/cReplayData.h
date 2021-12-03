#pragma once
#include <vector>

#include <gen/CarStateArray.pb.h>

class cReplayData
{
public:
	cReplayData() = default;
	~cReplayData() = default;

	void SetFastedRecording(std::vector<bufferProtos::CarStateArray_CarState>, float);
	void SetTickTime(int tick);

	void Integrate();
	bufferProtos::CarStateArray_CarState GetReplayState();

	void StartReplaying();
	bool IsReplaying();

private:
	bool replaying = false;

	std::vector<bufferProtos::CarStateArray_CarState> fastedCarState;
	float fastedLapTime = std::numeric_limits<float>::max();
	float elapsedReplayTime = 0.0f;

	int numberReplayDataPerSecond = 0;
	int tickTime = 0;
	int currentReplayFrame = 0;

};