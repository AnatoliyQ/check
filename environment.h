#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include "errors.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

typedef __SIZE_TYPE__ size_t;

void ShowMenu();

void PressToContinue();

void ClearConsole();

int GetIntValue();

std::string GetStringValue();

ERRORS SaveToFile(const json &jArr, const std::string &strName);

json LoadFromFile(const std::string &strName, ERRORS *pError = nullptr);

#endif // ENVIRONMENT_H
