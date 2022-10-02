#include "card.h"
#include "gui.h"

Card::Card()
{

};

Card::Card(int count)
{
   CreateProblemsArray(count);
};

Card::~Card()
{

};

void Card::CreateProblemsArray(int count)
{
   problems.reset( new DescList[count + 1]);
};

void Card::ClearAndLoadTemplate()
{
    cardTemplate.clear();
    cardTemplate.load_bmp(templatePath.c_str());
};

void Card::AddEntry(std::string description)
{
    lastId++;
    problems[lastId].desc = description;
    problems[lastId].id = lastId;
    totalProblems++;
};

void Card::IncProblemsCount(int id)
{
    problems[id].nrOfProblems++;
    totalProblems++;
};

void Card::ChangeCardInfo(std::string client, std::string product, std::string ccnr, std::string step, std::string date, std::string person)
{
    info.client = client;
    info.product = product;
    info.controlCardNr = ccnr;
    info.step = step;
    info.date = date;
    info.person = person;
};

void Card::FillCard(std::string desc, int column, int row)
{
    unsigned char   black[] = {0, 0, 0};

    int             x = XstartPos + column * CARD_WIDTH, 
                    y = YstartPos + row * CARD_HEIGHT;

    cardTemplate.draw_text(x, y, info.client.c_str(), black, 0, 1, 53);
    cardTemplate.draw_text(x, y + nrYOffset, info.controlCardNr.c_str(), black, 0, 1, 53);
    cardTemplate.draw_text(x, y + descYOffset, desc.c_str(), black, 0, 1, 53);
    cardTemplate.draw_text(x, y + personYOffset, info.person.c_str(), black, 0, 1, 53);
    cardTemplate.draw_text(x + incardXOffset, y, info.product.c_str(), black, 0, 1, 53);
    cardTemplate.draw_text(x + incardXOffset, y + nrYOffset, info.step.c_str(), black, 0, 1, 53);
    cardTemplate.draw_text(x + incardXOffset, y + personYOffset, info.date.c_str(), black, 0, 1, 53);
}

void Card::SaveCurrentCard(std::string filename)
{
    cardTemplate.save_bmp(filename.c_str());
};

void Card::GenerateSummaryPage()
{
    unsigned char   black[] = {0, 0, 0};
    int y = 400;

    CImg summaryPage(2480, 3508);
    summaryPage.fill(255);
    summaryPage.draw_text(2480/3/2+100, 200, "RAPORT WADLIWYCH PRODUKTOW", black, 0, 1, 103);

    summaryPage.draw_text(200, y, "Klient:", black, 0, 1, 60);
    summaryPage.draw_text(1000, y, info.client.c_str(), black, 0, 1, 53);
    y += 70;

    summaryPage.draw_text(200, y, "Produkt:", black, 0, 1, 60);
    summaryPage.draw_text(1000, y, info.product.c_str(), black, 0, 1, 53);
    y += 70;

    summaryPage.draw_text(200, y, "Nr karty kontroli wyrobu:", black, 0, 1, 60);
    summaryPage.draw_text(1000, y, info.controlCardNr.c_str(), black, 0, 1, 53);
    y += 70;

    summaryPage.draw_text(200, y, "Etap produkcji:", black, 0, 1, 60);
    summaryPage.draw_text(1000, y, info.step.c_str(), black, 0, 1, 53);
    y += 70;

    summaryPage.draw_text(200, y, "Data:", black, 0, 1, 60);
    summaryPage.draw_text(1000, y, info.date.c_str(), black, 0, 1, 53);
    y += 70;

    summaryPage.draw_text(200, y, "Wadliwe produkty:", black, 0, 1, 60);
    summaryPage.draw_text(1000, y, std::to_string(totalProblems).c_str(), black, 0, 1, 53);
    y += 170;

    summaryPage.draw_text(200, y, "Ilosc", black, 0, 1, 70);
    summaryPage.draw_text(500, y, "Opis wady", black, 0, 1, 70);

    for(int i = 1, offset = y + 100; i <= lastId; i++)
    {
        summaryPage.draw_text(200, offset, std::to_string(problems[i].nrOfProblems).c_str(), black, 0, 1, 53);
        summaryPage.draw_text(500, offset, problems[i].desc.c_str(), black, 0, 1, 53);
        offset += 50;
    }
      std::string summaryOutputPath = outputPath;
      summaryOutputPath += "summary.bmp";
    summaryPage.save_bmp(summaryOutputPath.c_str());
};

bool Card::Generate(Gui* caller, double* dProgress, std::mutex* mPBar, bool* bStopGen)
{
   outputPath += "/";
   std::string outputPath_tmp = outputPath;
   ClearAndLoadTemplate();
   int         currentPage = 0,
               cardCounter = 0;
   bool        bStopThread = false;

   for(int j = 0, i = 1, x = 0, y = 0; j < totalProblems; j++)
   {
      caller->dispatcherGui.emit();
      {
         std::lock_guard<std::mutex> guard(*mPBar);
         *dProgress = j;
         bStopThread = *bStopGen;
      }
      if(bStopThread)
         return false;
      if(cardCounter > problems[i].nrOfProblems - 1)
      {
         i++;
         cardCounter = 0;
      }
      FillCard(problems[i].desc, x, y);
      y++;
      if(y == 3)
      {
         x++;
         y = 0;
      }
      if(x == 2)
      {
         outputPath_tmp += std::to_string(currentPage);
         outputPath_tmp += ".bmp";
         SaveCurrentCard(outputPath_tmp);
         ClearAndLoadTemplate();
         currentPage++;
         x = 0;
         y = 0;
         outputPath_tmp = outputPath;
      }
      if(j == totalProblems - 1)
      {
         outputPath_tmp = outputPath;
         outputPath_tmp += std::to_string(currentPage);
         outputPath_tmp += ".bmp";
         SaveCurrentCard(outputPath_tmp);
         ClearAndLoadTemplate();
         GenerateSummaryPage();
         break;
      }
      cardCounter++;
   }
   *dProgress+=2;
   caller->dispatcherGui.emit();
   return true;
}