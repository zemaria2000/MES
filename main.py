import json
import socket
from bs4 import BeautifulSoup
from time import sleep
from colorama import Fore, Style, init
import numpy.random as random


# List of variables that are monitored by MES

VARIABLES = {"Blr_Gas_Pres_Max", 
             "Blr_Gas_Pres_Min",
             "Blr_Supply",
             "Bnr_Gas_Pres_Max",
             "Bnr_Gas_Pres_Min",
             "CH_Flow_NTC_Temp_Rise",
             "CH_Flow_Press",
             "CH_Flow_Rate",
             "CO_Max",
             "CO_Min",
             "CO2_Max",
             "CO2_Min",
             "Code_Plug",
             "Delta_CH_Flow_Temp_Cold",
             "Delta_CH_Flow_Temp_Hot",
             "Gas_Flow_Max",
             "Gas_Flow_Min",
             "Humidity",
             "Pressure",
             "Temperature"}


# =========================================================
# .......................MAIN FUNCTION.....................
# =========================================================
def main():
    # Get event type and the name of the XML file template to use
    event_name, xml_template_name = c['Event'], j['XMLtemplate']

    print(xml_template_name)

    # Read the XML template
    xml_template = extract_xml_template(xml_template_name)

    # Fill XML fields from configuration file
    xml_filled = populate_xml_constants(j, xml_template, event_name)

    while True:
        # Fill variable XML fields
        xml_filled = populate_xml_variables(xml_filled, VARIABLES)
        # print(xml_filled)

        # Add 4 header bytes and create telegram to send
        telegram = outward_msg(xml_filled)
        print('TELEGRAM!')
        print(telegram)

        # Send telegram via TCP/IP
        send_routine(c, telegram)

# =========================================================
# .........................FUNCTIONS.......................
# =========================================================


# ==================================================================
# FUNCTION THAT READS CONFIG FILE FOR RESPECTIVE PARSED EVENT IN CMD
# ==================================================================
def initial_config():
    init() # Initialise Colorama (just for appearance purposes)

# Reads config.json file according to event
    with open('./config.json', 'r') as f: # opens and reads the json file
        c = json.load(f) # saves the dictionary as j

    with open('./config_' + c['Event'] + '.json', 'r') as f2: # opens and reads the json file
        j = json.load(f2) # saves the dictionary as j

    print("\nINITIALIZING...\n\n" + "EVENT: " + Fore.GREEN + j['header']['eventName'] + Style.RESET_ALL + "\n")
    
    return c, j


# ==================================================================
# FUNCTION THAT READS XML FILE
# ==================================================================
def extract_xml_template(xml):
    xml_path = './xml/' + xml

    with open(xml_path, 'r', encoding="utf8") as f:
        xml_data = f.read()
    
    return xml_data


# ==================================================================
# FUNCTION THAT READS config.json AND ADDS INFO TO XML FILE
# ==================================================================
def populate_xml_constants(j, xml, event):
    bs_xml = BeautifulSoup(xml, 'xml')

    # ---------------------- <header> ------------------------------
    # Change <header> xml values to config.json values
    for key in j['header']:
        if key == 'location':
            continue
        for tag in bs_xml.find_all('header', {'': ''}):
            tag[key] = j['header'][key] # Change xml value to config.json value

    # ---------------------- <location> ------------------------------
    # Change <location> xml values to config.json values
    for key in j['header']['location']:
        for tag in bs_xml.find_all('location', {'': ''}):
            # print(key)
            # print(tag[key])
            tag[key] = j['header']['location'][key] # Change xml value to config.json value

    # ---------------------- <event> ------------------------------
    # print(bs_xml.event.partProcessed['identifier'])
    if event == 'partProcessed':
        bs_xml.event.partProcessed['identifier'] = j['event']['identifier']

    # convert bs_xml_data back to string type after altering values
    xml_data = str(bs_xml.prettify())
    
    return xml_data


# ==================================================================
# FUNCTION THAT READS config.json AND ADDS INFO TO XML FILE
# ==================================================================

def populate_xml_variables(xml, VARIABLES):
    bs_xml = BeautifulSoup(xml, 'xml')
    
    # ---------------------- <body> ------------------------------
    # Find Temperature item
    opc_vars = generate_env_vars(VARIABLES)
    
    i = 0
    for var in VARIABLES:
        for tag in bs_xml.find_all('item', {'name': var}):
            tag['value'] = opc_vars[i]
        i += 1
    
    # convert bs_xml_data back to string type after altering values
    xml_data = str(bs_xml.prettify())

    return xml_data


# ==================================================================
# FUNCTION THAT CREATES THE MESSAGE TO SEND TO MES
# ==================================================================
def outward_msg(xml_data):
# ____________ PROCESS XML _________________
    xml_size_og = len(xml_data) # original size
    xml_size = xml_size_og + 4 # add 4 bytes (MES needs these to know xml size)
    print('\nSize␣=␣' + str(xml_size_og))

    # Creating 4 header bytes
    lolo = (xml_size & 0xff)
    hilo = ((xml_size >> 8) & 0xff)
    lohi = ((xml_size >> 16) & 0xff)
    hihi = (xml_size >> 24)

    # Array of decimals
    telegram_bytes = [hihi, lohi, hilo, lolo] # save values in array
    print("\n")
    print('Telegram bytes []')
    print(telegram_bytes)

    # Array of bytes to be sent to MES
    telegram_bytes = bytearray(telegram_bytes) # convert normal array (decimals) into an array of bytes
    print("\n")
    print('Telegram bytes in an array')
    print(telegram_bytes)

    # Cyclically read character from xml, convert to decimal, append to array of bytes
    for i in range(0, xml_size_og):
        char_to_decimal = ord(xml_data[i])
        telegram_bytes.append(char_to_decimal)
    print("\n")
    print('Sent:␣' + str(telegram_bytes) + '\n\n')

    return telegram_bytes


# ==================================================================
# FUNCTION THAT ESTABLISHES TCP/IP CONNECTION, SENDS DATA, CLOSES CONNECTION
# ==================================================================
def send_routine(config, telegram):
    host, port, interval = config['Host'], config['Port'], config['Interval']

    tcp_socket = socket.create_connection((host, port))

    # ____________ SEND XML TO SERVER _________________
    try:
        tcp_socket.sendall(telegram)

    finally:
        print('\n\nSENT ' + Fore.GREEN + config['Event'] + Style.RESET_ALL + '␣XML␣to␣' + Fore.CYAN + host + Style.RESET_ALL + "\n\n")
        print("Closing␣socket")
        print("==================================================================\n")
        tcp_socket.close()
        sleep(interval)

        # print(f"Sent to {config['Host']}\n{telegram}\n\n")




# ==================================================================
# FUNCTIONS THAT GENERATE RANDOM VALUES
# ==================================================================

def generate_new_value():

    x = random.uniform()
    if x < 0.99:
        val = round(float(230 + 5*random.normal(0,1)), 2)
    else:
        val = float(0)
    return (val)

def generate_env_vars(VARIABLES):
    
    env_vars = []

    for i in range(len(VARIABLES)):
        val = generate_new_value()
        env_vars.append(val)

    return env_vars



if __name__ == '__main__':
    # Read config file
    c, j = initial_config()
    main()


