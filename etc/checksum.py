def cal_checksum(packet):
    if(packet[0] == 0xFF and packet[1] == 0xFF):
        packet = packet[2:]

    # Check the number of packet
    if(len(packet) != packet[1] + 1):
        if(len(packet) == packet[1] + 2):
            raise Exception('you have to check packet! \nI think your purpose is to check checksum. but this is to calculate checksum.')
        else:
            raise Exception('you have to check packet!')

    sum = 0
    for i in packet:
        sum += i

    checksum = hex(0xFF - sum % 0xFF)
    print(checksum)


def check_checksum(packet):
    if(packet[0] == 0xFF and packet[1] == 0xFF):
        packet = packet[2:]

    # Check the number of packet
    if(len(packet) != packet[1] + 2): 
        if(len(packet) == packet[1] + 1):
            raise Exception('you have to check packet! \nI think your purpose is to calculate checksum. but this is to check checksum.')
        else:
            raise Exception('you have to check packet!')

    sum = 0
    for i in packet:
        sum += i

    if(sum % 0xFF == 0):
        print("checksum is correct!")
    else:
        print("have to check checksum")
    

# [0x11] [0x14] -> [0x11,0x14]
def parser(packet_str):
    packet_str = packet_str.strip()
    chunk = packet_str.split(' ')
    packet_list = []
    for e in chunk:
        packet_list.append(int(e[1:-1], 16))
    #for testing
    #print(packet_list)
    return packet_list


if __name__ == "__main__":
    packet_str = '[0xFF] [0xFF] [0x02] [0x05] [0x03] [0x1E] [0xd0] [0x07] [0xFF]'
    packet = parser(packet_str)
    #cal_checksum(packet)
    check_checksum(packet)


    #packet = [0x00, 0x02, 0x06, 0xF7]
    #check_checksum(packet)