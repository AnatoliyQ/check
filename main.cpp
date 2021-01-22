#include <iostream>
#include "customlinkedlist.h"
#include "aeroflot.h"
#include "environment.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    size_t nCommand = 1;

    LinkedList<AEROFLOT> *pFlights = new LinkedList<AEROFLOT>();

    while (nCommand != 0) {
        ClearConsole();

        ShowMenu();

        nCommand = GetIntValue();

        ClearConsole();

        switch (nCommand) {
            case 1: {
                std::cout << "Добавить рейс\n\n";

                const AEROFLOT flight = EnterUnicFlight(*pFlights);

                const LinkedList<AEROFLOT>::Node *const pNode = pFlights->Find(flight);

                if (pNode != nullptr) {
                    std::cout << "Ошибка: рейс уже добавлен";
                } else {
                    pFlights->PushFront(flight);
                }
            }
                break;
            case 2: {
                std::cout << "Редактировать рейс\n\n";

                std::cout << "Поиск рейса для редактирования:\n";

                AEROFLOT flight = EnterFlight();

                LinkedList<AEROFLOT>::Node *const pNode = pFlights->Find(flight);

                if (pNode != nullptr) {
                    EditFlight(flight, pFlights);

                    const LinkedList<AEROFLOT>::Node *const pFindEqual = pFlights->Find(flight);

                    bool bUniq = true;

                    if (pFindEqual != nullptr) {
                        if (pFindEqual != pNode) {
                            bUniq = false;
                        }
                    }

                    if (bUniq) {
                        pNode->m_value = flight;

                        std::cout << "Успешно отредактировно";
                    } else {
                        std::cout << "Ошибка: рейс отсутствует. Изменения отклонены";
                    }

                } else {
                    std::cout << "Ошибка: рейс не найден";
                }
            }
                break;
            case 3: {
                std::cout << "Удаление рейса\n\n";

                const AEROFLOT flight = EnterFlight();

                LinkedList<AEROFLOT>::Node *const pNode = pFlights->Find(flight);

                if (pNode == nullptr) {
                    std::cout << "Ошибка: рейс не найден";
                } else {
                    pFlights->Remove(pNode);
                }
            }
                break;
            case 4: {
                std::cout << "Поиск рейса по самолету\n\n";

                std::string strAircraft;

                bool bFirst = true;
                std::cout << "Введите тип самолета:";
                do {
                    if (bFirst) {
                        bFirst = false;
                    } else {
                        std::cout << "Тип самолета должен содержать одну заглавную букву и 4 цифры"
                                  << std::endl;
                    }

                    strAircraft = GetStringValue();
                } while (!CheckAircraftType(strAircraft));


                LinkedList<AEROFLOT> *pFinded = FindByAircraft(pFlights, strAircraft);

                const ERRORS err = PrintFlightList(*pFinded);

                if (err == ERRORS::OBJ_EMPTY) {
                    std::cout << "Ошибка: не найдено";
                }

                if (pFinded != nullptr) {
                    delete pFinded;
                }
            }
                break;
            case 5: {
                std::cout << "Отображение списка рейсов\n\n";

                const ERRORS err = PrintFlightList(*pFlights);

                if (err == ERRORS::OBJ_EMPTY) {
                    std::cout << "Ошибка: список пустой";
                }
            }
                break;
            case 6: {
                std::cout << "Сортировка списка полетов\n\n";

                std::cout << "Сортировать по :"
                          << "\n 1. Пункт назначения"
                          << "\n 2. Номер рейса"
                          << "\n 3. Тип самолета"
                          << "\n Любая другая цифра - без сортировки"
                          << std::endl;

                int iSort = GetIntValue();

                switch (iSort) {
                    case 1:
                        pFlights->SortBySelection(PredicateSortByDestination);
                        break;
                    case 2:
                        pFlights->SortBySelection(PredicateSortByFlightNumber);
                        break;
                    case 3:
                        pFlights->SortBySelection(PredicateSortByAirType);
                        break;
                }
            }
                break;
            case 7: {
                std::cout << "Сохранить в базу данных\n\n";

                const json jArrFlights = AeroflotListToJson(*pFlights);

                std::cout << "Введите имя файла с расширением: ";

                const std::string strName = GetStringValue();

                const ERRORS errSave = SaveToFile(jArrFlights, strName);

                if (errSave == ERRORS::FILE_OPEN_ERROR) {
                    std::cout << "Ошибка: невозможно открыть файл";
                } else if (errSave == ERRORS::INVALID_ARGUMENT) {
                    std::cout << "Ошибка: список пустой или неправильное имя файла";
                } else {
                    std::cout << "Успешно сохранено";
                }
            }
                break;
            case 8: {
                std::cout << "Загрузка базы данных\n\n";

                std::cout << "Введите имя файла с расширением: ";

                const std::string strName = GetStringValue();

                ERRORS errLoad;

                const json jArrFlights = LoadFromFile(strName, &errLoad);

                if (errLoad == ERRORS::INVALID_ARGUMENT) {
                    std::cout << "Ошибка: неправильное имя файла";
                } else if (errLoad == ERRORS::FILE_OPEN_ERROR) {
                    std::cout << "Ошибка: невозможно открыть файл";
                } else {
                    LinkedList<AEROFLOT> *pNewFlights = JsonToAeroflotList(jArrFlights);

                    if (pNewFlights == nullptr) {
                        std::cout << "Ошибка: файл невалидный";
                    } else {
                        if (pFlights != nullptr) {
                            delete pFlights;
                        }

                        pFlights = pNewFlights;

                        std::cout << "Успешно загружено";
                    }
                }
            }
                break;
            case 0:
                std::cout << "Выход\n" << std::endl;
                break;
            default:
                std::cout << "Ошибка: неизвестная команда";
                break;
        }

        PressToContinue();
    }

    if (pFlights != nullptr) {
        delete pFlights;
    }

    return 0;
}
