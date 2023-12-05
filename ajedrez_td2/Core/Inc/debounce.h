#ifndef INC_DEBOUNCE_H_
#define INC_DEBOUNCE_H_
#include <stdint.h>
typedef enum {
	BTN_LIBRE = 0, BTN_PRESIONADO
} Debounce;
typedef struct {
	uint16_t debounce_ticks;
	uint16_t debounce;
	uint8_t estado;
	uint8_t estado_pin;
	uint8_t estado_anterior_pin;
	uint8_t activo_en_cero;
} Debounce_t;

void debounce_init(Debounce_t *s, uint8_t activo_en_cero, uint16_t debounce);
void debounce_run(Debounce_t *s, uint32_t estado);
int debounce_flank(Debounce_t *s);

#endif /* INC_DEBOUNCE_H_ */
