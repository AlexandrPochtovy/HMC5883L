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

 *  HMC5883L.c
 *  Created on: Jan 31, 2022
 */
#include "HMC5883L.h"

static const uint8_t HMC5883L_DEFAULT_ADDRESS = 0x3C;
static const uint32_t HMC5883L_CHIP_ID = 0x00483433; //< Assumes ALT address pin low

HMC5883L_Connect_Status HMC5883L_Init(I2C_Connection *_i2c, HMC5883L_dev *dev, uint8_t *pbuffer) {
	if (_i2c->i2cStatus == I2C_Bus_Free) {//send setup
		dev->status = HMC_Init;
        switch (dev->step) {
		case 0://setup sensor
			_i2c->addr = dev->addr;
			_i2c->rxtxp = pbuffer;
			_i2c->reg = HMC5883L_REG_CONFIG_A;
			_i2c->len = 3;
			_i2c->mode = I2C_MODE_WRITE;
            _i2c->rxtxp[0] = HMC5883L_SAMPLES_1 | HMC5883L_DATARATE_15HZ | HMC5883L_NORMAL;
            _i2c->rxtxp[1] = HMC5883L_GAIN_1_3GA;
            _i2c->rxtxp[2] = HMC5883L_CONTINOUS;
			dev->step = 1;
			break;

		case 1:
			dev->status = HMC_OK;
            dev->step = 0;
			return HMC_Complite;
			break;
		default:
			dev->step = 0;
            break;
		}
        I2C_Start_IRQ(_i2c);
	}
	return HMC_Processing;
}

HMC5883L_Connect_Status HMC5883L_GetData(I2C_Connection *_i2c, HMC5883L_dev *dev, uint8_t *pbuffer) {
	if (_i2c->i2cStatus == I2C_Bus_Free) {
		_i2c->rxtxp = pbuffer;
        switch (dev->step) {
		case 0://read data
			_i2c->addr = dev->addr;
			_i2c->reg = HMC5883L_REG_OUT_X_M;
			_i2c->len = 6;
			_i2c->mode = I2C_MODE_READ;
			dev->step = 1;
			break;

		case 1://calculate data
			dev->raw.X = (_i2c->rxtxp[0]<<8) | _i2c->rxtxp[1];
			dev->raw.Z = (_i2c->rxtxp[2]<<8) | _i2c->rxtxp[3];
			dev->raw.Y = (_i2c->rxtxp[4]<<8) | _i2c->rxtxp[5];
			dev->status = HMC_OK;
            dev->step = 0;
			return HMC_Complite;
			break;
		default:
			dev->step = 0;
            break;
		}
        I2C_Start_IRQ(_i2c);
	}
	return HMC_Processing;
}
