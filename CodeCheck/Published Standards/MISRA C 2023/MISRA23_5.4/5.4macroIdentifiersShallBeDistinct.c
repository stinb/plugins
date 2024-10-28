/* 		1234567890123456789012345678901********* 		Characters */
#define engine_exhaust_gas_temperature_raw egt_r
#define engine_exhaust_gas_temperature_scaled egt_s /* UndCC_Violation */

/* 		1234567890123456789012345678901********* 		Characters */
#define engine_exhaust_gas_temp_raw egt_r
#define engine_exhaust_gas_temp_scaled egt_s /* 		Compliant */

#define min(X, Y)  ((X) < (Y) ? (X) : (Y)) /*			Compliant */
/* Below is non compliant */
#define max(engine_exhaust_gas_temperature_X, engine_exhaust_gas_temperature_Y)  ((engine_exhaust_gas_temperature_X) > (engine_exhaust_gas_temperature_Y) ? (engine_exhaust_gas_temperature_Y) : (engine_exhaust_gas_temperature_X)) // UndCC_Violation
