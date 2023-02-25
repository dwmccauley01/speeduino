#include "globals.h"
#include "logger.h"
#include "errors.h"

/** 
 * Returns a numbered byte-field (partial field in case of multi-byte fields) from "current status" structure in the format expected by TunerStudio
 * Notes on fields:
 * - Numbered field will be fields from @ref currentStatus, but not at all in the internal order of strct (e.g. field RPM value, number 14 will be
 *   2nd field in struct)
 * - The fields stored in multi-byte types will be accessed lowbyte and highbyte separately (e.g. PW1 will be broken into numbered byte-fields 75,76)
 * - Values have the value offsets and shifts expected by TunerStudio. They will not all be a 'human readable value'
 * @param byteNum - byte-Field number. This is not the entry number (As some entries have multiple byets), but the byte number that is needed
 * @return Field value in 1 byte size struct fields or 1 byte partial value (chunk) on multibyte fields.
 */
byte getTSLogEntry(uint16_t byteNum)
{
  byte statusValue = 0;

  switch(byteNum)
  {
    case 0: statusValue = currentStatus.secl; break; //secl is simply a counter that increments each second. Used to track unexpected resets (Which will reset this count to 0)
    case 1: statusValue = currentStatus.status1; break; //status1 Bitfield
    case 2: statusValue = currentStatus.engine; break; //Engine Status Bitfield
    case 3: statusValue = currentStatus.syncLossCounter; break;
    case 4: statusValue = lowByte(currentStatus.MAP); break; //2 bytes for MAP
    case 5: statusValue = highByte(currentStatus.MAP); break;
    case 6: statusValue = (byte)(currentStatus.IAT + CALIBRATION_TEMPERATURE_OFFSET); break; //mat
    case 7: statusValue = (byte)(currentStatus.coolant + CALIBRATION_TEMPERATURE_OFFSET); break; //Coolant ADC
    case 8: statusValue = currentStatus.batCorrection; break; //Battery voltage correction (%)
    case 9: statusValue = currentStatus.battery10; break; //battery voltage
    case 10: statusValue = currentStatus.O2; break; //O2
    case 11: statusValue = currentStatus.egoCorrection; break; //Exhaust gas correction (%)
    case 12: statusValue = currentStatus.iatCorrection; break; //Air temperature Correction (%)
    case 13: statusValue = currentStatus.wueCorrection; break; //Warmup enrichment (%)
    case 14: statusValue = lowByte(currentStatus.RPM); break; //rpm HB
    case 15: statusValue = highByte(currentStatus.RPM); break; //rpm LB
    case 16: statusValue = (byte)(currentStatus.AEamount >> 1); break; //TPS acceleration enrichment (%) divided by 2 (Can exceed 255)
    case 17: statusValue = lowByte(currentStatus.corrections); break; //Total GammaE (%)
    case 18: statusValue = highByte(currentStatus.corrections); break; //Total GammaE (%)
    case 19: statusValue = currentStatus.VE1; break; //VE 1 (%)
    case 20: statusValue = currentStatus.VE2; break; //VE 2 (%)
    case 21: statusValue = currentStatus.afrTarget; break;
    case 22: statusValue = lowByte(currentStatus.tpsDOT); break; //TPS DOT
    case 23: statusValue = highByte(currentStatus.tpsDOT); break; //TPS DOT
    case 24: statusValue = currentStatus.advance; break;
    case 25: statusValue = currentStatus.TPS; break; // TPS (0% to 100%)
    
    case 26: 
      if(currentStatus.loopsPerSecond > 60000) { currentStatus.loopsPerSecond = 60000;}
      statusValue = lowByte(currentStatus.loopsPerSecond); 
      break;
    case 27: 
      if(currentStatus.loopsPerSecond > 60000) { currentStatus.loopsPerSecond = 60000;}
      statusValue = highByte(currentStatus.loopsPerSecond); 
      break;
    
    case 28: 
      currentStatus.freeRAM = freeRam();
      statusValue = lowByte(currentStatus.freeRAM); //(byte)((currentStatus.loopsPerSecond >> 8) & 0xFF);
      break; 
    case 29: 
      currentStatus.freeRAM = freeRam();
      statusValue = highByte(currentStatus.freeRAM); 
      break;

    case 30: statusValue = (byte)(currentStatus.boostTarget >> 1); break; //Divide boost target by 2 to fit in a byte
    case 31: statusValue = (byte)(currentStatus.boostDuty / 100); break;
    case 32: statusValue = currentStatus.spark; break; //Spark related bitfield

    //rpmDOT must be sent as a signed integer
    case 33: statusValue = lowByte(currentStatus.rpmDOT); break;
    case 34: statusValue = highByte(currentStatus.rpmDOT); break;

    case 35: statusValue = currentStatus.ethanolPct; break; //Flex sensor value (or 0 if not used)
    case 36: statusValue = currentStatus.flexCorrection; break; //Flex fuel correction (% above or below 100)
    case 37: statusValue = currentStatus.flexIgnCorrection; break; //Ignition correction (Increased degrees of advance) for flex fuel

    case 38: statusValue = currentStatus.idleLoad; break;
    case 39: statusValue = currentStatus.testOutputs; break;

    case 40: statusValue = currentStatus.O2_2; break; //O2
    case 41: statusValue = currentStatus.baro; break; //Barometer value

    case 42: statusValue = lowByte(currentStatus.canin[0]); break;
    case 43: statusValue = highByte(currentStatus.canin[0]); break;
    case 44: statusValue = lowByte(currentStatus.canin[1]); break;
    case 45: statusValue = highByte(currentStatus.canin[1]); break;
    case 46: statusValue = lowByte(currentStatus.canin[2]); break;
    case 47: statusValue = highByte(currentStatus.canin[2]); break;
    case 48: statusValue = lowByte(currentStatus.canin[3]); break;
    case 49: statusValue = highByte(currentStatus.canin[3]); break;
    case 50: statusValue = lowByte(currentStatus.canin[4]); break;
    case 51: statusValue = highByte(currentStatus.canin[4]); break;
    case 52: statusValue = lowByte(currentStatus.canin[5]); break;
    case 53: statusValue = highByte(currentStatus.canin[5]); break;
    case 54: statusValue = lowByte(currentStatus.canin[6]); break;
    case 55: statusValue = highByte(currentStatus.canin[6]); break;
    case 56: statusValue = lowByte(currentStatus.canin[7]); break;
    case 57: statusValue = highByte(currentStatus.canin[7]); break;
    case 58: statusValue = lowByte(currentStatus.canin[8]); break;
    case 59: statusValue = highByte(currentStatus.canin[8]); break;
    case 60: statusValue = lowByte(currentStatus.canin[9]); break;
    case 61: statusValue = highByte(currentStatus.canin[9]); break;
    case 62: statusValue = lowByte(currentStatus.canin[10]); break;
    case 63: statusValue = highByte(currentStatus.canin[10]); break;
    case 64: statusValue = lowByte(currentStatus.canin[11]); break;
    case 65: statusValue = highByte(currentStatus.canin[11]); break;
    case 66: statusValue = lowByte(currentStatus.canin[12]); break;
    case 67: statusValue = highByte(currentStatus.canin[12]); break;
    case 68: statusValue = lowByte(currentStatus.canin[13]); break;
    case 69: statusValue = highByte(currentStatus.canin[13]); break;
    case 70: statusValue = lowByte(currentStatus.canin[14]); break;
    case 71: statusValue = highByte(currentStatus.canin[14]); break;
    case 72: statusValue = lowByte(currentStatus.canin[15]); break;
    case 73: statusValue = highByte(currentStatus.canin[15]); break;

    case 74: statusValue = currentStatus.tpsADC; break;
    case 75: statusValue = getNextError(); break;

    case 76: statusValue = lowByte(currentStatus.PW1); break; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
    case 77: statusValue = highByte(currentStatus.PW1); break; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
    case 78: statusValue = lowByte(currentStatus.PW2); break; //Pulsewidth 2 multiplied by 10 in ms. Have to convert from uS to mS.
    case 79: statusValue = highByte(currentStatus.PW2); break; //Pulsewidth 2 multiplied by 10 in ms. Have to convert from uS to mS.
    case 80: statusValue = lowByte(currentStatus.PW3); break; //Pulsewidth 3 multiplied by 10 in ms. Have to convert from uS to mS.
    case 81: statusValue = highByte(currentStatus.PW3); break; //Pulsewidth 3 multiplied by 10 in ms. Have to convert from uS to mS.
    case 82: statusValue = lowByte(currentStatus.PW4); break; //Pulsewidth 4 multiplied by 10 in ms. Have to convert from uS to mS.
    case 83: statusValue = highByte(currentStatus.PW4); break; //Pulsewidth 4 multiplied by 10 in ms. Have to convert from uS to mS.

    case 84: statusValue = currentStatus.status3; break;
    case 85: statusValue = currentStatus.engineProtectStatus; break;
    case 86: statusValue = lowByte(currentStatus.fuelLoad); break;
    case 87: statusValue = highByte(currentStatus.fuelLoad); break;
    case 88: statusValue = lowByte(currentStatus.ignLoad); break;
    case 89: statusValue = highByte(currentStatus.ignLoad); break;
    case 90: statusValue = lowByte(currentStatus.dwell); break;
    case 91: statusValue = highByte(currentStatus.dwell); break;
    case 92: statusValue = currentStatus.CLIdleTarget; break;
    case 93: statusValue = lowByte(currentStatus.mapDOT); break;
    case 94: statusValue = highByte(currentStatus.mapDOT); break;
    case 95: statusValue = lowByte(currentStatus.vvt1Angle); break; //2 bytes for vvt1Angle
    case 96: statusValue = highByte(currentStatus.vvt1Angle); break;
    case 97: statusValue = currentStatus.vvt1TargetAngle; break;
    case 98: statusValue = (byte)(currentStatus.vvt1Duty); break;
    case 99: statusValue = lowByte(currentStatus.flexBoostCorrection); break;
    case 100: statusValue = highByte(currentStatus.flexBoostCorrection); break;
    case 101: statusValue = currentStatus.baroCorrection; break;
    case 102: statusValue = currentStatus.VE; break; //Current VE (%). Can be equal to VE1 or VE2 or a calculated value from both of them
    case 103: statusValue = currentStatus.ASEValue; break; //Current ASE (%)
    case 104: statusValue = lowByte(currentStatus.vss); break;
    case 105: statusValue = highByte(currentStatus.vss); break;
    case 106: statusValue = currentStatus.gear; break;
    case 107: statusValue = currentStatus.fuelPressure; break;
    case 108: statusValue = currentStatus.oilPressure; break;
    case 109: statusValue = currentStatus.wmiPW; break;
    case 110: statusValue = currentStatus.status4; break;
    case 111: statusValue = lowByte(currentStatus.vvt2Angle); break; //2 bytes for vvt2Angle
    case 112: statusValue = highByte(currentStatus.vvt2Angle); break;
    case 113: statusValue = currentStatus.vvt2TargetAngle; break;
    case 114: statusValue = (byte)(currentStatus.vvt2Duty); break;
    case 115: statusValue = currentStatus.outputsStatus; break;
    case 116: statusValue = (byte)(currentStatus.fuelTemp + CALIBRATION_TEMPERATURE_OFFSET); break; //Fuel temperature from flex sensor
    case 117: statusValue = currentStatus.fuelTempCorrection; break; //Fuel temperature Correction (%)
    case 118: statusValue = currentStatus.advance1; break; //advance 1 (%)
    case 119: statusValue = currentStatus.advance2; break; //advance 2 (%)
    case 120: statusValue = currentStatus.TS_SD_Status; break; //SD card status
    case 121: statusValue = lowByte(currentStatus.EMAP); break; //2 bytes for EMAP
    case 122: statusValue = highByte(currentStatus.EMAP); break;
    case 123: statusValue = currentStatus.fanDuty; break;
    case 124: statusValue = currentStatus.airConStatus; break;
  }

  return statusValue;
}

