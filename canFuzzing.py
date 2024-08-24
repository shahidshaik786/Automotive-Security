import can
import random
import time

# Create a CAN bus instance
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Fuzzing parameters
can_id_range = (0x000, 0x7FF)  # Standard CAN IDs
data_length = 8

try:
    while True:
        can_id = random.randint(*can_id_range)
        data = bytearray(random.getrandbits(8) for _ in range(data_length))
        message = can.Message(arbitration_id=can_id, data=data, is_extended_id=False)
        
        bus.send(message)
        print(f'Sent message: ID={hex(can_id)} Data={data.hex()}')
        
        time.sleep(0.1)  # Adjust as needed for your testing
except KeyboardInterrupt:
    print("Fuzzing stopped.")
