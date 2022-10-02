#include "gui.h"

Gui::Gui()
{
    Init();
};

Gui::Gui(char* listFile)
{
    Init();
    listFilePath = listFile;
    LoadList();
};

void Gui::Init()
{
    builder = Gtk::Builder::create_from_resource("/gui/gui.glade");
    builder->get_widget("windowMain", windowMain);
    builder->get_widget("windowAddEntry", windowAddEntry);
    builder->get_widget("windowEditEntry", windowEditEntry);
    builder->get_widget("windowGenProgress", windowGenProgress);

    builder->get_widget("buttonSave", buttonSave);
    builder->get_widget("buttonLoad", buttonLoad);
    builder->get_widget("buttonAddToList", buttonAddToList);
    builder->get_widget("buttonRemoveFromList", buttonRemoveFromList);
    builder->get_widget("buttonConfirmAdding", buttonConfirmAdding);
    builder->get_widget("buttonCancelAdding", buttonCancelAdding);
    builder->get_widget("buttonEditListEntry", buttonEditListEntry);
    builder->get_widget("buttonCancelEdit", buttonCancelEdit);
    builder->get_widget("buttonConfirmEdit", buttonConfirmEdit);
    builder->get_widget("buttonGenToFile", buttonGenToFile);
    builder->get_widget("buttonGenAndPrint", buttonGenAndPrint);
    builder->get_widget("buttonCancelGenInProgress", buttonCancelGenInProgress);

    builder->get_widget("entryDescPopup", entryDescPopup);
    builder->get_widget("entryEditElement", entryEditElement);
    builder->get_widget("entryClient", entryClient);
    builder->get_widget("entryProduct", entryProduct);
    builder->get_widget("entryCardControllNr", entryCardControllNr);
    builder->get_widget("entryStep", entryStep);
    builder->get_widget("entryDate", entryDate);
    builder->get_widget("entryPerson", entryPerson);

    builder->get_widget("labelTotalErrors", labelTotalErrors);

    builder->get_widget("levelbarProgress", levelbarProgress);

    builder->get_widget("listboxMain", listboxMain);

    buttonSave->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Gui::ListFileRW), CARD_SAVE_LIST));
    buttonLoad->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Gui::ListFileRW), CARD_LOAD_LIST));
    buttonRemoveFromList->signal_clicked().connect(sigc::mem_fun(*this, &Gui::RemoveEntry));
    buttonCancelAdding->signal_clicked().connect(sigc::mem_fun(*this, &Gui::HidePopupWindows));
    buttonAddToList->signal_clicked().connect(sigc::mem_fun(*this, &Gui::AddEntryPopUp));
    buttonConfirmAdding->signal_clicked().connect(sigc::mem_fun(*this, &Gui::AddEntry));
    buttonCancelEdit->signal_clicked().connect(sigc::mem_fun(*this, &Gui::HidePopupWindows));
    buttonEditListEntry->signal_clicked().connect(sigc::mem_fun(*this, &Gui::EditEntryPopUp));
    buttonConfirmEdit->signal_clicked().connect(sigc::mem_fun(*this, &Gui::EditEntry));
    buttonGenToFile->signal_clicked().connect(sigc::mem_fun(*this, &Gui::SetGenOutPath));
    buttonCancelGenInProgress->signal_clicked().connect(sigc::mem_fun(*this, &Gui::CancelGenerate));

    dispatcherGui.connect(sigc::mem_fun(*this, &Gui::UpdateGenProgress));

// FOR TESTING ONLY
/*
    for(int i = 0; i < 10; i++)
    {
        Gtk::SpinButton* count = Gtk::manage(new Gtk::SpinButton(Gtk::Adjustment::create(1.0, 1.0, 9999.0)));
        Gtk::Label* labelDesc = Gtk::manage(new Gtk::Label("description"));
        Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());
        grid->set_border_width(2);
        grid->set_column_spacing(20);
        grid->attach(*count, 0, 0, 2);
        grid->attach(*labelDesc, 3, 0, 3);
        Gtk::ListBoxRow* r = Gtk::manage(new Gtk::ListBoxRow());
        r->add(*grid);
        listboxMain->append(*r);
    }
*/
    std::string tmpV = "Card Generator v";
    tmpV += CARD_GEN_VERSION;
    windowMain->set_title(tmpV);
    windowMain->show_all();

    if(CheckFileExists("config"))
    {
        std::ifstream f("config");
        getline(f, templatePath);
        if(!(CheckFileExists(templatePath)))
            SetTemplatePath();
        f.close();
    }
    else
        SetTemplatePath();
};


void Gui::RemoveEntry()
{
    Gtk::ListBoxRow* row = listboxMain->get_selected_row();
    if(row != nullptr)
        listboxMain->remove(*row);

    CountErrors();
};

void Gui::HidePopupWindows()
{
    entryDescPopup->set_text("");
    windowAddEntry->hide();
    windowEditEntry->hide();
    windowGenProgress->hide();
};

