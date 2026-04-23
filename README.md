# Práctica 1 - Mosquitto MQTT con Arduino UNO Q y ESP32 
## Descripción 
En esta práctica se instaló y configuró Mosquitto MQTT en una tarjeta Arduino UNO Q con Linux, funcionando como broker MQTT. Posteriormente, se programó una ESP32 como publisher para enviar mensajes periódicos a un tópico MQTT. 
## Objetivo 
Comprender el funcionamiento del protocolo MQTT y la integración entre dispositivos embebidos y un servidor ligero de mensajería. 
## Material utilizado 
* Arduino UNO Q con Linux 
* ESP32
* Red WiFi local
* Arduino IDE
* Librería PubSubClient
* Mosquitto y mosquitto-clients
  
## Instalación del broker en Arduino UNO Q

Ejecutamos los siguientes comandos para actualizar dependencias y descargar las herramientas [Ejemplo](https://github.com/user-attachments/assets/541081e4-81d0-4c77-b159-4a37b0b95154)

```bash
sudo apt update
sudo apt install -y mosquitto mosquitto-clients
```
Luego ejecutamos los siguientes comandos para habilitar y ejecutar el servidor [Ejemplo](https://github.com/user-attachments/assets/7e3a20d6-7c09-4007-a5aa-b0ad74d79d8f)
```bash
sudo systemctl enable mosquitto
sudo systemctl start mosquitto
sudo systemctl status mosquitto
```
## Prueba local del broker 
Para lanzar esta prueba necesitaremos abrir 2 terminales de nuestra arduino Uno Q, para que desde una consola este esuchando (Suscriber) y desde la otra consola mandemos los mensajes (Publisher), todo esto localmente. [Ejemplo](https://github.com/user-attachments/assets/3d9aa91d-8ff1-4caf-acb9-805077cb4625) 
### Subscriber:

```bash
mosquitto_sub -h localhost -t prueba/mensaje
```
### Publisher:

```bash
mosquitto_pub -h localhost -t prueba/mensaje -m "Hola desde Arduino UNO Q"
```

Una vez que se tenga el Suscriber listo y mandemos el mensaje desde el Publisher, podremos observar el mensaje enviado. De esta manera sabemos que nuestro servidor MQTT esta funcionando de manera correcta localmente. 
## Habilitar broker para conexiones remotas 
Hasta este punto tenemos nuestro servidor funcionando para conexiones locales del mismo dispositivo, pero que pasa si queremos recibir una conexion desde otro dispositivo, es aqui donde tenemos que hacer una modificacion en la configuracion del servidor, para que este admita conexiones remotas. 
### Configuración
Debemos ingresar a el archivo [Ejemplo](https://github.com/user-attachments/assets/4de678cf-d7a3-4cf4-bf1a-6ae0fbb0ea6f)


```bash
/etc/mosquitto/conf.d/default.conf
```
Agregar la siguiente configuración [Ejemplo](https://github.com/user-attachments/assets/81e09657-a8b8-40c8-9cab-c34d42df932e)

```bash
listener 1883
allow_anonymous true
```

Como hemos modificado la configuración del servidor, debemos reiniciar el servidor para poder ver reflejados los cambios, y eso lo hacemos con el siguiente comando [Ejemplo](https://github.com/user-attachments/assets/ff571f63-1525-4cf0-92d1-e87e31a5ca11)

```bash
sudo systemctl restart mosquitto
```

### IP del servidor

Para que nuestro servidor reciba los mensajes debemos colocar la IP del servidor MQTT en los clientes (Publisher), es por eso que para poder obetener la direccion IP podemos usar los comandos

### Linux

```bash
hostname -I
```
### Windows
```bash
ipconfig
```
## Prerequisitos Arduino IDE
Se tiene que tener descargada la libreria **PubSubClient** dentro del IDE de arduino, esta se puede buscar en la opcion de libererias y ahi se busca esta libreria, tal como se muestra en la imagen
<img width="975" height="548" alt="image" src="https://github.com/user-attachments/assets/daae40b0-a995-4a69-b24c-6189dcd78927" />

## Codigo Publisher 
El codigo del [publisher.ino](https://github.com/Champagnepagcois/Mosquitto-MQTT/blob/main/broker.ino) se tienen que sustituir 3 variables, la primera es el nombre de tu red, la contraseña de tu red y por ultimo la dirección IP de tu servidor MQTT (Broker) [Ejemplo](https://github.com/user-attachments/assets/9ab8b69a-ec69-414d-9f98-fbba59ab0615)

### Ejemplo nombre de red

```bash
const char* ssid = "INFINITUM2083"
```
### Ejemplo contraseña de red
```bash
const char* password ="Passwordsegura1234"
```
### Ejemplo IP servidor (Broker)
```bash
const char* mqtt_server = "192.168.1.107"
```

Finalmente solo queda cargar el codigo a nuestra ESP32.

## Ejecución del Broker 
Ya que tenemos nuestros ambientes listos, y el codigo del Publisher esta cargado en nuestra ESP32, vamos a nuestra Arduino y ejecutamos el siguiente comando

```bash
mosquitto_sub -h localhost -t esp32/datos -v
```

y mientas en nuestra ESP32 la conectamos a una fuente de alimentación y veremos como empiezan a llegar los mensajes a nuestro servidor, asi como si vemos el monitor de nuestra ESP32 (_115200 bat_) vemos el mensaje que se esta enviando

<img width="975" height="548" alt="image" src="https://github.com/user-attachments/assets/1fa9a499-9a58-4e0a-ae84-6105ff6a908f" />






