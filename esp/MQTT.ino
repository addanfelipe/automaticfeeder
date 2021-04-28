#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
#include <sys/time.h>

#define IO_USERNAME ""
#define IO_KEY ""
const char *ssid = "";
const char *password = "";
const char *mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char *mqttUser = IO_USERNAME;
const char *mqttPassword = IO_KEY;

WiFiClient espClient;
WiFiUDP ntpUDP;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// COnfigurações do servidor NTP para buscar a data atual na Internet
const char *ntpServer = "0.br.pool.ntp.org";
const int timeZone = -3;

NTPClient ntpClient(
    ntpUDP,
    ntpServer,
    timeZone * 3600,
    60000);

typedef struct
{
  char inicio[10];
  char fim[10];
  int count;
} hora;

typedef struct
{
  char string[20];
} arraystr;

// Entradas digitais
#define LED 32
#define BUZ 34
#define POT 33

//Variavéis
int hora_salvo_ini = 0;
int minuto_salvo_ini = 0;
int hora_salvo_fim = 0;
int minuto_salvo_fim = 0;
int hora_atual = 0;
int minuto_atual = 0;
float pot = 0;
int pot_int = 0;
hora horarios[20];
char *pch;
char *pch2;
arraystr aux[20];
int peso_ideal;
struct tm dataAtual;
char date[50];
void setup_wifi()
{

  delay(10);
  analogSetAttenuation(ADC_0db);          // Atenuação do ADC 1,1V - 4095
  analogSetPinAttenuation(POT, ADC_11db); // Atenuação do ADC 3,3V - 4095
  ledcAttachPin(BUZ, 0);
  ledcSetup(0, 5000, 12);

  pinMode(LED, OUTPUT);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Messagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  char messageTemp[100];

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    messageTemp[i] = (char)payload[i];
  }
  Serial.println();
  if (strcmp(topic, "isaken/feeds/alimentador.periodoverificacaodepeso") == 0)
  { //pega a string do periodo de verificação no formato 00:00/00:00_00:00/00:00 e separa
    pch = strtok(messageTemp, "_");
    while (pch != NULL)
    {
      strcpy(aux[horarios[0].count].string, pch);
      horarios[0].count = horarios[0].count + 1;
      pch = strtok(NULL, "_");
    }
    for (int i = 0; i < horarios[0].count; i++)
    {
      pch = strtok(aux[i].string, "/");
      strcpy(horarios[i].inicio, pch);
      pch = strtok(NULL, "/");
      strcpy(horarios[i].fim, pch);
    }
  } //fim
  else if (strcmp(topic, "isaken/feeds/alimentador.pesominimo") == 0)
  {
    peso_ideal = atoi(messageTemp);
    Serial.print(peso_ideal);
    Serial.println();
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Tentando conexão MQTT...");
    // Create a random client ID
    String clientId = "ESP32 - Sensores";
    clientId += String(random(0xffff), HEX);
    // Se conectado
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword))
    {
      Serial.println("conectado");
      // Depois de conectado, publique um anúncio ...
      client.publish("isaken/feeds/alimentador.pesoatual", "Iniciando Comunicação"); // publicando o peso no feed do mqtt
      //... e subscribe.
      client.subscribe("isaken/feeds/alimentador.pesominimo");
      //client.subscribe("isaken/feeds/alimentador.pesoideal");
      client.subscribe("isaken/feeds/alimentador.periodoverificacaodepeso");
      //client.subscribe("isaken/feeds/alimentador.momentosverificacao");
    }
    else
    {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s");
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttserver, 1883); // Publicar
  client.setCallback(callback);       // Receber mensagem

  //Inicializa o cliente NTP para atualizar a data/hora do sistema
  ntpClient.begin();
  ntpClient.forceUpdate();
  int horaNtp = ntpClient.getEpochTime();
  Serial.print("Hora atual no padrão Unix: ");
  Serial.println(horaNtp);
  timeval tv;
  tv.tv_sec = horaNtp;
  settimeofday(&tv, NULL);          //Atualiza o relógio interno (RTC) do ESP.
  time_t tempoAtual = time(NULL);   //Busca a hora atual do RTC.
  dataAtual = *gmtime(&tempoAtual); //Converte o tempo atual para o padrão e salva na variável.
  Serial.print("Data atual formatada: ");
  char dataFormatada[64];
  strftime(dataFormatada, 64, "%d/%m/%Y %H:%M:%S", &dataAtual);
  Serial.println(dataFormatada);
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  delay(10000); //Intervalo de 10s entre leituras
  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;

    pot = analogRead(POT) / 10;
    pot_int = round(pot);
    time_t tempoAtual = time(NULL);   //Busca a hora atual do RTC.
    dataAtual = *gmtime(&tempoAtual); //Converte o tempo atual para o padrão e salva na variável.
    for (int i = 0; i < horarios[0].count; i++)
    {
      strcpy(aux[0].string, horarios[i].inicio);
      strcpy(aux[1].string, horarios[i].fim);
      pch = strtok(horarios[i].inicio, ":");
      hora_salvo_ini = atoi(pch);
      pch = strtok(NULL, ":");
      minuto_salvo_ini = atoi(pch);
      pch = strtok(horarios[i].fim, ":");
      hora_salvo_fim = atoi(pch);
      pch = strtok(NULL, ":");
      minuto_salvo_fim = atoi(pch);
      strcpy(horarios[i].inicio, aux[0].string);
      strcpy(horarios[i].fim, aux[1].string);
      strftime(aux[0].string, 64, "%H", &dataAtual);
      strftime(aux[1].string, 64, "%M", &dataAtual);
      hora_atual = atoi(aux[0].string);
      minuto_atual = atoi(aux[1].string);
      if ((hora_atual >= hora_salvo_ini && hora_atual < hora_salvo_fim) || (hora_atual == hora_salvo_fim && minuto_atual <= minuto_salvo_fim))
      {
        if (pot_int < peso_ideal)
        {
          digitalWrite(LED, HIGH);
          ledcWriteTone(0, 3000);
          Serial.println(minuto_atual);
          Serial.print("Liberando ração");
          Serial.println();
          delay(5000);
          digitalWrite(LED, LOW);
          break;
        }
      }
    }
    char s_pot[8];
    dtostrf(pot, 1, 2, s_pot);
    Serial.print("Peso (em gramas): ");
    Serial.println(s_pot);
    Serial.println(pot);
    client.publish("isaken/feeds/alimentador.pesoatual", s_pot);
  }
}