/** 
 * Similar to the @ref getTSLogEntry function, however this returns a full, unadjusted (ie human readable) log entry value.
 * See logger.h for the field names and order
 * @param logIndex - The log index required. Note that this is NOT the byte number, but the index in the log
 * @return Raw, unadjusted value of the log entry. No offset or multiply is applied like it is with the TS log
 */
int16_t getReadableLogEntry(uint16_t logIndex)
{
  int16_t statusValue = 0;

  switch(logIndex)
  {
    case 0: statusValue = currentStatus.secl; break; //secl is simply a counter that increments each second. Used to track unexpected resets (Which will reset this count to 0)
    case 1: statusValue = currentStatus.status1; break; //status1 Bitfield
    case 2: statusValue = currentStatus.engine; break; //Engine Status Bitfield
    case 3: statusValue = currentStatus.syncLossCounter; break;
    case 4: statusValue = currentStatus.MAP; break; //2 bytes for MAP
    case 5: statusValue = currentStatus.IAT; break; //mat
    case 6: statusValue = currentStatus.coolant; break; //Coolant ADC
    case 7: statusValue = currentStatus.batCorrection; break; //Battery voltage correction (%)
    case 8: statusValue = currentStatus.battery10; break; //battery voltage
    case 9: statusValue = currentStatus.O2; break; //O2
    case 10: statusValue = currentStatus.egoCorrection; break; //Exhaust gas correction (%)
    case 11: statusValue = currentStatus.iatCorrection; break; //Air temperature Correction (%)
    case 12: statusValue = currentStatus.wueCorrection; break; //Warmup enrichment (%)
    case 13: statusValue = currentStatus.RPM; break; //rpm HB
    case 14: statusValue = currentStatus.AEamount; break; //TPS acceleration enrichment (%)
    case 15: statusValue = currentStatus.corrections; break; //Total GammaE (%)
    case 16: statusValue = currentStatus.VE1; break; //VE 1 (%)
    case 17: statusValue = currentStatus.VE2; break; //VE 2 (%)
    case 18: statusValue = currentStatus.afrTarget; break;
    case 19: statusValue = currentStatus.tpsDOT; break; //TPS DOT
    case 20: statusValue = currentStatus.advance; break;
    case 21: statusValue = currentStatus.TPS; break; // TPS (0% to 100%)
    
    case 22: 
      if(currentStatus.loopsPerSecond > 60000) { currentStatus.loopsPerSecond = 60000;}
      statusValue = currentStatus.loopsPerSecond; 
      break;
    
    case 23: 
      currentStatus.freeRAM = freeRam();
      statusValue = currentStatus.freeRAM;
      break; 

    case 24: statusValue = currentStatus.boostTarget; break;
    case 25: statusValue = currentStatus.boostDuty; break;
    case 26: statusValue = currentStatus.spark; break; //Spark related bitfield
    case 27: statusValue = currentStatus.rpmDOT; break;
    case 28: statusValue = currentStatus.ethanolPct; break; //Flex sensor value (or 0 if not used)
    case 29: statusValue = currentStatus.flexCorrection; break; //Flex fuel correction (% above or below 100)
    case 30: statusValue = currentStatus.flexIgnCorrection; break; //Ignition correction (Increased degrees of advance) for flex fuel
    case 31: statusValue = currentStatus.idleLoad; break;
    case 32: statusValue = currentStatus.testOutputs; break;
    case 33: statusValue = currentStatus.O2_2; break; //O2
    case 34: statusValue = currentStatus.baro; break; //Barometer value

    case 35: statusValue = currentStatus.canin[0]; break;
    case 36: statusValue = currentStatus.canin[1]; break;
    case 37: statusValue = currentStatus.canin[2]; break;
    case 38: statusValue = currentStatus.canin[3]; break;
    case 39: statusValue = currentStatus.canin[4]; break;
    case 40: statusValue = currentStatus.canin[5]; break;
    case 41: statusValue = currentStatus.canin[6]; break;
    case 42: statusValue = currentStatus.canin[7]; break;
    case 43: statusValue = currentStatus.canin[8]; break;
    case 44: statusValue = currentStatus.canin[9]; break;
    case 45: statusValue = currentStatus.canin[10]; break;
    case 46: statusValue = currentStatus.canin[11]; break;
    case 47: statusValue = currentStatus.canin[12]; break;
    case 48: statusValue = currentStatus.canin[13]; break;
    case 49: statusValue = currentStatus.canin[14]; break;
    case 50: statusValue = currentStatus.canin[15]; break;
    
    case 51: statusValue = currentStatus.tpsADC; break;
    case 52: statusValue = getNextError(); break;

    case 53: statusValue = currentStatus.PW1; break; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
    case 54: statusValue = currentStatus.PW2; break; //Pulsewidth 2 multiplied by 10 in ms. Have to convert from uS to mS.
    case 55: statusValue = currentStatus.PW3; break; //Pulsewidth 3 multiplied by 10 in ms. Have to convert from uS to mS.
    case 56: statusValue = currentStatus.PW4; break; //Pulsewidth 4 multiplied by 10 in ms. Have to convert from uS to mS.
  
    case 57: statusValue = currentStatus.status3; break;
    case 58: statusValue = currentStatus.engineProtectStatus; break;

    case 59: break; //UNUSED!!

    case 60: statusValue = currentStatus.fuelLoad; break;
    case 61: statusValue = currentStatus.ignLoad; break;
    case 62: statusValue = currentStatus.dwell; break;
    case 63: statusValue = currentStatus.CLIdleTarget; break;
    case 64: statusValue = currentStatus.mapDOT; break;
    case 65: statusValue = currentStatus.vvt1Angle; break;
    case 66: statusValue = currentStatus.vvt1TargetAngle; break;
    case 67: statusValue = currentStatus.vvt1Duty; break;
    case 68: statusValue = currentStatus.flexBoostCorrection; break;
    case 69: statusValue = currentStatus.baroCorrection; break;
    case 70: statusValue = currentStatus.VE; break; //Current VE (%). Can be equal to VE1 or VE2 or a calculated value from both of them
    case 71: statusValue = currentStatus.ASEValue; break; //Current ASE (%)
    case 72: statusValue = currentStatus.vss; break;
    case 73: statusValue = currentStatus.gear; break;
    case 74: statusValue = currentStatus.fuelPressure; break;
    case 75: statusValue = currentStatus.oilPressure; break;
    case 76: statusValue = currentStatus.wmiPW; break;
    case 77: statusValue = currentStatus.status4; break;
    case 78: statusValue = currentStatus.vvt2Angle; break; //2 bytes for vvt2Angle
    case 79: statusValue = currentStatus.vvt2TargetAngle; break;
    case 80: statusValue = currentStatus.vvt2Duty; break;
    case 81: statusValue = currentStatus.outputsStatus; break;
    case 82: statusValue = currentStatus.fuelTemp; break; //Fuel temperature from flex sensor
    case 83: statusValue = currentStatus.fuelTempCorrection; break; //Fuel temperature Correction (%)
    case 84: statusValue = currentStatus.advance1; break; //advance 1 (%)
    case 85: statusValue = currentStatus.advance2; break; //advance 2 (%)
    case 86: statusValue = currentStatus.TS_SD_Status; break; //SD card status
    case 87: statusValue = currentStatus.EMAP; break;
    case 88: statusValue = currentStatus.fanDuty; break;
    case 89: statusValue = currentStatus.airConStatus; break;
  }

  return statusValue;
}

