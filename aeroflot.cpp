#include "aeroflot.h"

#include <iostream>
#include <iomanip>
#include <regex>
#include <tuple>
#include <cmath>

#include <nlohmann/json.hpp>

#include "customlinkedlist.h"
#include "environment.h"
#include "errors.h"

constexpr int FLIGHT_NUMBER_PADDING = 15;
constexpr int DESTINATION_PADDING = 25;
constexpr int AIRCRAFT_PADDING = 15;
constexpr int ADDITION_HEAD_TABLE = 2;

using json = nlohmann::json;

void PrintLine(size_t nSize) {
    std::cout << std::setfill('-')
              << std::setw(static_cast<int>(nSize))
              << ""
              << std::setfill(' ')
              << "";
}

ERRORS PrintFlightList(const LinkedList<AEROFLOT> &arrFlight) {
    if (arrFlight.GetSize() == 0) {
        return ERRORS::OBJ_EMPTY;
    }

    const LinkedList<AEROFLOT>::Node *pHead = arrFlight.GetHead();

    LinkedList<AEROFLOT>::Node *pNode = const_cast<LinkedList<AEROFLOT>::Node *>(pHead);

    constexpr size_t nLineSize = FLIGHT_NUMBER_PADDING
                                 + DESTINATION_PADDING
                                 + AIRCRAFT_PADDING
                                 + ADDITION_HEAD_TABLE;

    std::cout << " ";
    PrintLine(nLineSize);
    std::cout << '\n';

    std::cout << '|' << std::setw(FLIGHT_NUMBER_PADDING) << "Номер рейса"
              << '|' << std::setw(DESTINATION_PADDING) << "Пункт назначения"
              << '|' << std::setw(AIRCRAFT_PADDING) << "Тип самолета"
              << '|';

    std::cout << "\n ";
    PrintLine(nLineSize);

    while (pNode != nullptr) {
        const AEROFLOT &flight = pNode->m_value;

        std::cout << '\n'
                  << '|' << std::setw(FLIGHT_NUMBER_PADDING) << flight.nFlightNumber
                  << '|' << std::setw(DESTINATION_PADDING) << flight.strDestination
                  << '|' << std::setw(AIRCRAFT_PADDING) << flight.strAircraftType
                  << '|';

        pNode = pNode->m_pNext;
    }

    std::cout << "\n ";
    PrintLine(nLineSize);

    return ERRORS::NO_ERROR;
}

json AeroflotListToJson(const LinkedList<AEROFLOT> &arrFlight) {
    json jFlightsArray;

    const LinkedList<AEROFLOT>::Node *pNode = arrFlight.GetHead();

    while (pNode != nullptr) {
        const json jAeroflot = AeroflotToJson(pNode->m_value);

        jFlightsArray.push_back(jAeroflot);

        pNode = pNode->m_pNext;
    }

    return jFlightsArray;
}

LinkedList<AEROFLOT> *JsonToAeroflotList(const json &jArr, ERRORS *pError) {
    if (jArr.empty()) {
        if (pError != nullptr) {
            *pError = ERRORS::OBJ_EMPTY;
        }

        return nullptr;
    }

    LinkedList<AEROFLOT> *pAeroList = new LinkedList<AEROFLOT>();

    if (pError != nullptr) {
        *pError = ERRORS::NO_ERROR;
    }

    for (const json &jVal : jArr) {

        if (!jVal.contains("Номер рейса")
            || !jVal.contains("Пункт назначения")
            || !jVal.contains("Тип самолета")) {
            if (pError != nullptr) {
                *pError = ERRORS::JSON_PART_BROKEN;
            }

            continue;
        }


        AEROFLOT flight;
        flight.nFlightNumber = jVal.at("Номер рейса");
        flight.strDestination = jVal.at("Пункт назначения");
        flight.strAircraftType = jVal.at("Тип самолета");

        pAeroList->PushFront(flight);
    }

    return pAeroList;
}

json AeroflotToJson(const AEROFLOT &flight) {

    return {
            {"Пункт назначения", flight.strDestination},
            {"Номер рейса",      flight.nFlightNumber},
            {"Тип самолета",     flight.strAircraftType}
    };
}

bool PredicateSortByDestination(const AEROFLOT &flight1, const AEROFLOT &flight2) {
    return flight1.strDestination.compare(flight2.strDestination) < 0;
}

bool PredicateSortByFlightNumber(const AEROFLOT &flight1, const AEROFLOT &flight2) {
    return flight1.nFlightNumber < flight2.nFlightNumber;
}

bool PredicateSortByAirType(const AEROFLOT &flight1, const AEROFLOT &flight2) {
    return flight1.strAircraftType.compare(flight2.strAircraftType) < 0;
}

bool CheckAircraftType(const std::string &strAircraftType) {
    const std::regex airRegex("[A-Z][0-9]{4}");

    return regex_match(strAircraftType, airRegex);
}

