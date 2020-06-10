[us_en](README.md)
[zh_cn](README_zh_cn.md)
# QtMedicalCurve
Sample of medical curve by Qt Quick

This sample show how to draw medical curve by QPaint and repaint in QML.

## Feature

* provide API to cache data, and update on QPixmap
* support cycle drawing curve by moving deletion bar
* support combining multiple curves
* support up/down scale for curve automatically
* provide curve manager to manager all instance of curve
* provide curve trigger to trigger repaint event in global

## API

### Receive data
```cpp
// slot in CurveManager
CurveManager::onReceiveNewData(CurveData data,int type)
```

### Trigger update
```cpp
// slot in CurveManager
CurveManager::onTriggerCurveUpdate()
```


## Notes
* only support MinGW 64-bit building on Windows

## Screenshots

![](./screenshot/screenshot1.png)

![](./screenshot/screenshot2.png)

![](./screenshot/screenshot3.png)

![](./screenshot/screenshot4.png)

## Know Issue

* sometimes it will not erase clean at start during loop, maybe leave some tiny point at begining