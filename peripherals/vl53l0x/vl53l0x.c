/*
 * vl53l0x.c
 *
 */

#include <peripherals/vl53l0x/vl53l0x.h>
#include <peripherals/vl53l0x/vl53l0x_api.h>
#include <peripherals/vl53l0x/vl53l0x_i2c_platform.h>
#include <peripherals/vl53l0x/vl53l0x_platform.h>
#include "stdbool.h"
#include "utilities/fsl_debug_console.h"

#define ERR_FAILED	1
#define ERR_OK		0

static VL53L0X_Dev_t MyDevice; /* device descriptor */

static bool_t isMeasurementReady(void){
  uint8_t deviceReady;
  uint32_t err_code;

  err_code = VL53L0X_GetMeasurementDataReady(&MyDevice, &deviceReady);
  if(VL53L0X_ERROR_NONE != err_code) {
    PRINTF("ERROR: VL53L0X_GetMeasurementDataReady: %d\r\n", err_code);
    return false;
  }
  return deviceReady!=0;
}

uint8_t VL53L0X_MeasureSingle(uint16_t *mm) {
  VL53L0X_RangingMeasurementData_t rangingData;
  uint32_t err_code;

  err_code = VL53L0X_StartMeasurement(&MyDevice);
  if (err_code!=VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_StartMeasurement() failed\r\n");
    return ERR_FAILED;
  }
  do{
    /* blocking wait! */
  } while(!isMeasurementReady());

  err_code = VL53L0X_GetRangingMeasurementData(&MyDevice, &rangingData);
  if (err_code!=VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_GetRangingMeasurementData() failed\r\n");
    return ERR_FAILED;
  }
  err_code = VL53L0X_ClearInterruptMask(&MyDevice, 0);
  if (err_code!=VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_ClearInterruptMask() failed\r\n");
    return ERR_FAILED;
  }
  if (mm!=NULL) {
    *mm = rangingData.RangeMilliMeter;
  }
  return ERR_OK;
}

uint8_t VL53L0X_Init(void) {
  VL53L0X_Error err_code;
  uint8_t vhvCalibrationValue;
  uint8_t phaseCalibrationValue;
  uint32_t refSpadCount;
  uint8_t isApertureSpads;

  /* Initialize Comms */
  MyDevice.I2cDevAddr      =  0x29;
  MyDevice.comms_type      =  1;
  MyDevice.comms_speed_khz =  400;

  err_code = VL53L0X_comms_initialise(I2C, MyDevice.comms_speed_khz);
  if(err_code!=VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_comms_initialise(): %d\r\n", err_code);
    return ERR_FAILED;
  }

  err_code = VL53L0X_DataInit(&MyDevice);
  if(err_code!=VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: DataInit(): %d\r\n", err_code);
    return ERR_FAILED;
  }

  err_code = VL53L0X_StaticInit(&MyDevice);
  if(err_code != VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: StaticInit: %d\r\n", err_code);
    return ERR_FAILED;
  }

  VL53L0X_PerformRefSpadManagement(&MyDevice, &refSpadCount, &isApertureSpads);
  if(err_code!=VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: SpadCal: %d\r\n", err_code);
    return ERR_FAILED;
  }

  err_code = VL53L0X_PerformRefCalibration(&MyDevice, &vhvCalibrationValue, &phaseCalibrationValue);
  if(err_code!=VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_PerformRefCalibration(): %d\r\n", err_code);
    return ERR_FAILED;
  }

  err_code = VL53L0X_SetDeviceMode(&MyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING);
  if(err_code != VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_SetDeviceMode(): %d\r\n", err_code);
    return ERR_FAILED;
  }

  /* Enable/Disable Sigma and Signal check */
  err_code = VL53L0X_SetLimitCheckEnable(&MyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1 );
  if(err_code != VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_SetLimitCheckEnable(): %d\r\n", err_code);
    return ERR_FAILED;
  }

  err_code = VL53L0X_SetLimitCheckEnable(&MyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1 );
  if(err_code != VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_SetLimitCheckEnable(): %d\r\n", err_code);
    return ERR_FAILED;
  }

  err_code = VL53L0X_SetLimitCheckEnable(&MyDevice, VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, 1 );
  if(err_code != VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_SetLimitCheckEnable(): %d\r\n", err_code);
    return ERR_FAILED;
  }

  err_code = VL53L0X_SetLimitCheckValue(&MyDevice, VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, (FixPoint1616_t)( 1.5 * 0.023 * 65536 ) );
  if(err_code != VL53L0X_ERROR_NONE) {
    PRINTF("ERROR: VL53L0X_SetLimitCheckValue(): %d\r\n", err_code);
    return ERR_FAILED;
  }
  return ERR_OK;
}

