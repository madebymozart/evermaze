package org.cocos2dx.lib;

interface IGameTuningService
{
    int setPreferredResolution(int resolution);
    int setFramePerSecond(int fps);
    int boostUp(int seconds);
    int getAbstractTemperature();
    int setGamePowerSaving(boolean enable);
}
