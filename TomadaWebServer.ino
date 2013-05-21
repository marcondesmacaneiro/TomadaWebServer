#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x40, 0x02 };
IPAddress ip(192,168,100, 110);
int ligaLed; 
String rd; 
EthernetServer server(80); 

void setup(){
   Ethernet.begin(mac, ip); //define IP e MAC na shield
   server.begin(); //Começa a ouvir
   pinMode(3,OUTPUT); 
   ligaLed = 0; 
   rd = "rd=0"; 
}

void loop(){
   EthernetClient client = server.available(); //Servidor disponivel a receber chamadas
   if(client){ //Se tiver cliente conectado
     String get; //Cria um String essa String armazenará o que vem do cliente
     while(client.connected()){ //Inicia um laço, que repetirá enquanto houver o cliente conectado
         if(client.available()){ //Se tiver conexão com cliente
           char c = client.read(); //Le o que vem do cliente (neste caso vem Char por Char)
           get.concat(c); //Vai concatenando cada char que vem na String get
           if (c == '\n') { // o \n é simbolo de final de String, ou seja, o que tinha pra concatenar ja concatenou.
              if(get.substring(6,10) == "rd=1"){ //rd=1 é o valor do check Ligado
                 ligaLed = 1; //Define que é para ligar o led
                 rd = "rd=1"; //Salva estado do check, ou seja, se o usuario recarregar a pagina ou acessar de outra maquina vai vir checkado o Ligado
              }else{
                 if(get.substring(6,10) == "rd=0"){ //rd=0 é o valor do check Desligado
                    ligaLed = 0; // Define que é para desligar o led
                    rd = "rd=0"; // //Salva estado do check, ou seja, se o usuario recarregar a pagina ou acessar de outra maquina vai vir checkado o Desligado
                 }
              }
              //Escreve o HTML da pagina
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println("<html><head><title>.::Acionamento Led::.</title></head><body>");
              client.println();
              client.print("<form method='get'>"); //Usei a passagem dos valores com form e metodo get
              if(rd == "rd=1"){ //Aqui ele vai escrever o html caso venho rd=1 o Ligado será checkado
                 client.print("<input type='radio' value='0' name='rd' > Desligado<br>");
                 client.print("<input type='radio' value='1' checked name='rd' > Ligado<br>"); 
              }else{ //Aqui ele vai escrever o html caso venho rd=1 o Desligado será checkado
                 client.print("<input type='radio' value='0' checked name='rd' > Desligado<br>"); 
                 client.print("<input type='radio' value='1' name='rd' > Ligado<br>");           
              }
              client.print("<input type=submit value='Submit'></form>");
              client.print("</body></html>");
              //Acaba de escrever o HTML
              client.stop();//Desconecta o cliente 
           }
         }
      }
      if(ligaLed == 1){
         digitalWrite(3, HIGH); //Acende o led
      }else{
         digitalWrite(3, LOW); //Apaga o led
      }
   }   
}
