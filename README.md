# IERG4230_Mini_Project - Smart Mailbox 

> ### Target Users: Postman, Hong Kong Citizens
> ### Target Place: public and pirvate housing estate, Hong Kong Post
> ### ***Purpose:
> ### 1.Improve efficiency of mailing procdure
> ### 2.Reduce mail checking time, Improve user experience
> ### 3.Reduce the risk of mails being stolen

```
## Uplink 
Inputs: 
Sensors: - ^Sound source Beeper (Buzzer Digital Output)
         - ^Switches (Dual tact switcher)
         - OLED Display (SSD1306)
         - RFID Reader (MFRC522)
         - Touch Switch (TIP223)
         
Components: - Arduino 
            - E

Processor: Locker and Smart Watch with RFID Tag  

Network: - Protocal: 
         - Architecture:
         - Topology:
```

```
## Processing
Data: - Source: Infomations(e.g. address) can be accessed by the RFID reader, return address, the letter delivered time, the letter taken time
      - Time: Real Time
      - Storage: SQLserver 
      - Privacy: Data Encryption 
      
Information: 

Assumption{
    Pre-procedure: 
    >step1: Enter the letter destination and other informations(e.g.return address) by smartphone Hong Kong Post App
    >step2: Receive RFID stamps in Hong Kong Post Workstation
    >step3: Stick the RFID stamp on the letter and finish the posting

    Procedure:
    >step1: The letter can filter the letters in difference regions by RFID
    >step2: Particular region of the letters can be delivered by the postman
}

>step3: When unmatched address letter is delievered into a mailbox -> the OLED display shows "Unmatched mailbiox" with red light
>step4: The message of "letter is delievered" and the numbner of mails in the mailbox can be received by the citizens
>step5: Green light with the number of mail in the mailbox will be shown in the OLED display
>step6: Citizens can access their mailbox by their resident card.
>step7: If right card {display"access success"} else {display "access denied"}
>step8: After the citizens taken the mails, long touch the touch switch to finish the process ->reset the number of mails in the mailbox. 
>step9: The mailbiox display 0 number of mail in the mailbox.

```

```
## Downlink

Network: Same as Uplink

Output:

Processor:
```



## Reference
* *
* *
* *
