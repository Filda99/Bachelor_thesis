/*
 * Shell.c
 *
 *  Created on: 04.08.2011
 *      Author: Erich Styger
 */

#include "Platform.h"
#include "Shell.h"
#include "CLS1.h"
#include "vl53l0x.h"

static const CLS1_ParseCommandCallback CmdParserTable[] =
{
  CLS1_ParseCommand,
  VL53L0X_ParseCommand,
  NULL /* sentinel */
};

void SHELL_Parse(void) {
  (void)CLS1_ReadAndParseWithCommandTable(CLS1_DefaultShellBuffer, sizeof(CLS1_DefaultShellBuffer), CLS1_GetStdio(), CmdParserTable);
}

void SHELL_Init(void) {
  CLS1_DefaultShellBuffer[0] = '\0';
}
