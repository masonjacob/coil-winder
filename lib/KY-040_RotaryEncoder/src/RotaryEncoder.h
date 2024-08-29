#ifndef _RotaryEncoder_h
#define _RotaryEncoder_h

#include <OneButtonTiny.h>

class RotaryEncoder
{
protected:
	const byte DT_Pin, CLK_Pin, SW_Pin;
	int currentStateCLK;
	int lastStateCLK;
	int position = 0;
	unsigned long lastButtonPress = 0;
	OneButtonTiny *button;

	typedef void (*callbackFunctionWithParameter)(int);
	callbackFunctionWithParameter positionChangedCallback;

	void updatePosition()
	{
		currentStateCLK = digitalRead(CLK_Pin);

		if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
		{
			if (digitalRead(DT_Pin) == currentStateCLK)
			{
				position--;
			}
			else
			{
				position++;
			}
			positionChangedCallback(position);
		}
		lastStateCLK = currentStateCLK;
	}

public:
	RotaryEncoder(const byte DT_Pin, const byte CLK_Pin, const byte SW_Pin, const callbackFunction buttonCallback, const callbackFunctionWithParameter positionChangedCallback) : DT_Pin(DT_Pin), CLK_Pin(CLK_Pin), SW_Pin(SW_Pin), positionChangedCallback(positionChangedCallback)
	{
		pinMode(DT_Pin, INPUT);
		pinMode(CLK_Pin, INPUT);
		pinMode(SW_Pin, INPUT);
		button = new OneButtonTiny(SW_Pin, true);
		button->attachClick(buttonCallback);
	}

	void begin()
	{
		lastStateCLK = digitalRead(CLK_Pin);
	}

	const int GetPosition()
	{
		return position;
	}

	void tick()
	{
		this->updatePosition();
		button->tick();
	}
};
#endif