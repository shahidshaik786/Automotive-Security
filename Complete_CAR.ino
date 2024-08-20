#include <SPI.h>
#include "mcp2515_can.h"

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

const int SPI_CS_PIN = 9;
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin

bool sendData = false; // Flag to control data sending

// Global variable declarations
int engineSpeed = 0;
int throttlePosition = 0;
int engineTemperature = 0;
int fuelInjectionTiming = 0;
int ignitionTiming = 0;

int gearPosition = 0;
int transmissionFluidTemperature = 0;
int shiftCommands = 0;
int torqueConverterLockup = 0;

int doorStatus = 0;
int windowPosition = 0;
int headlightsOn = 0;
int indicatorsOn = 0;
int hornStatus = 0;
int wiperStatus = 0;

int wheelSpeed = 0;
int brakePressure = 0;
int absActive = 0;
int tractionControlActive = 0;

int airbagDeployment = 0;
int crashSensorsActive = 0;
int seatbeltTensionerActive = 0;

int audioVolume = 0;
int radioOn = 0;
int gpsOn = 0;
int bluetoothConnected = 0;

int cabinTemperature = 0;
int fanSpeed = 0;
int acCompressorOn = 0;
int airVentPosition = 0;

int fuelLevel = 0;
int oxygenSensorReadings = 0;
int emissionControls = 0;
int batteryVoltage = 0;
int alternatorOutput = 0;

int steeringAngle = 0;
int powerSteeringPressure = 0;

int tirePressure = 0;
int tireTemperature = 0;

// Global variable declarations for ADAS
int laneKeepAssist = 0;
int adaptiveCruiseControl = 0;
int collisionDetection = 0;
int parkingAssist = 0;

// Define fault codes
const int ENGINE_FAULT_CODE = 0x01;
const int TRANSMISSION_FAULT_CODE = 0x02;
const int BODY_CONTROL_FAULT_CODE = 0x03;
const int ABS_FAULT_CODE = 0x04;
const int AIRBAG_FAULT_CODE = 0x05;
const int INFOTAINMENT_FAULT_CODE = 0x06;
const int CLIMATE_CONTROL_FAULT_CODE = 0x07;
const int PCM_FAULT_CODE = 0x08;
const int STEERING_FAULT_CODE = 0x09;
const int TPMS_FAULT_CODE = 0x0A;
const int ADAS_FAULT_CODE = 0x0B;

// Timing variables for sending data
unsigned long previousMillis[10] = {0}; // Array to store previous millis for each data type
const unsigned long interval = 2000; // Interval of 2 seconds

void setup() {
    SERIAL.begin(115200);
    while (!Serial) {};

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
        SERIAL.println("CAN BUS Shield init fail");
        SERIAL.println("Init CAN BUS Shield again");
        delay(100);
    }
    SERIAL.println("CAN BUS Shield init ok!");
}

void loop() {
    unsigned long currentMillis = millis();

    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read until newline

        // Process commands
        if (command.equals("start")) {
            sendData = true;  // Start sending CAN data
            SERIAL.println("Data sending started.");
        } else if (command.equals("stop")) {
            sendData = false; // Stop sending CAN data
            SERIAL.println("Data sending stopped.");
        }
    }

    if (sendData) {
        // Update simulated real-time values
        updateSimulatedValues();

        // Check if it's time to send each type of data
        if (currentMillis - previousMillis[0] >= interval) {
            previousMillis[0] = currentMillis;
            sendEngineData();
        }

        if (currentMillis - previousMillis[1] >= interval) {
            previousMillis[1] = currentMillis;
            sendTransmissionData();
        }

        if (currentMillis - previousMillis[2] >= interval) {
            previousMillis[2] = currentMillis;
            sendBodyControlData();
        }

        if (currentMillis - previousMillis[3] >= interval) {
            previousMillis[3] = currentMillis;
            sendAbsData();
        }

        if (currentMillis - previousMillis[4] >= interval) {
            previousMillis[4] = currentMillis;
            sendAirbagData();
        }

        if (currentMillis - previousMillis[5] >= interval) {
            previousMillis[5] = currentMillis;
            sendInfotainmentData();
        }

        if (currentMillis - previousMillis[6] >= interval) {
            previousMillis[6] = currentMillis;
            sendClimateControlData();
        }

        if (currentMillis - previousMillis[7] >= interval) {
            previousMillis[7] = currentMillis;
            sendPcmData();
        }

        if (currentMillis - previousMillis[8] >= interval) {
            previousMillis[8] = currentMillis;
            sendSteeringData();
        }

        if (currentMillis - previousMillis[9] >= interval) {
            previousMillis[9] = currentMillis;
            sendTpmsData();
        }

        // Optional: Add a delay to prevent flooding in case the sending functions are too frequent
        delay(10); // Adjust this delay as needed
    }
}

