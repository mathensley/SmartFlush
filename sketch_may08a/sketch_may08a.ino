#include <Ethernet.h>
#include <SPI.h>
#include <Servo.h>

//Configuracao ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//IPAddress ip(169,254,0,87);
//IPAddress gateway(169,254,0,1);
//IPAddress subnet(255,255,0,0);

IPAddress ip(192,168,0,102);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

//Inicializa servidor porta 80
EthernetServer server(80);

//Motor
Servo motor;

//Receber dado digital
int recebe1;
int recebe2;

//Pinos do botoes
int botao1 = 0; //azul
int botao2 = 3; //verde

//Contador para botao1
int cont = 0;

//Dados do gasto de agua
int dadoDescarga = 0;
unsigned int dadoDia = 0;

//Funcao para mover o servo motor
void girar(int angulo, int seg){
  motor.write(0);
  delay(100);
  motor.write(angulo);
  delay(seg);
  motor.write(0);
}

void setup() {
  motor.attach(7);
  Serial.begin(9600);
  motor.write(0);
  pinMode(botao1, INPUT);
  digitalWrite(botao1, HIGH);
  pinMode(botao2, INPUT);
  digitalWrite(botao2, HIGH);  
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void loop() {
  
  EthernetClient client = server.available();

  if(client){
    Serial.println("new client");

    boolean currentLineIsBlank = true;
    while (client.connected()) {
      char c = client.read();
      Serial.write(c);
      if (c == 'n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 5"); //Recarrega a pagina a cada 5seg
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>");
          client.println("SmartFlush");
          client.println("</title>");
          client.println("</head>");
          //Configura o texto e imprime o titulo no browser
          client.print("<font color=#FF0000><b><u>");
          client.print("Dados Estatisticos do Consumo de Agua:");
          client.print("</u></b></font>");
          client.println("<br />");
          client.println("<br />");
          client.print("Dados p/ Descarga(Litros): ");
          client.print("<b>");
          client.print(dadoDescarga);
          client.print("</b>");
          client.println("<br />");
          client.print("Dados p/ Dia(Litros): ");
          client.print("<b>");
          client.print(dadoDia);
          client.print("</b>");
          client.println("</b></html>");
          break;
    }
  }
  // give the web browser time to receive the data
  delay(1);
  // close the connection:
  client.stop();
 }

 //Interacao botoes
  recebe1 = digitalRead(botao1);
  recebe2 = digitalRead(botao2);
  
  if(recebe1 != 1){
    dadoDescarga = 0;
    if(cont < 3){
      cont++;
      girar(60, 3000);
      dadoDescarga = 5;
      dadoDia += 5;
    }
    else{
      cont = 0;
      girar(90, 6000);
      dadoDescarga = 10;
      dadoDia += 10;
    }
  }
  if(recebe2 != 1){
    dadoDescarga = 0;
    cont = 0;
    girar(90, 6000);
    dadoDescarga = 10;
    dadoDia += 10;
  }
 
}
