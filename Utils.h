#pragma once

template <typename T>
void Wrap(T min, T max, T& value)
{
	if (value < min)
		value = max;
	else if (value > max)
		value = min;
}

template <typename T>
void ToBounds(T min, T max, T& value)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
}
