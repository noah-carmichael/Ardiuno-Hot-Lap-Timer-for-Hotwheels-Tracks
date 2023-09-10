import serial
import shutil
import os
import tempfile

arduino_port = "COM4" # serial port of Arduino, change if necessary
baud = 9600 # arduino uno runs at 9600 baud
title = input("Enter file name to save laptimes and average: ")
f = open(f"{title}.txt", "x", encoding="utf-8")
file_path = f"{title}.txt"

ser = serial.Serial(arduino_port, baud)

def parse_time(time_str):
    hours, minutes, seconds, milliseconds = map(int, time_str.split(':'))
    total_milliseconds = (hours * 3600 + minutes * 60 + seconds) * 1000 + milliseconds
    return total_milliseconds

def format_time(milliseconds):
    seconds, milliseconds = divmod(milliseconds, 1000)
    minutes, seconds = divmod(seconds, 60)
    hours, minutes = divmod(minutes, 60)
    return f"{hours:02d}:{minutes:02d}:{seconds:02d}:{milliseconds:03d}"

def calculate_average_time(time_strings):
    total_milliseconds = sum(parse_time(time_str) for time_str in time_strings)
    average_milliseconds = total_milliseconds / len(time_strings)
    return format_time(int(average_milliseconds))

with open(file_path, 'w') as f:
    f.write('temp_line')



lap_average = []
number_of_laps = 0
file1 = open(file_path, "a")  # append mode
file1.write("\n\nLaps:\n")
while True:
    getData=ser.readline()
    dataString = getData.decode('utf-8')
    data=dataString[0:][:-2]
    print(data)

    # check for button reset:
    if data == "RESET":
        break

    # append laptimes
    file1 = open(file_path, "a")
    file1.write(f"{data} \n")
    file1.close()
    lap_average.append(data)
    average_time = calculate_average_time(lap_average)
    print("Average Time:", average_time)
    number_of_laps += 1


    with open(file_path, 'r') as file:
        file_content = file.readlines()

    file_content[0] = "Number of laps: " + str(number_of_laps) + "; Average laptime: " + average_time + '\n'

    with open(file_path, 'w') as file:
        file.writelines(file_content)