#ifndef CARD_H_
#define CARD_H_

#include <stdio.h>
#include <string>
#include <cstdlib>
#include <mutex>
#include <memory>
#define cimg_display 0
#include "CImg.h"

#if defined(__linux__)
    #define CLSSCR system("clear")
#else
    #define CLSSCR system("cls")
#endif

using namespace cimg_library;

#define CARD_HEIGHT 826
#define CARD_WIDTH 1754

class Gui;

/*
* Page size in pixels: 3508x2480
* 
* 2480/3 = 826.666666
* 3508/2 = 1754
*/

struct OtherInfo
{
    std::string             client;
    std::string             product;
    std::string             controlCardNr;
    std::string             step;
    std::string             date;
    std::string             person;
};

struct DescList
{
    std::string             desc;
    int                     id;
    int                     nrOfProblems = 1;
};

class Card
{
    private:
        CImg<uint8_t>       cardTemplate;
        int                 XstartPos = 55, 
                            YstartPos = 190,
                            nextCardXOffset = 1754,
                            nextCardYOffset = 826,
                            nrYOffset = 135,
                            descYOffset = 310,
                            personYOffset = 440,
                            incardXOffset = 825;

    public:
        std::shared_ptr<DescList[]>            problems;
        OtherInfo           info;
        int                 lastId = 0,
                            totalProblems = 0;
        uint8_t             *data_cardTemplate;
        std::string         outputPath;
        std::string         templatePath;
        Card();
        Card(int count);
        ~Card();
        void CreateProblemsArray(int count);
        void ClearAndLoadTemplate();
        void AddEntry(std::string description);
        void IncProblemsCount(int id);
        void ChangeCardInfo(std::string client, std::string product, std::string ccnr, std::string step, std::string date, std::string person);
        void FillCard(std::string desc, int column, int row);
        void SaveCurrentCard(std::string filename);
        void GenerateSummaryPage();
        bool Generate(Gui* caller, double* dProgress, std::mutex* mPBar, bool* bStopGen);
};


#endif