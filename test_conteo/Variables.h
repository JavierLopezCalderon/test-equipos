//ENTRADAS Y SALIDAS
#define PinBotonReset 27
#define IndicadorWebSocket 12
#define IndicadorActividad 14
#define Relevador 13
const int PinSensorMagnetico[2] = {36, 39}; 
#define Buzzer 17
const int ledPin = 15;
const int LedIndicadorSensores[3] = {2, 4, 16}; //SUPERIOR, MEDIO, INFERIOR

// VARIABLES ENVIO DE SEÑAL PWM //
const int freq = 38000;//38kHz   //50000//50KHz
const int ledChannel = 0;
const int resolution = 8;

const char * File_Contadores_Flash[18] = {"CS", "CB", "L1S", "L2S", "L3S", "L1B", "L2B", "L3B",
                                          "CBS1", "CBS2" ,"CBS3" ,"CBS4" ,"CBS5" ,"CBS6" , "CBL1", "CBL2", "CBL3", "CBT"};

int enableSaveCount;
int HabilitarRegistroBloqueoTotal, GuardarRegistroBloqueoTotal;
Preferences preferences;

//CONTADORES BLOQUEOS
int CountSensoresBloqueo[6];
int CountLineaBloqueos[3];
int CountBloqueoTotal;

// INSTANCIA LCD 20x4 
LiquidCrystal_I2C lcd(0x27, 20, 4); 

//VARIABLES LOGICA SENSORES //
String PARTE_DE_REGISTRO;
class Sensor_var{   
  public:
    const int Pin[6] = {25, 26, 32, 33, 34, 35}; //PINES DE SENSORES
    int Estado, Lectura, Count , AntEstado, Bloqueo, EnableB;
    int GuardarRegistroBloqueo, HabilitarRegistroBloqueo;
    unsigned long TiempoBloqueado;
    unsigned long previousMillisBloqueo ;
};
Sensor_var Sensor[6];
class Estados{
  public:
    int Posicion1, Posicion2, Posicion3, Completo, Evento, Termino, Suma, Valant, ES;
};
class Lineas{
  public:
   int Sentido, Entrando, Bajando, Aux, Posicion;
   int SensorLineaUp, CountLineaUp, Bloqueo;
   int Enable, countEnable;
   unsigned long previousMillisBloqueoLinea, TiempoLineaBloqueado, BloqueoSuma;
   unsigned long previousMillisSensado;
   int GuardarRegistroBloqueo, HabilitarRegistroBloqueo;
    Estados Sub, Baj;
};
Lineas Linea[3];
int STS, STB, CountSubida, CountBajada; //(STS)Suma total subida, (STB)Suma total bajada
int BS, SBL, edo_sal, edo_ent; // (SBL)suma bloqueos
class var_sens_mag{
  public:
    String Clave_Registro_sd, Hora_Fecha_Registro_sd, longitud_registro_sd, latitud_registro_sd;
};
var_sens_mag LecturaP1, LecturaP2;

// VARIBLES TIMERS //
Ticker Ticker1;
const int limit = 5;
const int timer_interrupcion = 5;
const int timer_bloqueo = 1000;
const int intervalEnable = 800; //800ms
const int intervalBloqueo = 12000; //12s
const int intervalSuma = 800;
int SumaEnable;
unsigned long previousMillisBloqueoSuma;
int BloqueoTotal;
unsigned long previousMillisBloqueoTotal , TimepoBloqueoTotal;
const int intervaLEnvioStatus = 20000; //20seg//Envio_de_status
unsigned long previousMillisEnvioStatus = millis();
unsigned long TimerCurrient;
int EnableActivateRele;
//REENVIAR REGISTRO SESNORES MAGNETICOS
unsigned long PreviousEnvioRegistroSensorMag[2];
//TIMER RELEVADOR
const int intervalActivateRele = 600; //600ms
unsigned long previousActivateRele;

//AUXILIAR
int Reset;
