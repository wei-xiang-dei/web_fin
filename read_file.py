import serial

a=0

f = open('xyz_sample.txt', 'w')
class SerialArduino:
    def __init__(self):
        get_inputbtn = int(input('input 0 to start: '))
        if get_inputbtn == 0:
            ser = serial.Serial('COM5',  baudrate=19200)
        while True and get_inputbtn == 0:

            line = ser.readline()
            self.check_inf(line)

        ser.close()
    def check_inf(self, input_inf):
        global a
        if input_inf != b'':
            print(input_inf)
            if(a!=0):
                get_cm = str(input_inf,'utf-8')
                print('input_inf is' ,get_cm,'\n')
            # print('input_inf: ', get_cm)
            
            if(a!=0):
                f.writelines(get_cm+"\n")
            else:
                a+=1


if __name__ == '__main__':
    try:
        SerialArduino()
    except KeyboardInterrupt:
        print("Shutting down")
        f.close()
