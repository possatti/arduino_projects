
#!/usr/bin/python
import json
import serial
import socket
import datetime

from influxdb import InfluxDBClient

INFLUXDB_HOST = 'localhost'
INFLUXDB_PORT = 8086
INFLUXDB_DATABASE_NAME = 'dht'
SERIAL_PORT = '/dev/ttyACM0'
SERIAL_BAUDRATE = 9600
SERIAL_TIMEOUT = 2

def check_database_exists(client, database_name):
    for database in client.get_list_database():
        if ('name' in database) and database['name'] == database_name:
            return True
    return False

def prepare_points(reading):
    points = [
        {
            "measurement": "reading",
            "tags": {
                "host": socket.gethostname(),
            },
            "time": datetime.datetime.utcnow().isoformat(),
            "fields": reading
        }
    ]
    return points


def main():
    # Setup InfluxDB.
    client = InfluxDBClient(host=INFLUXDB_HOST, port=INFLUXDB_PORT)
    if not check_database_exists(client, INFLUXDB_DATABASE_NAME):
        client.create_database(INFLUXDB_DATABASE_NAME)
    client.switch_database(INFLUXDB_DATABASE_NAME)

    # Setup serial port.
    ard = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=SERIAL_TIMEOUT)

    while True:
        line = ard.readline()
        print(line.decode('ascii'), end='')
        try:
            reading = json.loads(line)
            points = prepare_points(reading)
            print(points)
            client.write_points(points)
        except json.JSONDecodeError:
            pass

if __name__ == "__main__":
    main()
