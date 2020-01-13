/*
 * Copyright (c) 2017 Bernd Walter
 * All rights reserved.
 *
 * $URL$
 * $Date$
 * $Author$
 * $Rev$
 */

#include "asf.h"
#include "FreeRTOS.h"
#include "task.h"
#include <strings.h>
#include <main.h>

void processgcode();

struct {
	uint32_t hasA : 1;
	uint32_t hasB : 1;
	uint32_t hasC : 1;
	uint32_t hasD : 1;
	uint32_t hasE : 1;
	uint32_t hasF : 1;
	uint32_t hasG : 1;
	uint32_t hasH : 1;
	uint32_t hasI : 1;
	uint32_t hasJ : 1;
	uint32_t hasK : 1;
	uint32_t hasL : 1;
	uint32_t hasM : 1;
	uint32_t hasO : 1;
	uint32_t hasP : 1;
	uint32_t hasR : 1;
	uint32_t hasS : 1;
	uint32_t hasT : 1;
	uint32_t hasX : 1;
	uint32_t hasY : 1;
	uint32_t hasZ : 1;
	uint16_t G;
	uint16_t M;
	float A;
	float B;
	float C;
	float D;
	float E;
	float F;
	float H;
	float I;
	float J;
	float K;
	float L;
	float O;
	long P;
	float R;
	long S;
	long T;
	float X;
	float Y;
	float Z;
} gd;

static float parseFloatValue(const char *s)
{
	char *endPtr;
	while (*s == ' ') {
		s++;
	}
	float f = (strtod(s, &endPtr));
	if (s == endPtr) {
		f = 0.0;
	}
	return f;
}

static long parseLongValue(const char *s)
{
	char *endPtr;
	while (*s == ' ') {
		s++;
	}
	long l = (strtol(s, &endPtr, 10));
	if (s == endPtr) {
		l = 0;
	}
	return l;
}

void
parsegcode(const char* data)
{
	const char *pos = data;
	char c;
	bzero(&gd, sizeof(gd));

	while ( (c = *(pos++)) ) {
		if (c == '(' || c == '%') break; // alternative comment or program block
		switch(c) {
		case 'A':
		case 'a':
			gd.hasA = true;
			gd.A = parseFloatValue(pos);
			break;
		case 'B':
		case 'b':
			gd.hasB = true;
			gd.B = parseFloatValue(pos);
			break;
		case 'C':
		case 'c':
			gd.hasC = true;
			gd.C = parseFloatValue(pos);
			break;
		case 'D':
		case 'd':
			gd.hasD = true;
			gd.D = parseFloatValue(pos);
			break;
		case 'E':
		case 'e':
			gd.hasE = true;
			gd.E = parseFloatValue(pos);
			break;
		case 'F':
		case 'f':
			gd.hasF = true;
			gd.F = parseFloatValue(pos);
			break;
		case 'G':
		case 'g':
			gd.hasG = true;
			gd.G = parseLongValue(pos) & 0xffff;
			break;
		case 'H':
		case 'h':
			gd.hasH = true;
			gd.H = parseFloatValue(pos);
			break;
		case 'I':
		case 'i':
			gd.hasI = true;
			gd.I = parseFloatValue(pos);
			break;
		case 'J':
		case 'j':
			gd.hasJ = true;
			gd.J = parseFloatValue(pos);
			break;
		case 'K':
		case 'k':
			gd.hasK = true;
			gd.K = parseFloatValue(pos);
			break;
		case 'L':
		case 'l':
			gd.hasL = true;
			gd.L = parseFloatValue(pos);
			break;
		case 'M':
		case 'm':
			gd.hasM = true;
			gd.M = parseLongValue(pos) & 0xffff;
			break;
		case 'O':
		case 'o':
			gd.hasO = true;
			gd.O = parseFloatValue(pos);
			break;
		case 'P':
		case 'p':
			gd.hasP = true;
			gd.P = parseLongValue(pos);
			break;
		case 'R':
		case 'r':
			gd.hasR = true;
			gd.R = parseFloatValue(pos);
			break;
		case 'S':
		case 's':
			gd.hasS = true;
			gd.S = parseLongValue(pos);
			break;
		case 'T':
		case 't':
			gd.hasT = true;
			gd.T = parseLongValue(pos);
			break;
		case 'X':
		case 'x':
			gd.hasX = true;
			gd.X = parseFloatValue(pos);
			break;
		case 'Y':
		case 'y':
			gd.hasZ = true;
			gd.Z = parseFloatValue(pos);
			break;
		case 'Z':
		case 'z':
			gd.hasZ = true;
			gd.Z = parseFloatValue(pos);
			break;
		}
	}

	processgcode();
}

void
processgcode()
{
	const int delaytime = 50;

	if (gd.hasG) {
		switch(gd.G) {
		case 4:
			if (gd.hasP) {
				vTaskDelay(gd.P / portTICK_PERIOD_MS);
			}
			if (gd.hasS) {
				vTaskDelay(gd.S * 1000 / portTICK_PERIOD_MS);
			}
			break;
		}
		
	} else if (gd.hasM) {
		switch(gd.M) {
		case 42:
			// P=feeder S=lane
			if (gd.hasP && gd.hasS) {
				if (gd.S >= 0 && gd.S <= 1) {
					setpin(gd.P * 2 + gd.S, 1);
					set_off_timer(gd.P * 2 + gd.S, delaytime);
				}
			}
			break;
		}
	}
}

