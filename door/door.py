import time
from mfrc522 import SimpleMFRC522
import RPi.GPIO as GPIO
import os
from datetime import datetime

Pin_Step = 6    #pin do step do driver do stepper
Pin_Dir = 19    #pin da direção do stepper
Pin_Enable = 13    #pin do enable do driver do stepper
step_delay = 0.001
steps = 200
Pin_IN1 = 21    #luz
Pin_IN2 = 17    #stepper
Flag = 1 

 
# setup
GPIO.setmode( GPIO.BCM )
GPIO.setup( Pin_Enable, GPIO.OUT )
GPIO.setup( Pin_Dir, GPIO.OUT )
GPIO.setup( Pin_Step, GPIO.OUT )
GPIO.setup( Pin_IN1, GPIO.OUT )
GPIO.setup( Pin_IN2, GPIO.OUT )
 
# inicialização
GPIO.output( Pin_Enable, GPIO.HIGH )
GPIO.output( Pin_Dir, GPIO.LOW )
GPIO.output( Pin_Step, GPIO.LOW )
GPIO.output( Pin_IN1, GPIO.LOW )
GPIO.output( Pin_IN2, GPIO.LOW )
 
 
def cleanup():
    GPIO.output( Pin_Enable, GPIO.LOW )
    GPIO.output( Pin_Dir, GPIO.LOW )
    GPIO.cleanup()





 
 
#roda o stepper
def rotate(Flag):
    try:
        GPIO.output( Pin_IN1, GPIO.HIGH  )
        GPIO.output( Pin_IN2, GPIO.HIGH )
        GPIO.output( Pin_Enable, GPIO.LOW )

        if(Flag == 1):
            GPIO.output(Pin_Dir, GPIO.LOW)  # Define a direção do motor
            for _ in range(steps):
                GPIO.output(Pin_Step, GPIO.HIGH)  # Sinal de passo HIGH
                time.sleep(step_delay)           # Delay entre os passos
                GPIO.output(Pin_Step, GPIO.LOW)   # Sinal de passo LOW
                time.sleep(step_delay)           # Delay para completar o ciclo
            Flag = 0
            print(Flag)
                
        else:
            GPIO.output(Pin_Dir, GPIO.HIGH)  # Define a direção do motor
            for _ in range(steps):
                GPIO.output(Pin_Step, GPIO.HIGH)  # Sinal de passo HIGH
                time.sleep(step_delay)           # Delay entre os passos
                GPIO.output(Pin_Step, GPIO.LOW)   # Sinal de passo LOW
                time.sleep(step_delay)           # Delay para completar o ciclo
                Flag = 1

        GPIO.output( Pin_Enable, GPIO.HIGH )
        GPIO.output( Pin_IN2, GPIO.LOW)
        time.sleep(2)
        GPIO.output( Pin_IN1, GPIO.LOW)

        return Flag
    except KeyboardInterrupt:
        cleanup()
        exit(1)



#le os ids do arquivo
def ler_lista_numeros(nome_arquivo):
    numeros = []
    try:
        with open(nome_arquivo,'r') as arquivo:
            for linha in arquivo:
                linha_limpa = linha.strip()
                if linha_limpa:  
                    numeros.append(int(linha_limpa))
            return numeros
    except FileNotFoundError:
        print(f"O arquivo '{nome_arquivo}' não foi encontrado.")
        return []
    
#escreve os novos ids no arquivo     
def escrever_numeros(nome_arquivo):
    try:
        with open(nome_arquivo,'a') as arquivo:
            print("introduza o novo cartao")
            time.sleep(0.5)
            id, _ = reader.read()
            print('ID= ', id)
            numero_procurado = str(id)
            arquivo.write(numero_procurado + '\n')

    except FileNotFoundError:
        print(f"O arquivo '{nome_arquivo}' não foi encontrado.")



def reboot_pi():
        
    print("rebooting")
    os.system("sudo reboot")  

     

# Define os nomes dos arquivo
nome_arquivo='numero.txt'
save_cartoes = 346147891815
save_cartoes2 = 3382435468
reboot_time = "00:00"

# Inicializa o leitor RFID
reader=SimpleMFRC522()


try:
    while True:
        # Le os ids dos arquivos
        numero = ler_lista_numeros(nome_arquivo)
        print("introduza o cartao")
        id, _ = reader.read()
        print('ID= ', id)
        numero_procurado = id
        print(id)

        Real_time = datetime.now().strftime("%H:%M")

        if Real_time == reboot_time:
            reboot_pi()
        
        if numero_procurado in numero:
            print("o numero esta na lista")
            Flag = rotate(Flag)

        if (numero_procurado == save_cartoes or numero_procurado == save_cartoes2):
            time.sleep(1)
            GPIO.output( Pin_IN1, GPIO.HIGH  )
            print("introduza novo cartao")
            escrever_numeros(nome_arquivo)
            GPIO.output( Pin_IN1, GPIO.LOW)
        time.sleep(1)

 
except KeyboardInterrupt:
    GPIO.cleanup()

