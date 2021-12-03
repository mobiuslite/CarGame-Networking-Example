#include "cReplayData.h"


void cReplayData::SetFastedRecording(std::vector<bufferProtos::CarStateArray_CarState> fastedData, float time)
{
	//Set the new data if the set time is faster than the current lap time
	if (time < this->fastedLapTime)
	{
		this->fastedCarState = fastedData;
		this->fastedLapTime = time;
		this->numberReplayDataPerSecond = fastedData.size() / time;
	}
}
void cReplayData::SetTickTime(int tick)
{
	this->tickTime = tick;
}

void cReplayData::Integrate()
{
	if (replaying)
	{
		this->elapsedReplayTime += tickTime / 1000.0f;
		//this->elapsedSecondTime += deltaTime;

		//If the play time has gone over the elapsed time, stop replaying.
		if (this->elapsedReplayTime > this->fastedLapTime)
		{
			this->replaying = false;
			this->elapsedReplayTime = 0.0f;
		}
		else
		{
			//Sets the replay frame
			this->currentReplayFrame = this->elapsedReplayTime * this->numberReplayDataPerSecond;

			if (this->currentReplayFrame >= this->fastedCarState.size())
			{
				this->replaying = false;
			}

			/*if (this->elapsedSecondTime >= 1.0)
			{
				this->elapsedSecondTime = 0.0f;
				this->currentReplayFrame += this->numberReplayDataPerSecond;

				
			}*/
		}
	}
}

bufferProtos::CarStateArray_CarState cReplayData::GetReplayState()
{
	std::cout << "Outputing frame: " << this->currentReplayFrame << std::endl;
	return this->fastedCarState[currentReplayFrame];
}

void cReplayData::StartReplaying()
{
	if(this->fastedCarState.size() > 0)
		this->replaying = true;
}

bool cReplayData::IsReplaying()
{
	return this->replaying;
}