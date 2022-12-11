#pragma once

#ifndef F_CPU
	# define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>

void creerDelaiMs(uint16_t delai);
void creerDelaiUs(uint16_t delai);