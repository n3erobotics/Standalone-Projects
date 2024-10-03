import os
import time
from datetime import datetime

def reiniciar_em_horario(reboot_time):
    while True:
        
        Real_time = datetime.now().strftime("%H:%M")

        if Real_time == reboot_time:
            print("rebooting")
            os.system("sudo reboot")
            break  


reboot_time = "10:25"
reiniciar_em_horario(reboot_time)
