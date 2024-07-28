#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    LIGHT,
    THERMOSTAT,
    DOOR_LOCK
} DeviceType;

typedef struct SmartDevice {
    int id;
    DeviceType type;
    void (*turnOn)(struct SmartDevice*);
    void (*turnOff)(struct SmartDevice*);
    void (*status)(struct SmartDevice*);
    int state; // 0 for off, 1 for on
} SmartDevice;

typedef struct Light {
    SmartDevice base;
} Light;

typedef struct Thermostat {
    SmartDevice base;
    int temperature;
} Thermostat;

typedef struct DoorLock {
    SmartDevice base;
} DoorLock;

void turnOnLight(SmartDevice* device) {
    device->state = 1;
    printf("Light %d turned on.\n", device->id);
}

void turnOffLight(SmartDevice* device) {
    device->state = 0;
    printf("Light %d turned off.\n", device->id);
}

void statusLight(SmartDevice* device) {
    printf("Light %d is %s.\n", device->id, device->state ? "On" : "Off");
}

void turnOnThermostat(SmartDevice* device) {
    device->state = 1;
    printf("Thermostat %d turned on.\n", device->id);
}

void turnOffThermostat(SmartDevice* device) {
    device->state = 0;
    printf("Thermostat %d turned off.\n", device->id);
}

void statusThermostat(SmartDevice* device) {
    Thermostat* t = (Thermostat*)device;
    printf("Thermostat %d is %s at %d degrees.\n", device->id, device->state ? "On" : "Off", t->temperature);
}

void turnOnDoorLock(SmartDevice* device) {
    device->state = 1;
    printf("DoorLock %d locked.\n", device->id);
}

void turnOffDoorLock(SmartDevice* device) {
    device->state = 0;
    printf("DoorLock %d unlocked.\n", device->id);
}

void statusDoorLock(SmartDevice* device) {
    printf("DoorLock %d is %s.\n", device->id, device->state ? "Locked" : "Unlocked");
}
SmartDevice* createDevice(int id, DeviceType type) {
    SmartDevice* device = NULL;

    switch (type) {
        case LIGHT:
            device = (SmartDevice*)malloc(sizeof(Light));
            device->type = LIGHT;
            device->turnOn = turnOnLight;
            device->turnOff = turnOffLight;
            device->status = statusLight;
            break;
        case THERMOSTAT:
            device = (SmartDevice*)malloc(sizeof(Thermostat));
            device->type = THERMOSTAT;
            device->turnOn = turnOnThermostat;
            device->turnOff = turnOffThermostat;
            device->status = statusThermostat;
            ((Thermostat*)device)->temperature = 70; // Default temperature
            break;
        case DOOR_LOCK:
            device = (SmartDevice*)malloc(sizeof(DoorLock));
            device->type = DOOR_LOCK;
            device->turnOn = turnOnDoorLock;
            device->turnOff = turnOffDoorLock;
            device->status = statusDoorLock;
            break;
    }

    if (device) {
        device->id = id;
        device->state = 0; // Initially off
    }

    return device;
}
typedef struct SmartHomeHub {
    SmartDevice** devices;
    int deviceCount;
} SmartHomeHub;

SmartHomeHub* createHub() {
    SmartHomeHub* hub = (SmartHomeHub*)malloc(sizeof(SmartHomeHub));
    hub->devices = NULL;
    hub->deviceCount = 0;
    return hub;
}

void addDeviceToHub(SmartHomeHub* hub, SmartDevice* device) {
    hub->deviceCount++;
    hub->devices = (SmartDevice**)realloc(hub->devices, hub->deviceCount * sizeof(SmartDevice*));
    hub->devices[hub->deviceCount - 1] = device;
}

void notifyDevices(SmartHomeHub* hub) {
    for (int i = 0; i < hub->deviceCount; i++) {
        hub->devices[i]->status(hub->devices[i]);
    }
}
typedef struct DeviceProxy {
    SmartDevice* device;
    int accessLevel;
} DeviceProxy;

DeviceProxy* createDeviceProxy(SmartDevice* device, int accessLevel) {
    DeviceProxy* proxy = (DeviceProxy*)malloc(sizeof(DeviceProxy));
    proxy->device = device;
    proxy->accessLevel = accessLevel;
    return proxy;
}

void turnOnProxy(DeviceProxy* proxy) {
    if (proxy->accessLevel >= 1) {
        proxy->device->turnOn(proxy->device);
    } else {
        printf("Access denied to turn on device %d.\n", proxy->device->id);
    }
}

void turnOffProxy(DeviceProxy* proxy) {
    if (proxy->accessLevel >= 1) {
        proxy->device->turnOff(proxy->device);
    } else {
        printf("Access denied to turn off device %d.\n", proxy->device->id);
    }
}

void statusProxy(DeviceProxy* proxy) {
    if (proxy->accessLevel >= 0) {
        proxy->device->status(proxy->device);
    } else {
        printf("Access denied to check status of device %d.\n", proxy->device->id);
    }
}
int main() {
    SmartHomeHub* hub = createHub();

    SmartDevice* light1 = createDevice(1, LIGHT);
    SmartDevice* thermostat1 = createDevice(2, THERMOSTAT);
    SmartDevice* doorLock1 = createDevice(3, DOOR_LOCK);

    addDeviceToHub(hub, light1);
    addDeviceToHub(hub, thermostat1);
    addDeviceToHub(hub, doorLock1);

    DeviceProxy* lightProxy = createDeviceProxy(light1, 1);
    DeviceProxy* thermostatProxy = createDeviceProxy(thermostat1, 1);
    DeviceProxy* doorLockProxy = createDeviceProxy(doorLock1, 1);

    turnOnProxy(lightProxy);
    turnOnProxy(thermostatProxy);
    turnOnProxy(doorLockProxy);

    notifyDevices(hub);

    turnOffProxy(lightProxy);
    turnOffProxy(thermostatProxy);
    turnOffProxy(doorLockProxy);

    notifyDevices(hub);

    free(light1);
    free(thermostat1);
    free(doorLock1);
    free(lightProxy);
    free(thermostatProxy);
    free(doorLockProxy);
    free(hub->devices);
    free(hub);

    return 0;
}