/** 
 * An expansion to the @ref getReadableLogEntry function for systems that have an FPU. It will provide a floating point value for any parameter that this is appropriate for, otherwise will return the result of @ref getReadableLogEntry.
 * See logger.h for the field names and order
 * @param logIndex - The log index required. Note that this is NOT the byte number, but the index in the log
 * @return float value of the requested log entry. 
 */
#if FPU_MAX_SIZE >= 32
float getReadableFloatLogEntry(uint16_t logIndex)
{
  float statusValue = 0.0;

  switch(logIndex)
  {
    case 8: statusValue = currentStatus.battery10 / 10.0; break; //battery voltage
    case 9: statusValue = currentStatus.O2 / 10.0; break;
    case 18: statusValue = currentStatus.afrTarget / 10.0; break;
    case 21: statusValue = currentStatus.TPS / 2.0; break; // TPS (0% to 100% = 0 to 200)
    case 33: statusValue = currentStatus.O2_2 / 10.0; break; //O2

    case 53: statusValue = currentStatus.PW1 / 1000.0; break; //Pulsewidth 1 Have to convert from uS to mS.
    case 54: statusValue = currentStatus.PW2 / 1000.0; break; //Pulsewidth 2 Have to convert from uS to mS.
    case 55: statusValue = currentStatus.PW3 / 1000.0; break; //Pulsewidth 3 Have to convert from uS to mS.
    case 56: statusValue = currentStatus.PW4 / 1000.0; break; //Pulsewidth 4 Have to convert from uS to mS.

    default: statusValue = getReadableLogEntry(logIndex); break; //If logIndex value is NOT a float based one, use the regular function
  }

  return statusValue;
}
#endif

