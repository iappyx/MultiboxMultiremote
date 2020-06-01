// Configure your boxes and remotes here. See readme on github and config.h for how the object is defined.

IthoBoxes boxesAndRemotes[] = {{

	"ITHO_1", "Badkamer beneden", "low", "none", 0, 0, 1,

	{{

			"00:00:00", "Afstandsbediening #1", {
			{"low", "Low", 0, (uint8_t[]){0x22, 0xf1, 0x03, 0x63, 0x02, 0x04}, 6}, 
			{"high", "High", 0, (uint8_t[]){0x22, 0xf1, 0x03, 0x63, 0x04, 0x04}, 6}, 
			{"auto1", "Auto 1", 0, (uint8_t[]){0x22, 0xf1, 0x03, 0x63, 0x03, 0x04}, 6}, 
			{"auto2", "Auto 2", 0, (uint8_t[]){0x22, 0xf8, 0x03, 0x63, 0x02, 0x03}, 6}, 
			{"timer1", "Timer 10 min", 10, (uint8_t[]){0x22, 0xf3, 0x03, 0x63, 0x80, 0x01}, 6}, 
			{"timer2", "Timer 20 min", 20, (uint8_t[]){0x22, 0xf3, 0x03, 0x63, 0x80, 0x02}, 6}, 
			{"timer3", "Timer 30 min", 30, (uint8_t[]){0x22, 0xf3, 0x03, 0x63, 0x80, 0x03}, 6}, 
			{NULL, NULL, 0, NULL, 0}
			}},
		{

			NULL, NULL, {{NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}}},
		{

			NULL, NULL, {{NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}}}

	}

},

{

	"ITHO_2", "Badkamer boven", "low", "none", 0, 0, 1,

	{{

			"00:00:00", "Afstandsbediening #2", {
			{"low", "Low", 0, (uint8_t[]){0x22, 0xf1, 0x03, 0x63, 0x02, 0x04}, 6}, 
			{"high", "High", 0, (uint8_t[]){0x22, 0xf1, 0x03, 0x63, 0x04, 0x04}, 6}, 
			{"auto1", "Auto 1", 0, (uint8_t[]){0x22, 0xf1, 0x03, 0x63, 0x03, 0x04}, 6}, 
			{"auto2", "Auto 2", 0, (uint8_t[]){0x22, 0xf8, 0x03, 0x63, 0x02, 0x03}, 6}, 
			{"timer1", "Timer 10 min", 10, (uint8_t[]){0x22, 0xf3, 0x03, 0x63, 0x80, 0x01}, 6}, 
			{"timer2", "Timer 20 min", 20, (uint8_t[]){0x22, 0xf3, 0x03, 0x63, 0x80, 0x02}, 6}, 
			{"timer3", "Timer 30 min", 30, (uint8_t[]){0x22, 0xf3, 0x03, 0x63, 0x80, 0x03}, 6}, 
			{NULL, NULL, 0, NULL, 0}
			}},
		{

			NULL, NULL, {{NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}}},
		{

			NULL, NULL, {{NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}, {NULL, NULL, 0, NULL, 0}}}

	}

}};