void updateSimulatedValues() {
    // Simulate engine data
    engineSpeed = (engineSpeed + 50) % 7000;
    throttlePosition = (throttlePosition + 5) % 100;
    engineTemperature = (engineTemperature + 1) % 120;
    fuelInjectionTiming = (fuelInjectionTiming + 1) % 100;
    ignitionTiming = (ignitionTiming + 1) % 30;

    // Simulate transmission data
    gearPosition = (gearPosition + 1) % 6;
    transmissionFluidTemperature = (transmissionFluidTemperature + 1) % 120;
    shiftCommands = (shiftCommands + 1) % 10;
    torqueConverterLockup = (torqueConverterLockup + 1) % 2;

    // Simulate body control data
    doorStatus = (doorStatus + 1) % 2;
    windowPosition = (windowPosition + 5) % 100;
    headlightsOn = (headlightsOn + 1) % 2;
    indicatorsOn = (indicatorsOn + 1) % 2;
    hornStatus = (hornStatus + 1) % 2;
    wiperStatus = (wiperStatus + 1) % 2;

    // Simulate ABS data
    wheelSpeed = (wheelSpeed + 5) % 200;
    brakePressure = (brakePressure + 1) % 100;
    absActive = (absActive + 1) % 2;
    tractionControlActive = (tractionControlActive + 1) % 2;

    // Simulate airbag data
    airbagDeployment = (airbagDeployment + 1) % 2;
    crashSensorsActive = (crashSensorsActive + 1) % 2;
    seatbeltTensionerActive = (seatbeltTensionerActive + 1) % 2;

    // Simulate infotainment data
    audioVolume = (audioVolume + 1) % 100;
    radioOn = (radioOn + 1) % 2;
    gpsOn = (gpsOn + 1) % 2;
    bluetoothConnected = (bluetoothConnected + 1) % 2;

    // Simulate climate control data
    cabinTemperature = (cabinTemperature + 1) % 40;
    fanSpeed = (fanSpeed + 1) % 5;
    acCompressorOn = (acCompressorOn + 1) % 2;
    airVentPosition = (airVentPosition + 1) % 4;

    // Simulate PCM data
    fuelLevel = (fuelLevel + 1) % 100;
    oxygenSensorReadings = (oxygenSensorReadings + 1) % 100;
    emissionControls = (emissionControls + 1) % 2;
    batteryVoltage = (batteryVoltage + 1) % 15;
    alternatorOutput = (alternatorOutput + 1) % 100;

    // Simulate steering data
    steeringAngle = (steeringAngle + 1) % 360;
    powerSteeringPressure = (powerSteeringPressure + 1) % 100;

    // Simulate TPMS data
    tirePressure = (tirePressure + 1) % 40;
    tireTemperature = (tireTemperature + 1) % 100;
}

void sendEngineData() {
    unsigned char ecmData[8] = {
        (unsigned char)engineSpeed,
        (unsigned char)throttlePosition,
        (unsigned char)engineTemperature,
        (unsigned char)fuelInjectionTiming,
        (unsigned char)ignitionTiming,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        ENGINE_FAULT_CODE
    };
    CAN.sendMsgBuf(0x400, 0, 8, ecmData);
    SERIAL.println("Engine Speed: " + String(engineSpeed));
    SERIAL.println("Throttle Position: " + String(throttlePosition));
    SERIAL.println("Engine Temperature: " + String(engineTemperature));
    SERIAL.println("Fuel Injection Timing: " + String(fuelInjectionTiming));
    SERIAL.println("Ignition Timing: " + String(ignitionTiming));
    SERIAL.println();
}

void sendTransmissionData() {
    unsigned char transmissionData[8] = {
        (unsigned char)gearPosition,
        (unsigned char)transmissionFluidTemperature,
        (unsigned char)shiftCommands,
        (unsigned char)torqueConverterLockup,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        TRANSMISSION_FAULT_CODE
    };
    CAN.sendMsgBuf(0x500, 0, 8, transmissionData);
    SERIAL.println("Gear Position: " + String(gearPosition));
    SERIAL.println("Transmission Fluid Temperature: " + String(transmissionFluidTemperature));
    SERIAL.println("Shift Commands: " + String(shiftCommands));
    SERIAL.println("Torque Converter Lockup: " + String(torqueConverterLockup));
    SERIAL.println();
}

void sendBodyControlData() {
    unsigned char bodyControlData[8] = {
        (unsigned char)doorStatus,
        (unsigned char)windowPosition,
        (unsigned char)headlightsOn,
        (unsigned char)indicatorsOn,
        (unsigned char)hornStatus,
        (unsigned char)wiperStatus,
        0x00, // Placeholder for future use
        BODY_CONTROL_FAULT_CODE
    };
    CAN.sendMsgBuf(0x600, 0, 8, bodyControlData);
    SERIAL.println("Door Status: " + String(doorStatus));
    SERIAL.println("Window Position: " + String(windowPosition));
    SERIAL.println("Headlights On: " + String(headlightsOn));
    SERIAL.println("Indicators On: " + String(indicatorsOn));
    SERIAL.println("Horn Status: " + String(hornStatus));
    SERIAL.println("Wiper Status: " + String(wiperStatus));
    SERIAL.println();
}

