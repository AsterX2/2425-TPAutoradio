# 2425-TPAutoradio-notes vincent

 TP de Synthèse – Autoradio ESE







écrire sur la broche GPIO pour allumer la LED : (les LEDs sont généralement connectées en logique inversée, donc un 0 allume la LED).



The system host can **enable** the I/Os as either **inputs or outputs** by writing the I/O **configuration bits ()(IODIRA/B).**

The **data** for each input or output is kept in the corresponding **input or output register.** 

Les registres de configuration et de contrôle sont sur 8 bits 

![image-20241209233122326](/home/vincent/Documents/ese_3a/autoradio/2425-TPAutoradio/assets/image-20241209233122326.png)





 All registers can be read by the system host

The 16-bit I/O port functionally consists of **two 8-bit ports (PORTA and PORTB).**  



can operate in the 8-bit or 16-bit modes via **IOCON.BANK**



Il y a 11 pair de registres  accessibles en tout sur un port (A ou B). Comme on a 2 ports, il y a 22 registres individuels. 

La disposition, l'assignation des adresses des chaque registre varie avec deux plan d'adressage possible. Le choix du plan d'adressage sur lequel le composant va fonctionner est définis par le bit IOCONBANK.

****

explication du chat 



Le bit **BANK** dans le registre IOCON contrôle la disposition des registres internes du microcontrôleur. En modifiant ce bit, vous pouvez changer la manière dont les adresses des registres sont mappées, ce qui affecte la façon dont vous accédez aux différents registres du périphérique.

### Valeurs possibles

- **BANK = 1** : Lorsque ce bit est défini à 1, les registres associés à chaque port (PORTA et PORTB) sont séparés en différentes banques. 
  - Les registres associés à PORTA sont mappés de l'adresse 00h à 0Ah, et les registres associés à PORTB sont mappés de l'adresse 10h à 1Ah.
- **BANK = 0** : Lorsque ce bit est défini à 0, les registres A et B sont appariés. 
  - Par exemple, le registre IODIRA est mappé à l'adresse 00h et le registre IODIRB est mappé à l'adresse suivante, 01h. Les adresses des registres vont de 00h à 15h.

****

Pour l'adressage des 22 registres détaillés voir le tableau ci-dessous

![image-20241209231327921](/home/vincent/Documents/ese_3a/autoradio/2425-TPAutoradio/assets/image-20241209231327921.png)

Pour la suite du tp, on partira avers un adressage pour BANK=0

***



par port (GPA et GPB)



***

- **MCP23S17_IODIRA** : Adresse du registre de direction  des ports GPIOA (0x00). Ce registre détermine si les broches GPIOA sont  des entrées ou des sorties.
- **MCP23S17_IODIRB** : Adresse du registre de direction  des ports GPIOB (0x01). Ce registre détermine si les broches GPIOB sont  des entrées ou des sorties.
- **MCP23S17_OLATA** : Adresse du registre de sortie  latch pour le port GPIOA (0x14). Écrire dans ce registre met à jour  l'état des sorties sur le port GPIOA.
- **MCP23S17_OLATB** : Adresse du registre de sortie  latch pour le port GPIOB (0x15). Écrire dans ce registre met à jour  l'état des sorties sur le port GPIOB.



On veut configurer en output tous nos gpio car on veut allumer les leds : le registre à manipuler est **IODIRA** et **IODIRB**





Pour écrire et lire les registres en SPI il faut former un octet appelé CONTROL BYTE qui est constitué des champs suivants d'après la documentation :

![image-20241209234743876](/home/vincent/Documents/ese_3a/autoradio/2425-TPAutoradio/assets/image-20241209234743876.png)

-un champ de 4 bits qui contient le mot binaire '`0100`' 

-un champ de 3 bits qui contient les bits A2 A1 A0 en l'occurence '`000`' comme ces pins sont connects à la masse.

-un champs de 1 bits qui contient définit la transmission comme une lecture ou une écriture



Voici le control byte formé : 

```c
#define MCP_CONTROL_BYTE(adress, RW)\
		((0b0100 << 4) | (adress & 0b111 << 1) | RW)
```

**Préfixe (`0b0100 << 4`)** :

- `0b0100` est un préfixe spécifique au composant MCP23S17 
- `<< 4` déplace ce préfixe de 4 bits vers la gauche, ce qui donne `0b01000000`.
- Cela correspond au préfixe de l'instruction de lecture/écriture pour le MCP23S17.

**Adresse Matérielle (`(address & 0b111 << 1)`)** :

- `address` est l'adresse matérielle du MCP23S17, déterminée par les broches A0, A1, et A2.
- `& 0b111` masque l'adresse pour ne garder que les 3 bits de poids faible (les bits les moins significatifs).
- `<< 1` déplace le résultat de 1 bit vers la gauche pour le placer correctement dans le byte de contrôle.

![image-20241210004611354](/home/vincent/Documents/ese_3a/autoradio/2425-TPAutoradio/assets/image-20241210004611354.png)



Ensuite, il faut transmettre les 8 bits d'adresse du registre à manipuler suivis de 8 bits de données.



Peu importe l'opération, lecture ou écriture, il faut mettre à 0 la broche CS.

La broche nCS est câblée sur le pin 7 du port B de la stm32 L476 (PB7)

![Capture d’écran du 2024-12-10 08-44-38](./assets/Capture d’écran du 2024-12-10 08-44-38.png)

```c
// Assert chip select
	HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, GPIO_PIN_RESET);
```

En fin de transmission il faut remettre à 1 la broche CS.

```c
// Deassert chip select
	HAL_GPIO_WritePin(VU_nCS_GPIO_Port, VU_nCS_Pin, GPIO_PIN_SET);
```