bool CheckDestination(const std::string &strDestination) {
    const std::regex airRegex("[A-Za-z]{1,25}");

    return regex_match(strDestination, airRegex);
}

bool CheckNumber(int iFlightNumber) {
    if (iFlightNumber < 1) {
        return false;
    }

    const int iCount = std::log10(iFlightNumber) + 1;

    return iCount == 4;
}

AEROFLOT EnterFlight() {
    AEROFLOT flight;

    std::string strTemp;
    bool bFirst = true;

    std::cout << "Введите аэропорт назначения: ";
    do {
        if (bFirst) {
            bFirst = false;
        } else {
            std::cout << "Пункт назначения должен быть не больше 25 символов" << std::endl;
        }

        strTemp = GetStringValue();
    } while (!CheckDestination(strTemp));

    flight.strDestination = strTemp;


    bFirst = true;
    std::cout << "Введите тип самолета: ";
    do {
        if (bFirst) {
            bFirst = false;
        } else {
            std::cout << "Тип самолета должен быть из одной заглавной буквы и 4 цифр" << std::endl;
        }

        strTemp = GetStringValue();
    } while (!CheckAircraftType(strTemp));

    flight.strAircraftType = strTemp;


    int iNumber = 0;
    bFirst = true;
    std::cout << "Введите номер рейса: ";
    do {
        if (bFirst) {
            bFirst = false;
        } else {
            std::cout << "Номер рейса должен быть из 4 цифр" << std::endl;
        }

        iNumber = GetIntValue();
    } while (!CheckNumber(iNumber));

    flight.nFlightNumber = iNumber;


    return flight;
}

bool operator==(const AEROFLOT &lhs, const AEROFLOT &rhs) {
    return std::make_tuple(lhs.nFlightNumber, lhs.strDestination, lhs.strAircraftType)
           == std::make_tuple(rhs.nFlightNumber, rhs.strDestination, rhs.strAircraftType);
}

LinkedList<AEROFLOT> *FindByAircraft(LinkedList<AEROFLOT> *pList, const std::string &strFindAir, ERRORS *pError) {
    if (pList == nullptr || strFindAir.empty()) {
        if (pError != nullptr) {
            *pError = ERRORS::INVALID_ARGUMENT;
        }

        return nullptr;
    }

    LinkedList<AEROFLOT> *const pResult = new LinkedList<AEROFLOT>();

    const LinkedList<AEROFLOT>::Node *pNode = pList->GetHead();

    while (pNode != nullptr) {
        const AEROFLOT &flight = pNode->m_value;

        const std::string &strAirCraft = flight.strAircraftType;

        if (strAirCraft == strFindAir) {
            pResult->PushFront(flight);
        }

        pNode = pNode->m_pNext;
    }

    if (pError != nullptr) {
        *pError = ERRORS::NO_ERROR;
    }

    return pResult;
}

void EditFlight(AEROFLOT &flight) {
    int iNumParam = 1;

    while (iNumParam >= 1 && iNumParam <= 3) {
        std::cout << "Введите номер параметра:"
                  << "\n 1. Пункт назначения"
                  << "\n 2. Номер рейса"
                  << "\n 3. Тип самолета"
                  << "\n Любое другое число - сохранить"
                  << std::endl;

        iNumParam = GetIntValue();

        switch (iNumParam) {
            case 1:
                EditDestination(flight.strDestination);
                break;
            case 2:
                EditFlightNumber(flight.nFlightNumber);
                break;
            case 3:
                EditAircraftType(flight.strAircraftType);
                break;
        }

        std::cout << '\n' << std::endl;
    }

}

void EditDestination(std::string &strDestination) {

    bool bFirst = true;
    std::string strTemp;

    std::cout << "Введите новый аэропорт назначения: ";
    do {
        if (bFirst) {
            bFirst = false;
        } else {
            std::cout << "Пункт назначения должен быть не больше 25 символов" << std::endl;
        }

        strTemp = GetStringValue();
    } while (!CheckDestination(strTemp));

    strDestination = strTemp;
}

void EditFlightNumber(size_t &nFlightNumber) {
    bool bFirst = true;

    int iNumber = 0;

    std::cout << "Введите новый номер рейса: ";
    do {
        if (bFirst) {
            bFirst = false;
        } else {
            std::cout << "Номер рейса должен быть из 4 цифр" << std::endl;
        }

        iNumber = GetIntValue();
    } while (!CheckNumber(iNumber));

    nFlightNumber = iNumber;
}

void EditAircraftType(std::string &strAircraftType) {
    bool bFirst = true;
    std::string strTemp;

    std::cout << "Введите тип самолета: ";

    do {
        if (bFirst) {
            bFirst = false;
        } else {
            std::cout << "Тип самолета должен быть из одной заглавной буквы и 4 цифр" << std::endl;
        }

        strTemp = GetStringValue();
    } while (!CheckAircraftType(strTemp));

    strAircraftType = strTemp;
}