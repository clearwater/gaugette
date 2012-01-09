/*
 Copyright (c) 2011 Guy Carpenter
 */

#include <Arduino.h>
#include "Command.h"

#define CMDBUF_LEN 40


void Command::skipWhitespace(char **line, int *len)
{
  while (*len > 0 && **line == ' ') {
    (*len)--;
    (*line)++;
  }
}


boolean Command::parseCommand(char **line, int *len, char *cmd)
{
  boolean result = false;
  skipWhitespace(line, len);
  if (*len > 0) {
    *cmd = **line;
    (*len)--;
    (*line)++;
    result = true;
  }
  return result;
}


boolean Command::parseInteger(char **line, int *len, value_t *value)
{
  boolean result = false;
  *value = 0;
  while (*len>0 && **line>='0' && **line<='9') {
    *value = *value * 10 + (**line - '0');
    (*len)--;
    (*line)++;
    result = true;
  }
  return result;
}

boolean Command::parseTuple(char **line, int *len, value_t *tuple)
{
  boolean result = false;
  skipWhitespace(line, len);
  for (int i=0; i<COMMAND_TUPLE_LEN; i++) {
    if (parseInteger(line, len, tuple+i+1)) {
      result = true;
      tuple[0] = i+1;
      if (*len>0 && **line == ',') {
        // found comma, so more integers expected
        (*len)--;
        (*line)++;
        result = false;  // need another int to succeed
      } 
      else {
        break;
      }
    } 
    else {
      break;
    }
  }        
  return result;
}

boolean Command::parseLine(char *line, int len)
{
  boolean result = false;
  if (parseCommand(&line, &len, &command) &&
    parseTuple(&line, &len, address) &&
    parseTuple(&line, &len, value)) {
    result = true;
  } 
  else {
    Serial.println("$Error Invalid command syntax");
  }
  return result;
}


boolean Command::parseInput()
{
  boolean result = false;
  static char line[CMDBUF_LEN];  // NOT zero terminated
  static int len = 0;

  if (Serial.available()) {
    char c = Serial.read();
    if (c==10 || c==13) {
      result = parseLine(line, len);
      len = 0;
    } 
    else if (len < CMDBUF_LEN) {
      line[len++] = c;
    }
    return result;
  }
}

void Command::dump()
{
  Serial.print(command);
  Serial.print(" ");
  for (int i=0;i<address[0];i++) {
    if (i>0) Serial.print(",");
    Serial.print(address[i+1]);
  }
  Serial.print(" ");
  for (int i=0;i<value[0];i++) {
    if (i>0) Serial.print(",");
    Serial.print(value[i+1]);
  }
  Serial.println();
}


