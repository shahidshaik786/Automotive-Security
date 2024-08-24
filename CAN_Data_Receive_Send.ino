#include <SPI.h>
#include "mcp2515_can.h"

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

// Timing variables
unsigned long previousMillis[10] = {0}; // Array to store previous millis for each data type
const unsigned long interval = 5000; // Interval of 2 seconds

const int SPI_CS_PIN = 9;
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin

bool sendData = false; // Flag to control data sending

// Global variables for simulated data
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

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

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

    if (CAN_MSGAVAIL == CAN.checkReceive()) {
        byte len;
        byte canMsg[8];
        CAN.readMsgBuf(&len, canMsg);
        String canData = "Data received - CAN ID: " + String(CAN.getCanId(), HEX) + " Data: ";
        for (int i = 0; i < len; i++) {
            canData += String(canMsg[i], HEX) + " ";
        }
        SERIAL.println(canData);
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
    transmissionFluidTemperature = (transmissionFluidTemperature + 1) % 150;
    shiftCommands = (shiftCommands + 1) % 5;
    torqueConverterLockup = (torqueConverterLockup + 1) % 2;

    // Simulate body control data
    doorStatus = (doorStatus + 1) % 2;
    windowPosition = (windowPosition + 1) % 4;
    headlightsOn = (headlightsOn + 1) % 2;
    indicatorsOn = (indicatorsOn + 1) % 2;
    hornStatus = (hornStatus + 1) % 2;
    wiperStatus = (wiperStatus + 1) % 2;

    // Simulate ABS data
    wheelSpeed = (wheelSpeed + 1) % 200;
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
    oxygenSensorReadings = (oxygenSensorReadings + 1) % 5;
    emissionControls = (emissionControls + 1) % 2;
    batteryVoltage = (batteryVoltage + 1) % 15;
    alternatorOutput = (alternatorOutput + 1) % 100;

    // Simulate steering data
    steeringAngle = (steeringAngle + 1) % 360;
    powerSteeringPressure = (powerSteeringPressure + 1) % 100;

    // Simulate tire pressure monitoring
    tirePressure = (tirePressure + 1) % 50;
    tireTemperature = (tireTemperature + 1) % 120;

    // Simulate ADAS data
    laneKeepAssist = (laneKeepAssist + 1) % 2;
    adaptiveCruiseControl = (adaptiveCruiseControl + 1) % 2;
    collisionDetection = (collisionDetection + 1) % 2;
    parkingAssist = (parkingAssist + 1) % 2;
}

void sendEngineData() {
    unsigned char ecmData[8] = {
        (unsigned char)engineSpeed,
        (unsigned char)throttlePosition,
        (unsigned char)engineTemperature,
        (unsigned char)fuelInjectionTiming,
        (unsigned char)ignitionTiming,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)ENGINE_FAULT_CODE
    };
    CAN.sendMsgBuf(0x400, 0, 8, ecmData);
    SERIAL.print("Engine Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(ecmData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}

void sendTransmissionData() {
    unsigned char transmissionData[8] = {
        (unsigned char)gearPosition,
        (unsigned char)transmissionFluidTemperature,
        (unsigned char)shiftCommands,
        (unsigned char)torqueConverterLockup,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)TRANSMISSION_FAULT_CODE
    };
    CAN.sendMsgBuf(0x401, 0, 8, transmissionData);
    SERIAL.print("Transmission Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(transmissionData[i], HEX);
        SERIAL.print(" ");
    }
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
        (unsigned char)0x00, // Placeholder
        (unsigned char)BODY_CONTROL_FAULT_CODE
    };
    CAN.sendMsgBuf(0x402, 0, 8, bodyControlData);
    SERIAL.print("Body Control Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(bodyControlData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}

void sendAbsData() {
    unsigned char absData[8] = {
        (unsigned char)wheelSpeed,
        (unsigned char)brakePressure,
        (unsigned char)absActive,
        (unsigned char)tractionControlActive,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)ABS_FAULT_CODE
    };
    CAN.sendMsgBuf(0x403, 0, 8, absData);
    SERIAL.print("ABS Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(absData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}

void sendAirbagData() {
    unsigned char airbagData[8] = {
        (unsigned char)airbagDeployment,
        (unsigned char)crashSensorsActive,
        (unsigned char)seatbeltTensionerActive,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)AIRBAG_FAULT_CODE
    };
    CAN.sendMsgBuf(0x404, 0, 8, airbagData);
    SERIAL.print("Airbag Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(airbagData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}

void sendInfotainmentData() {
    unsigned char infotainmentData[8] = {
        (unsigned char)audioVolume,
        (unsigned char)radioOn,
        (unsigned char)gpsOn,
        (unsigned char)bluetoothConnected,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)INFOTAINMENT_FAULT_CODE
    };
    CAN.sendMsgBuf(0x405, 0, 8, infotainmentData);
    SERIAL.print("Infotainment Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(infotainmentData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}

void sendClimateControlData() {
    unsigned char climateControlData[8] = {
        (unsigned char)cabinTemperature,
        (unsigned char)fanSpeed,
        (unsigned char)acCompressorOn,
        (unsigned char)airVentPosition,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)CLIMATE_CONTROL_FAULT_CODE
    };
    CAN.sendMsgBuf(0x406, 0, 8, climateControlData);
    SERIAL.print("Climate Control Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(climateControlData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}

void sendPcmData() {
    unsigned char pcmData[8] = {
        (unsigned char)fuelLevel,
        (unsigned char)oxygenSensorReadings,
        (unsigned char)emissionControls,
        (unsigned char)batteryVoltage,
        (unsigned char)alternatorOutput,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)PCM_FAULT_CODE
    };
    CAN.sendMsgBuf(0x407, 0, 8, pcmData);
    SERIAL.print("PCM Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(pcmData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}

void sendSteeringData() {
    unsigned char steeringData[8] = {
        (unsigned char)steeringAngle,
        (unsigned char)powerSteeringPressure,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)STEERING_FAULT_CODE
    };
    CAN.sendMsgBuf(0x408, 0, 8, steeringData);
    SERIAL.print("Steering Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(steeringData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}

void sendTpmsData() {
    unsigned char tpmsData[8] = {
        (unsigned char)tirePressure,
        (unsigned char)tireTemperature,
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)0x00, // Placeholder
        (unsigned char)TPMS_FAULT_CODE
    };
    CAN.sendMsgBuf(0x409, 0, 8, tpmsData);
    SERIAL.print("TPMS Data Sent: ");
    for (int i = 0; i < 8; i++) {
        SERIAL.print(tpmsData[i], HEX);
        SERIAL.print(" ");
    }
    SERIAL.println();
}