/** 
 * Searches the log 2 byte array to determine whether a given index is a regular single byte or a 2 byte field
 * Uses a boundless binary search for improved performance, but requires the fsIntIndex to remain in order
 * Refer: https://github.com/scandum/binary_search
 * 
 * @param key - Index in the log array to check
 * @return True if the index is a 2 byte log field. False if it is a single byte
 */
bool is2ByteEntry(uint8_t key)
{
  bool isFound = false;
  unsigned int mid, bot;
  uint16_t array_size = sizeof(fsIntIndex);

  if (array_size > 0)
  {
    bot = 0;
    mid = array_size;
  
    while (mid > 1)
    {  
      if (key >= pgm_read_byte( &fsIntIndex[bot + mid / 2]) )
      {
        bot += mid++ / 2;
      }
      mid /= 2;
    }
  
    if (key == pgm_read_byte(&fsIntIndex[bot]) )
    {
      isFound = true;
    }
  }

  return isFound;
}

void startToothLogger(void)
{
  currentStatus.toothLogEnabled = true;
  currentStatus.compositeLogEnabled = false; //Safety first (Should never be required)
  BIT_CLEAR(currentStatus.status1, BIT_STATUS1_TOOTHLOG1READY);
  toothHistoryIndex = 0;

  //Disconnect the standard interrupt and add the logger version
  detachInterrupt( digitalPinToInterrupt(pinTrigger) );
  attachInterrupt( digitalPinToInterrupt(pinTrigger), loggerPrimaryISR, CHANGE );

  detachInterrupt( digitalPinToInterrupt(pinTrigger2) );
  attachInterrupt( digitalPinToInterrupt(pinTrigger2), loggerSecondaryISR, CHANGE );  
}

