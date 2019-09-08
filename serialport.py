#!/usr/bin/python
# -*- coding: cp1252 -*-
 
from datetime import datetime
import serial
import time
 
#Define a porta e o baudrate usado
#No Windows a porta e alguma das COMs
porta = '/dev/ttyACM0'
baud_rate = 115200
 
opcao = raw_input("Verificar GPS? s / n")
 
if opcao == 's':
 
    #Tempo de intervalo entre cada leitura
    tempo = int (raw_input("Informe o intervalo entre cada leitura: "))
 
    #Cria o objeto_porta, limpa o buffer da serial,
    objeto_porta = serial.Serial(porta, baud_rate)
    objeto_porta.flushInput();
 
    #Abre o arquivo para escrever os dados
    arquivo = open("LOG.txt", "a+")
 
    #Fica em LOOP Infinito, para sair pressione
    #Ctrl + c
    while True:
 
        #Cria o objeto_hoje, pega os valores de
        #dia, mes, ano e horario, converte para string

 
        # Le os valores passados pelo arduino
        GPS = objeto_porta.readline()
 
        #Escreve os dados de HORA, DATA e TEMPERATURA no arquivo

        arquivo.write(GPS)

        print " ", GPS
 
        #Faz uma pausa de X (tempo informado) segundos para fazer o loop novamente
        time.sleep(tempo)
 
    #Fecha o arquivo e a porta serial
    arquivo.close()
    objeto_porta.close()
 
else:
 
        print "Saindo..."