void Gui::AddEntryPopUp()
{
    windowAddEntry->show();
};

void Gui::AddEntry()
{
    Gtk::SpinButton* count = Gtk::manage(new Gtk::SpinButton(Gtk::Adjustment::create(1.0, 1.0, 9999.0)));
    count->signal_value_changed().connect(sigc::mem_fun(*this, &Gui::CountErrors));
    Gtk::Label* labelDesc = Gtk::manage(new Gtk::Label(entryDescPopup->get_text()));
    Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());
    grid->set_border_width(2);
    grid->set_column_spacing(20);
    grid->attach(*count, 0, 0, 2);
    grid->attach(*labelDesc, 3, 0, 3);
    Gtk::ListBoxRow* r = Gtk::manage(new Gtk::ListBoxRow());
    r->add(*grid);
    listboxMain->append(*r);
    windowMain->show_all();

    windowAddEntry->hide();
    entryDescPopup->set_text("");
    CountErrors();
};

void Gui::EditEntryPopUp()
{
    Gtk::ListBoxRow* row = listboxMain->get_selected_row();
    if(row == nullptr)
        return;
    Gtk::Grid* grid = (Gtk::Grid*)(row->get_child());
    Gtk::Label* lab = (Gtk::Label*)(grid->get_child_at(3, 0));
    if(lab == nullptr)
    {
        printf("Label from listboxrow returned nullptr!\n");
        fflush(stdout);
        return;
    }
    entryEditElement->set_text(lab->get_text());
    windowEditEntry->show();
};

void Gui::EditEntry()
{
    Gtk::ListBoxRow* row = listboxMain->get_selected_row();
    Gtk::Grid* grid = (Gtk::Grid*)(row->get_child());
    Gtk::Label* lab = (Gtk::Label*)(grid->get_child_at(3, 0));
    if(lab == nullptr)
    {
        printf("Label from listboxrow returned nullptr!\n");
        fflush(stdout);
        return;
    }
    lab->set_text(entryEditElement->get_text());
    entryEditElement->set_text("");
    HidePopupWindows();
};

void Gui::CountErrors()
{
    int counter = 0;
    rowCount = 0;
    for(auto* i: listboxMain->get_children())
    {
        Gtk::Grid* grid = (Gtk::Grid*)(((Gtk::ListBoxRow*)i)->get_child());
        Gtk::SpinButton* sb = (Gtk::SpinButton*)(grid->get_child_at(0, 0));
        double val = sb->get_value();
        counter += (int)val;
        rowCount++;
    }
    totalErrors = counter;
    labelTotalErrors->set_text(std::to_string(totalErrors));
};

void Gui::Generate()
{
    CountErrors();
    if(totalErrors == 0)
        return;
    if(!(CheckFileExists(templatePath)))
    {
        SetTemplatePath();
        return;
    }
    bStopGen = false;
    c.reset(new Card(rowCount));
    c->outputPath = cardOutputPath;
    c->templatePath = templatePath;

    c->info.client = entryClient->get_text();
    c->info.product = entryProduct->get_text();
    c->info.controlCardNr = entryCardControllNr->get_text();
    c->info.step = entryStep->get_text();
    c->info.date = entryDate->get_text();
    c->info.person = entryPerson->get_text();
    
    windowGenProgress->show();
    int problemIndex = 1;
    for(auto* i : listboxMain->get_children())
    {
        Gtk::Grid* grid = (Gtk::Grid*)(((Gtk::ListBoxRow*)i)->get_child());
        Gtk::SpinButton* sb = (Gtk::SpinButton*)(grid->get_child_at(0, 0));
        Gtk::Label* lb = (Gtk::Label*)(grid->get_child_at(3, 0));
        c->AddEntry(lb->get_text());
        double val = sb->get_value();
        c->problems[problemIndex++].nrOfProblems = (int)(val);
    }
    c->totalProblems = totalErrors;
    genThread = new std::thread(&Card::Generate, c.get(), this, &dGenProgress, &mPBar, &bStopGen);
};

void Gui::CancelGenerate()
{
    std::lock_guard<std::mutex> guard(mPBar);
    bStopGen = true;
};

void Gui::SetGenOutPath()
{
    auto dialog = new Gtk::FileChooserDialog("Wybierz folder wyjściowy", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog->add_button("_Wybierz", 10);
    dialog->add_button("_Zamknij", 100);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &Gui::SetGenOutPath_response), dialog));

    dialog->show();
};

void Gui::SetGenOutPath_response(int id, Gtk::FileChooserDialog* dialog)
{
    if(id == 10)
    {
        cardOutputPath = dialog->get_file()->get_path();
        Generate();
    }

    dialog->hide();
};

