#include "environment.h"

#include <iostream>
#include <string>
#include <regex>

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

void ClearConsole() {
    std::cout << "\x1B[2J\x1B[H" << std::endl;
}

void PressToContinue() {
    std::cout << "\nНажмите любую кнопку для продолжения ..."
              << std::endl;

    std::cin.get();
    std::cin.get();
}

void ShowMenu() {
    std::cout << " Меню:"
              << "\n 1.Добавить рейс"
              << "\n 2.Редактировать рейс"
              << "\n 3.Удалить рейс"
              << "\n 4.Найти рейс по типу самолета"
              << "\n 5.Показать все рейсы"
              << "\n 6.Сортировать список полетов"
              << "\n 7.Сохранить в базу данных"
              << "\n 8.Загрузить базу данных"
              << "\n 0.Выход"
              << std::endl;
}

int GetIntValue() {
    bool bFirst = true;

    bool bCheck = false;

    const std::regex regex("(\\+|-)?[[:digit:]]+");

    std::string strValue;

    do {
        if (bFirst) {
            bFirst = false;
        } else {
            std::cin.clear();

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Неверный ввод. Должно быть число." << std::endl;
        }

        strValue = GetStringValue();

        bCheck = regex_match(strValue, regex);

    } while (!bCheck);

    int iValue = atoi(strValue.c_str());

    return iValue;
}


std::string GetStringValue() {
    bool bFirst = true;
    std::string strValue;

    do {
        if (bFirst) {
            bFirst = false;
        } else {
            std::cin.clear();

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Не верный ввод. Должна быть строка.\n";
        }

        std::cin >> strValue;

    } while (!std::cin.good());

    return strValue;
}

ERRORS SaveToFile(const json &jArr, const std::string &strName) {
    if (jArr.empty() || strName.empty()) {
        return ERRORS::INVALID_ARGUMENT;
    }

//    const json strFileName = json::parse(strName);  // TODO
//    QFile saveFile(strName);

    std::ofstream file(strName);
    file << jArr;

//    if (!saveFile.open(QIODevice::WriteOnly)) {
//        return ERRORS::FILE_OPEN_ERROR;
//    }

//    QJsonDocument saveDoc(jArr);
//    saveFile.write(saveDoc.toJson());

    return ERRORS::NO_ERROR;
}

json LoadFromFile(const std::string &strName, ERRORS *pError) {
    if (strName.empty()) {
        if (pError != nullptr) {
            *pError = ERRORS::INVALID_ARGUMENT;
        }

        return {};
    }

    std::ifstream ifs(strName);
    json jf = json::parse(ifs);


//    const QString strFileName = QString::fromStdString(strName);
//    QFile loadFile(strFileName);

//    if (!loadFile.open(QIODevice::ReadOnly)) {
//        if (pError != nullptr) {
//            *pError = ERRORS::FILE_OPEN_ERROR;
//        }

//        return {};
//    }
//
//    QByteArray arrData = loadFile.readAll();
//
//    QJsonDocument loadDoc = QJsonDocument::fromJson(arrData);
//
//    if (pError != nullptr) {
//        *pError = ERRORS::NO_ERROR;
//    }

    return jf;
}