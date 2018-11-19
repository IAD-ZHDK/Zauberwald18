//
// code example from https://github.com/espressif/arduino-esp32/issues/849 thanks to bmitov 
//
namespace Mitov
{
  uint8_t CESP32PWMTimers[ 2 ] = {0};
  uint8_t CESP32PWMPinMap[ 40 ] = {0};
}
  void analogWrite( uint8_t APin, uint16_t AValue )
  {
//    Serial.println( "analogWrite" );
//    Serial.println( AValue );
    if( AValue == 0 || AValue == 255 )
    {
      if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
      {
        ledcWrite( Mitov::CESP32PWMPinMap[ APin ] - 1, AValue );
//        Serial.println( "ledcDetachPin" );
        digitalWrite( APin, (AValue == 255 ) ? HIGH : LOW );
        ledcDetachPin( APin );
        uint8_t AChannel = Mitov::CESP32PWMPinMap[ APin ] - 1;
        Mitov::CESP32PWMTimers[ AChannel >> 3 ] &= ~ ( 1 << ( AChannel & 7 ) );
        Mitov::CESP32PWMPinMap[ APin ] = 0;
      }
      
      digitalWrite( APin, (AValue == 255 ) ? HIGH : LOW );
      return;
    }

    if( Mitov::CESP32PWMPinMap[ APin ] == 0 )
    {
      uint8_t AChannel = 0;
      for( AChannel = 0; AChannel < 16; ++AChannel )
        if( ! ( Mitov::CESP32PWMTimers[ AChannel >> 3 ] & ( 1 << ( AChannel & 7 ) ) ))
          break;

      if( AChannel == 16 )
        return; // Can't allocate channel

      Mitov::CESP32PWMPinMap[ APin ] = AChannel + 1;
      Mitov::CESP32PWMTimers[ AChannel >> 3 ] != ( 1 << ( AChannel & 7 ) );
      ledcSetup( AChannel, 1000, 8 ); // 1KHz 8 bit
      ledcWrite( AChannel, AValue );
      ledcAttachPin( APin, AChannel );
//      Serial.print( "ledcAttachPin: " ); Serial.print( APin ); Serial.print( " " ); Serial.println( AChannel );
      return;
    }

    ledcWrite( Mitov::CESP32PWMPinMap[ APin ] - 1, AValue );
//    Serial.print( "ledcWrite: " ); Serial.print( Mitov::CESP32PWMPinMap[ APin ] - 1 ); Serial.print( " " ); Serial.println( AValue );
  }