void Gui::UpdateGenProgress()
{
    std::lock_guard<std::mutex> guard(mPBar);
    if(bStopGen)
        dGenProgress = (double)totalErrors;
    levelbarProgress->set_value(dGenProgress*(100.0/totalErrors));
    if((int)(dGenProgress) >= totalErrors)
    {
        windowGenProgress->hide();
        genThread->join();
        delete genThread;
        genThread = nullptr;
    }
};

void Gui::SaveList()
{
    std::string output = "";
    output += entryClient->get_text() + "\n";
    output += entryProduct->get_text() + "\n";
    output += entryCardControllNr->get_text() + "\n";
    output += entryStep->get_text() + "\n";
    output += entryDate->get_text() + "\n";
    output += entryPerson->get_text() + "\n";

    for(auto* i : listboxMain->get_children())
    {
        Gtk::Grid* grid = (Gtk::Grid*)(((Gtk::ListBoxRow*)i)->get_child());
        Gtk::SpinButton* sb = (Gtk::SpinButton*)(grid->get_child_at(0, 0));
        Gtk::Label* lb = (Gtk::Label*)(grid->get_child_at(3, 0));
        double val = sb->get_value();
        output += std::to_string((int)val);
        output += " ";
        output += lb->get_text();
        output += "\n";
    }
    output.resize(output.length()-1);
    listFilePath += ".cgen";
    std::ofstream f;
    f.open(listFilePath);
    f << output;
    f.close();
};

void Gui::LoadList()
{
    std::ifstream f;
    f.open(listFilePath);
    std::string tmp;

    getline(f, tmp);
    entryClient->set_text(tmp);
    getline(f, tmp);
    entryProduct->set_text(tmp);
    getline(f, tmp);
    entryCardControllNr->set_text(tmp);
    getline(f, tmp);
    entryStep->set_text(tmp);
    getline(f, tmp);
    entryDate->set_text(tmp);
    getline(f, tmp);
    entryPerson->set_text(tmp);

    for(auto* i : listboxMain->get_children())
        listboxMain->remove(*i);
    
    while(getline(f, tmp))
    {
        size_t valEndPos = 0;
        for(auto i : tmp)
        {
            if(i == ' ')
                break;
            valEndPos++;
        }
        Gtk::SpinButton* count = Gtk::manage(new Gtk::SpinButton(Gtk::Adjustment::create(1.0, 1.0, 9999.0)));
        count->set_value(std::stod(tmp.substr(0, valEndPos)));
        count->signal_value_changed().connect(sigc::mem_fun(*this, &Gui::CountErrors));
        Gtk::Label* labelDesc = Gtk::manage(new Gtk::Label(tmp.substr(valEndPos+1, tmp.length()-1)));
        Gtk::Grid* grid = Gtk::manage(new Gtk::Grid());
        grid->set_border_width(2);
        grid->set_column_spacing(20);
        grid->attach(*count, 0, 0, 2);
        grid->attach(*labelDesc, 3, 0, 3);
        Gtk::ListBoxRow* r = Gtk::manage(new Gtk::ListBoxRow());
        r->add(*grid);
        listboxMain->append(*r);
    }
    windowMain->show_all();

    f.close();
    CountErrors();
};

void Gui::ListFileRW(int actionType)
{
    auto dialog = new Gtk::FileChooserDialog("Wybierz plik", actionType == CARD_LOAD_LIST ? Gtk::FILE_CHOOSER_ACTION_OPEN : Gtk::FILE_CHOOSER_ACTION_SAVE);
    auto file_filter = Gtk::FileFilter::create();

    file_filter->add_pattern("*.cgen");
    file_filter->set_name("Plik listy");
    dialog->add_filter(file_filter);
    dialog->add_button("_Wybierz", 1);
    dialog->add_button("_Anuluj", 2);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &Gui::SetListFilePath), dialog, actionType));

    dialog->show();
};

void Gui::SetListFilePath(int id, Gtk::FileChooserDialog* dialog, int actionType)
{
    listFilePath = dialog->get_file()->get_path();
    dialog->hide();
    if(id != 2)
    {
        if(actionType == CARD_LOAD_LIST)
            LoadList();
        else
            SaveList();
    }
};

bool Gui::CheckFileExists(std::string path)
{
    std::ifstream f(path);
    return f.good();
};

void Gui::SetTemplatePath()
{
    auto dialog = new Gtk::FileChooserDialog("Wybierz plik szablonu");
    auto file_filter = Gtk::FileFilter::create();

    file_filter->add_pattern("*.bmp");
    file_filter->set_name("Szablon");
    dialog->add_filter(file_filter);
    dialog->add_button("_Wybierz", 10);
    dialog->add_button("_Anuluj", 2);
    dialog->set_modal(true);
    dialog->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &Gui::SetTemplatePath_response), dialog));

    dialog->show();
};

void Gui::SetTemplatePath_response(int id, Gtk::FileChooserDialog* dialog)
{
    if(id == 10)
    {
        templatePath = dialog->get_file()->get_path();
        std::ofstream f("config");
        f << templatePath;
        f.close();
    }

    dialog->hide();
};