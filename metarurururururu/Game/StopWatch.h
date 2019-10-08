#pragma once
class StopWatch
{
public:
	StopWatch();
	~StopWatch();
	//�v���J�n�B
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	//�v���I���B
	void Stop()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		::QueryPerformanceCounter((LARGE_INTEGER*)&end);
		elapsed = double(end - begin) / freq;
		elapsedMill = elapsed * 1000.0;
	}
	//�o�ߎ��Ԃ��擾(�P�ʁF�b)�B
	double GetElapsed() const
	{
		return elapsed;
	}
	//�o�ߎ��Ԃ��擾(�P�ʁF�~���b)�B
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
private:
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//�o�ߎ���(�P�ʁF�b)
	double elapsedMill;		//�o�ߎ���(�P�ʁF�~���b)
};

