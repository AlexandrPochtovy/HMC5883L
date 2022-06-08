/*********************************************************************************
   Original author: Alexandr Pochtovy<alex.mail.prime@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 *  HMC5883L.h
 *  Created on: Jan 31, 2022
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "I2C_Master/MyI2C.h"
#include "HMC5883L_Register.h"
/* состояние процесса обмена данными с устройством как с отдельным элементом сети
 * 	применяется для отображения состояния процесса работы с устройством для главного кода
 */
typedef enum HMC5883L_status_t {//состояние устройства
	HMC_Init,		//устройство не настроено
	HMC_OK,		//устройство готово к опросу
	HMC_Faulth	//устройство неисправно
} HMC5883L_status;

/*	состояние обмена данными с устройством, использовать для завершения функции работы с устройством */
typedef enum HMC5883L_Connect_Status_t {
	HMC_Processing, //выполняется работа с устройством: обмен данными, обработка результатов
	HMC_Complite	//работа с устройством завершена, данные считаны/записаны корректно
} HMC5883L_Connect_Status;

typedef struct HMC5883L_raw_data_t {
	uint16_t X;
	uint16_t Y;
	uint16_t Z;
	uint8_t status;
	uint32_t ID;
} HMC5883L_raw_data;

typedef struct HMC5883L_data_t {
	float X;
	float Y;
	float Z;
} HMC5883L_data;
//common data struct for sensor
typedef struct HMC5883L_dev_t {
	const uint8_t addr;
	uint8_t step;
	HMC5883L_status status;
	HMC5883L_raw_data raw;
	HMC5883L_data data;
} HMC5883L_dev;

//INITIALIZATION	================================================================
HMC5883L_Connect_Status HMC5883L_Init(I2C_Connection *_i2c, HMC5883L_dev *dev, uint8_t *pbuffer);
HMC5883L_Connect_Status HMC5883L_GetData(I2C_Connection *_i2c, HMC5883L_dev *dev, uint8_t *pbuffer);


#ifdef __cplusplus
}
#endif
#endif /* HMC5883L_H_ */
