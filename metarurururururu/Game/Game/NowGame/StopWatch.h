#pragma once
class StopWatch
{
public:
	StopWatch();
	~StopWatch();
	//計測開始。
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	//計測終了。
	void Stop()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		::QueryPerformanceCounter((LARGE_INTEGER*)&end);
		elapsed = double(end - begin) / freq;
		elapsedMill = elapsed * 1000.0;
	}
	//経過時間を取得(単位：秒)。
	double GetElapsed() const
	{
		return elapsed;
	}
	//経過時間を取得(単位：ミリ秒)。
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
private:
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//経過時間(単位：秒)
	double elapsedMill;		//経過時間(単位：ミリ秒)
};