void sendAbsData() {
    unsigned char absData[8] = {
        (unsigned char)wheelSpeed,
        (unsigned char)brakePressure,
        (unsigned char)absActive,
        (unsigned char)tractionControlActive,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        ABS_FAULT_CODE
    };
    CAN.sendMsgBuf(0x700, 0, 8, absData);
    SERIAL.println("Wheel Speed: " + String(wheelSpeed));
    SERIAL.println("Brake Pressure: " + String(brakePressure));
    SERIAL.println("ABS Active: " + String(absActive));
    SERIAL.println("Traction Control Active: " + String(tractionControlActive));
    SERIAL.println();
}

void sendAirbagData() {
    unsigned char airbagData[8] = {
        (unsigned char)airbagDeployment,
        (unsigned char)crashSensorsActive,
        (unsigned char)seatbeltTensionerActive,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        AIRBAG_FAULT_CODE
    };
    CAN.sendMsgBuf(0x800, 0, 8, airbagData);
    SERIAL.println("Airbag Deployment: " + String(airbagDeployment));
    SERIAL.println("Crash Sensors Active: " + String(crashSensorsActive));
    SERIAL.println("Seatbelt Tensioner Active: " + String(seatbeltTensionerActive));
    SERIAL.println();
}

void sendInfotainmentData() {
    unsigned char infotainmentData[8] = {
        (unsigned char)audioVolume,
        (unsigned char)radioOn,
        (unsigned char)gpsOn,
        (unsigned char)bluetoothConnected,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        INFOTAINMENT_FAULT_CODE
    };
    CAN.sendMsgBuf(0x900, 0, 8, infotainmentData);
    SERIAL.println("Audio Volume: " + String(audioVolume));
    SERIAL.println("Radio On: " + String(radioOn));
    SERIAL.println("GPS On: " + String(gpsOn));
    SERIAL.println("Bluetooth Connected: " + String(bluetoothConnected));
    SERIAL.println();
}

void sendClimateControlData() {
    unsigned char climateControlData[8] = {
        (unsigned char)cabinTemperature,
        (unsigned char)fanSpeed,
        (unsigned char)acCompressorOn,
        (unsigned char)airVentPosition,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        CLIMATE_CONTROL_FAULT_CODE
    };
    CAN.sendMsgBuf(0xA00, 0, 8, climateControlData);
    SERIAL.println("Cabin Temperature: " + String(cabinTemperature));
    SERIAL.println("Fan Speed: " + String(fanSpeed));
    SERIAL.println("AC Compressor On: " + String(acCompressorOn));
    SERIAL.println("Air Vent Position: " + String(airVentPosition));
    SERIAL.println();
}

void sendPcmData() {
    unsigned char pcmData[8] = {
        (unsigned char)fuelLevel,
        (unsigned char)oxygenSensorReadings,
        (unsigned char)emissionControls,
        (unsigned char)batteryVoltage,
        (unsigned char)alternatorOutput,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        PCM_FAULT_CODE
    };
    CAN.sendMsgBuf(0xB00, 0, 8, pcmData);
    SERIAL.println("Fuel Level: " + String(fuelLevel));
    SERIAL.println("Oxygen Sensor Readings: " + String(oxygenSensorReadings));
    SERIAL.println("Emission Controls: " + String(emissionControls));
    SERIAL.println("Battery Voltage: " + String(batteryVoltage));
    SERIAL.println("Alternator Output: " + String(alternatorOutput));
    SERIAL.println();
}

void sendSteeringData() {
    unsigned char steeringData[8] = {
        (unsigned char)steeringAngle,
        (unsigned char)powerSteeringPressure,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        STEERING_FAULT_CODE
    };
    CAN.sendMsgBuf(0xC00, 0, 8, steeringData);
    SERIAL.println("Steering Angle: " + String(steeringAngle));
    SERIAL.println("Power Steering Pressure: " + String(powerSteeringPressure));
    SERIAL.println();
}

void sendTpmsData() {
    unsigned char tpmsData[8] = {
        (unsigned char)tirePressure,
        (unsigned char)tireTemperature,
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        0x00, // Placeholder for future use
        TPMS_FAULT_CODE
    };
    CAN.sendMsgBuf(0xD00, 0, 8, tpmsData);
    SERIAL.println("Tire Pressure: " + String(tirePressure));
    SERIAL.println("Tire Temperature: " + String(tireTemperature));
    SERIAL.println();
}
