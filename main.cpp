#include <iostream>
#include <fstream>
#include <String>
#include <cstdlib>
//#include <locale.h>





using namespace std;

int main()
{
   // setlocale(LC_CTYPE, ".1252");
    // system("chcp 1252");

/// INIT

    string Language;
    string Card_file_contents; //, Card_file_contents;
    string Translate_file_contents[20][400]; //20 Languages max
    string English_text, Translated_text="";
    char character;
    size_t sizeS;
    int col_lang=0; // language column
    int line_word=0; // line of words
    int lang_choosen=0; // number of the language to translate (column) 1 = english by default
    int max_lang=0, max_word;
    int i,j,match=0; // just for test
    bool success=false;
    const char *appdata = std::getenv ( "APPDATA" );
    string path1, path2;



/// PRESENTATION

    cout << "##############################################################################" << endl;
    cout << "#                       Welcome in the MagicTranslator.                      #" << endl;
    cout << "#                                                                            #" << endl;
    cout << "# Il s'agit d'un micro-logicielayant pour but de traduire le contenu des     # " << endl;
    cout << "# cartes Magic afin de pouvoir jouer plus serainement dans sa langue natale  #" << endl;
    cout << "# en ajoutant facilement sa langue et adapté pour chaque nouvelle editions   #" << endl;
    cout << "#                                                                            #" << endl;
    cout << "#                                                          Cree par Feanor63 #" << endl;
    cout << "##############################################################################" << endl << endl;


/// OPENING AND BUFFERING THE TRANSLATOR FILE IN "Translate_file_contents"

    ifstream Translatorfile("translator.csv", ios::in);  // opening the cards file
    if(Translatorfile)
    {

    cout << "translator.csv file have been found and opened" <<endl;

    do
    {
        do
        {
            do
            {
            Translatorfile.get(character); //get character
            if(character != ';') Translate_file_contents[col_lang][line_word]+=character;
            }while(  !(character == ';' || character == '\n') );
            col_lang++;

        }while ( !(character == '\n') );
        if (line_word == 0 ) max_lang = col_lang; //get the number of available languages
        col_lang=0;
        line_word++;

    }while(!(Translatorfile.eof()));
    max_word = line_word-1; // get the number of words to translate
    //for(i=0; i<200; i++) cout << Translate_file_contents[0][i]  << endl;//'\t' << Translate_file_contents[1][i] << endl;
    Translatorfile.close();
    }
    else cerr << "ERROR: translator.csv file not found!" << endl;



/// SELECTION OF THE LANGUAGE AMONG AVAILABLES

    cout << endl;
    cout << "In which language you sink translate cards?" << endl;
    cout << "please write one : ";
    for(col_lang=1 ; col_lang<max_lang ; col_lang++) cout << Translate_file_contents[col_lang][0] << "   ";

   while(lang_choosen==0)
    {
        cout << endl << endl << "Your Answer: ";
        cin >> Language;

        for(col_lang=1 ; col_lang<max_lang ; col_lang++)
        {
            if(Translate_file_contents[col_lang][0]==Language) lang_choosen=col_lang; //get the column of the language choosen
        }
        if (lang_choosen==0) cerr << "ERROR: no match. Please try again." << endl;
    }

//    Language="French";
 //   lang_choosen=1;



/// OPENING, BUFFERING

    if ( appdata != 0 )
    {
        path1 = appdata;
        path1 += "\\..\\Local\\Cockatrice\\cards.xml";
        ifstream Cardfile ( path1.c_str() );

    if ( appdata != 0 )
    {
      //  path2.erase();
        path2 = appdata;
        path2 += "\\..\\Local\\Cockatrice\\";
        path2 += Language;
        path2 += "Card.xml";
        ofstream Cardfile_translated ( path2.c_str(),  ios::out | ios::trunc );


        if(Cardfile_translated) // if file is correctly open
        {
        cout << Language << "Card.xml file have been created and opened" <<endl;

            if(Cardfile)
            {
            cout << "Card.xml file have been found and opened" <<endl;

    /// & TRANSLATION OF TEXT AND TYPE IN "Card_file_contents"

                while( getline(Cardfile, Card_file_contents) ) // get the content of the Card file
                {
                Card_file_contents+='\n';
       //cout << Card_file_contents.find("</card>") <<  "\t" << Card_file_contents << endl;
            // GET TEXT TO TRANSLATE
                    English_text.erase();
                    if(Card_file_contents.find("<type>") != string::npos) //cout << "\t" << English_text << "\t" << Card_file_contents << endl;
                    {
                //take text in <type> to </card> contents

                        English_text=Card_file_contents;
                        Card_file_contents.erase();

                        while(!(Card_file_contents.find("</card>") != string::npos))
                        {
                            getline(Cardfile, Card_file_contents);
                            Card_file_contents+='\n';
                            English_text+=Card_file_contents;

                        //cout << Card_file_contents << endl;
                        }
               // cout << Card_file_contents << endl;

                //searching match
                        for (line_word=1 ; line_word<max_word ; line_word++ ) // search for all words to translate
                        {
                            if( English_text.find(Translate_file_contents[0][line_word])!= string::npos) // if word found -> replace translated word
                            {
                                sizeS =  English_text.find(Translate_file_contents[0][line_word]);
                                English_text.replace(sizeS, Translate_file_contents[0][line_word].length(), Translate_file_contents[lang_choosen][line_word]);
                            }

                    //cout <<  English_text << endl;
                        }
                    Translated_text= English_text;
                    }
                    else
                    {
                       // Card_file_contents+='\n';
                        Translated_text=Card_file_contents;
                        Card_file_contents.erase();
              //  cout << Card_file_contents << endl;
                    }




                Cardfile_translated << Translated_text;
                 Translated_text.erase();
     //   success = true;
                }

            Cardfile.close();
            } else cerr << "ERROR: Card.xml file not found!" << endl;

        Cardfile_translated.close();
        }else cerr << "ERROR: " << Language << "Card.xml file could not created!" << endl;



        }

    }






/// CREATE A NEW FILE "<LANGUAGE><CARD>.XML" in same path %AppData%\Local\Cockatrice\

/*
  if ( appdata != 0 )
    {
        path.erase();
        path = appdata;
        path += "\\..\\Local\\Cockatrice\\";
        path += Language;
        path += "Card.xml";
        ofstream Cardfile_translated ( path.c_str(),  ios::out | ios::trunc );

        if(Cardfile_translated) // if file is correctly open
        {
        cout << Language << "Card.xml file have been created and opened" <<endl;
        Cardfile_translated << Translated_text;
        success = true;

        Cardfile_translated.close();
        } else cerr << "ERROR: " << Language << "Card.xml file could not created!" << endl;

    }

*/

/// TELL THE SUCCESS OR NOT

    cout << endl;
    if (success == false) cout << "The translation has failed, please check above error information" << endl;
    else if(success==true)
    {
        cout << "your card file have been translated in" << Language << endl;
        cout << "it's named" << Language << "card in %AppData%\\Local\\Cockatrice\\" << endl;
        cout << "now, you have to change the path in Cockatrice in Settings" << endl;
        cout << "NB: If there is no your language, you can edit the translation file ^_^" << endl;
    }

   // cout << "" << endl;
    return 0;
}