void stopToothLogger(void)
{
  currentStatus.toothLogEnabled = false;

  //Disconnect the logger interrupts and attach the normal ones
  detachInterrupt( digitalPinToInterrupt(pinTrigger) );
  attachInterrupt( digitalPinToInterrupt(pinTrigger), triggerHandler, primaryTriggerEdge );

  detachInterrupt( digitalPinToInterrupt(pinTrigger2) );
  attachInterrupt( digitalPinToInterrupt(pinTrigger2), triggerSecondaryHandler, secondaryTriggerEdge );  
}

void startCompositeLogger(void)
{
  currentStatus.compositeLogEnabled = true;
  currentStatus.toothLogEnabled = false; //Safety first (Should never be required)
  BIT_CLEAR(currentStatus.status1, BIT_STATUS1_TOOTHLOG1READY);
  toothHistoryIndex = 0;

  //Disconnect the standard interrupt and add the logger version
  detachInterrupt( digitalPinToInterrupt(pinTrigger) );
  attachInterrupt( digitalPinToInterrupt(pinTrigger), loggerPrimaryISR, CHANGE );

  detachInterrupt( digitalPinToInterrupt(pinTrigger2) );
  attachInterrupt( digitalPinToInterrupt(pinTrigger2), loggerSecondaryISR, CHANGE );
}

void stopCompositeLogger(void)
{
  currentStatus.compositeLogEnabled = false;

  //Disconnect the logger interrupts and attach the normal ones
  detachInterrupt( digitalPinToInterrupt(pinTrigger) );
  attachInterrupt( digitalPinToInterrupt(pinTrigger), triggerHandler, primaryTriggerEdge );

  detachInterrupt( digitalPinToInterrupt(pinTrigger2) );
  attachInterrupt( digitalPinToInterrupt(pinTrigger2), triggerSecondaryHandler, secondaryTriggerEdge );
}
