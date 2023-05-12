//=====[Libraries]=============================================================

#include "arm_book_lib.h"

#include "bright_control.h"

#include "light_level_control.h"

//=====[Declaration of private defines]========================================

#define LEDS_QUANTITY 2

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//configurando los pines de salida puedo elegir usar los led de usauario de la placa
//probamos la intensidad del led2 de la placa mapeado a PB_7
DigitalOut RGBLed[] = {(PB_4), (PB_14), (PD_12)};


Ticker tickerBrightControl;

//=====[Declaration and initialization of private global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static int onTime[LEDS_QUANTITY];
static int offTime[LEDS_QUANTITY];

static int tickRateMSBrightControl = 1;
static int tickCounter[LEDS_QUANTITY];

static float periodSFloat[LEDS_QUANTITY];

//=====[Declarations (prototypes) of private functions]========================

static void setPeriod( lightSystem_t light, float period );
static void tickerCallbackBrightControl( );

//=====[Implementations of public functions]===================================

void brightControlInit()
{
    //el metodo attach llama al metodo tickerCallbackBrightControl a intervalos definidos en el segundo atributo
    //el modod de funcionamiento de esta implementacion es utilizar el metodo attach que a su vez 
    //utiliza la clase chrono de std para cambiar el valor de la salida del pin a intervalos regulares de tiempo 
    //lo que hace es llamar a intervalos regulares dependiendo de un valor de entrada analogica
    tickerBrightControl.attach( tickerCallbackBrightControl, 
                              ( (float) tickRateMSBrightControl) / 1000.0 );

    setPeriod( RGB_LED_RED, 0.01f );
    setPeriod( RGB_LED_GREEN, 0.01f );
    //setPeriod( RGB_LED_BLUE, 0.01f );

    setDutyCycle( RGB_LED_RED, 0.5f );
    setDutyCycle( RGB_LED_GREEN, 0.5f );
    //setDutyCycle( RGB_LED_BLUE, 0.5f );
}

void setDutyCycle( lightSystem_t light, float dutyCycle )
{
    onTime[light] = int ( ( periodSFloat[light] * dutyCycle ) * 1000 );
    offTime[light] = int ( periodSFloat[light] * 1000) - onTime[light];
}

//=====[Implementations of private functions]==================================

static void setPeriod( lightSystem_t light, float period )
{
    periodSFloat[light] = period;
}

static void tickerCallbackBrightControl( )
{
    int i;

    for (i = 0 ; i < LEDS_QUANTITY ; i++) {
        tickCounter[i]++;
        if ( RGBLed[i].read() == ON ) {
            if( tickCounter[i] > onTime[i] ) {
                tickCounter[i] = 1;
                if ( offTime[i] ) RGBLed[i] = OFF;
                
            }
        } else {
            if( tickCounter[i] > offTime[i] ) { 
                tickCounter[i] = 1;
                if ( onTime[i] ) RGBLed[i] = ON;
            }
        }
    }
}