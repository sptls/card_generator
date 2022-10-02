#ifndef CARD_GUI_H_
#define CARD_GUI_H_

#include <gtkmm.h>
#include <stdio.h>
#include <thread>
#include <fstream>
#include "card.h"

#define CARD_LOAD_LIST 1
#define CARD_SAVE_LIST 2

#define CARD_GEN_VERSION "1.0"

class Gui
{
    public:
        Gui();
        Gui(char* listFile);
        void Init();
        void RemoveEntry();
        void HidePopupWindows();
        void AddEntryPopUp();
        void AddEntry();
        void EditEntryPopUp();
        void EditEntry();
        void CountErrors();
        void Generate();
        void CancelGenerate();
        void SetGenOutPath();
        void SetGenOutPath_response(int id, Gtk::FileChooserDialog* dialog);
        void UpdateGenProgress();
        void SaveList();
        void LoadList();
        void ListFileRW(int actionType);
        void SetListFilePath(int id, Gtk::FileChooserDialog* dialog, int actionType);
        bool CheckFileExists(std::string path);
        void SetTemplatePath();
        void SetTemplatePath_response(int id, Gtk::FileChooserDialog* dialog);

        Glib::RefPtr<Gtk::Builder>      builder;
        Gtk::Window*                    windowMain;
        Gtk::Window*                    windowAddEntry;
        Gtk::Window*                    windowEditEntry;
        Gtk::Window*                    windowGenProgress;

        Gtk::Button*                    buttonSave;
        Gtk::Button*                    buttonLoad;
        Gtk::Button*                    buttonAddToList;
        Gtk::Button*                    buttonRemoveFromList;
        Gtk::Button*                    buttonConfirmAdding;
        Gtk::Button*                    buttonCancelAdding;
        Gtk::Button*                    buttonEditListEntry;
        Gtk::Button*                    buttonCancelEdit;
        Gtk::Button*                    buttonConfirmEdit;
        Gtk::Button*                    buttonGenToFile;
        Gtk::Button*                    buttonGenAndPrint;
        Gtk::Button*                    buttonCancelGenInProgress;

        Gtk::Entry*                     entryDescPopup;
        Gtk::Entry*                     entryEditElement;
        Gtk::Entry*                     entryClient;
        Gtk::Entry*                     entryProduct;
        Gtk::Entry*                     entryCardControllNr;
        Gtk::Entry*                     entryStep;
        Gtk::Entry*                     entryDate;
        Gtk::Entry*                     entryPerson;

        Gtk::Label                      labelTest;
        Gtk::Label*                     labelTotalErrors;

        Gtk::LevelBar*                  levelbarProgress;

        Gtk::ListBoxRow                 row;
        Gtk::ListBox*                   listboxMain;

        int                             totalErrors = 0;
        int                             rowCount = 0;
        std::mutex                      mPBar;
        Glib::Dispatcher                dispatcherGui;
        std::thread*                    genThread = nullptr;
        double                          dGenProgress = 0;
        std::shared_ptr<Card>           c;
        std::string                     cardOutputPath;
        std::string                     listFilePath = "DUPA.cgen";
        bool                            bStopGen;
        std::string                     configPath;
        std::string                     templatePath;
};

#endif