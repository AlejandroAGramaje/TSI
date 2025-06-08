import serial
import csv

puerto = "COM4"  # Cambia esto al puerto que te salga en Arduino IDE
archivo = "zanahoria.csv"
num_muestras = 400

with serial.Serial(puerto, 9600, timeout=1) as arduino, open(archivo, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["Red", "Green", "Blue"])
    count = 0

    while count < num_muestras:
        linea = arduino.readline().decode("utf-8").strip()
        if "," in linea:
            partes = linea.split(",")
            if len(partes) == 3:
                writer.writerow(partes)
                count += 1
                print(f"Muestra {count}: {linea}")
print("Captura completada.")
