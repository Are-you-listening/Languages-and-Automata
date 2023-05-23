//
// Created by watson on 4/28/23.
//

#ifndef TOG_SONG_H
#define TOG_SONG_H

#include <string>
#include <stack>

#include "DesignByContract.h"
#include "Utils.h"
#include "midiparser/MidiParser.h"
#include "midiparser/midiExporter.h"
#include "RE.h"
#include "Note.h"
#include "NFA.h"
#include "WNFA.h"
#include "ENFA.h"
#include "string"

using namespace std;

/**
 * Object which contains all the data of a Song.
 */
class Song {
private:
    /**
     * \brief Toggle the Console Output
     */
    bool console;

    /**
     * \brief Title of the song
     */
    string title;

    /**
     * \brief To keep track of the logs (pushed_back into by various functions)
     */
    vector<string> logs;
    
    /**
     * \brief Value of rounding for each note in the export (1000=1s)
     */
    const double time_split = 500;

    /**
     * \brief Keep a pointer to itsself
     */
    Song* fInitCheck;

    /**
     * \brief Map sorted by the timestamp of the song and a bool deciding if these Notes are on/off. The timestamp is manually set to a accuracy of 1ms (0,001s)
     */
    map<pair<unsigned int, bool>, vector<Note*>> note_map; //{TimeStamp, on/off} {Note*} Timestamp=0,001s

    /**
     * \brief Convert Regex's to minimized DFA's
     * @param s
     * @param complement, bool deciding if the DFA complement needs to be taken
     * @param reverse, bool deciding if the DFA reverse needs to be taken
     * @return
     */
    vector<DFA*> convert(vector<RE> &s, bool complement, bool reverse);

    /**
     * \brief Check the similarity in order
     * \REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * \REQUIRE(d.size()<=s.size(), "Indices should be always valid");
     * \ENSURE(succeeded, "Operation did not work properly");
     * @return
     */
    static double checkTibo(vector<DFA*> &d, vector<RE> &s);
    
    /**
     * \brief Variant for Genre
     * @param d 
     * @param s 
     * @return 
     */
    static double checkTibo2(vector<vector<DFA*>> &d, vector<RE> &s);

    /**
     * \brief Cross check the similarity
     * \REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * \ENSURE(succeeded, "Operation did not work properly");
     * @return
     */
    static double checkKars(vector<DFA*> &d, vector<RE> &s);

    /**
     * \brief Cross check the similarity but more accurately?
     * \REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * \ENSURE(succeeded, "Operation did not work properly");
     * @param d
     * @param s
     * @return
     */
    static double checkKarsAnas(vector<DFA*> &d, vector<RE> &s);

    /**
     * \brief Use the WNFA to calculate a similarity %
     * @param t
     * @return
     */
    double checkWNFA(RE &r , RE &s);

    /**
     * \brief Magimathical Formula Anas
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     */
    double magimathical(vector<vector<double>> &results);

    /**
     * \brief Generate a random Regex without operator (replaces the "+" by choosing 1 option randomly, e.g. (a+b) becomes b)
     * @param s 
     * @return 
     */
    string generalize_re(const string& s);
    
    /**
     * \brief Generate a random Regex without operator (replaces the "+" by choosing 1 option randomly, e.g. (a+b) becomes b)
     * @param s 
     * @return 
     */
    pair<string, string> GenerateRandomRE(const string& s);

public:
    /**
    * \brief Checks if an item is properly initialised
     * @return succes, bool deciding the succes of the operation
    */
    [[nodiscard]] bool ProperlyInitialized() const;

    /**
     * \brief Constructor
     * @param noteMap
     */
    Song(const map<pair<unsigned int, bool>, vector<Note *>> &noteMap, bool console);

    /**
     * \brief Empty Constructor
     * \ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     */
    Song();

    /**
     * \brief Parse a .mid file and load it's contents into a newly created object.
     * @param path , the path to the .mid file
     * \REQUIRE(FileExists(path) , "Given file not found");
     * \ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     */
    Song(const string& path, bool console);

    /**
     * \brief Generate a Song from a DFA , sets the title to the DFA's StartState
     * \ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * @param s, a random DFA
     * @param param, the list with used parameters for Construction of Note Objects
     */
    Song(DFA* s, vector<int> &param, bool console);

    /**
     * \brief Overload Constructor (Creates a new object) Including new usages of memory!
     * \REQUIRE(a.ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * \ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * @param a
     * @return
     */
    Song& operator=(const Song &a);

    /**
     * \brief Destructor to free all used memory
     */
    ~Song();

    /**
     * \brief Convert a Song into a vector of Sub-Regex's
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param time_stamp , bool deciding if this Note-parameter should be used
     * @param note_on , bool deciding if this Note-parameter should be used
     * @param instrument , bool deciding if this Note-parameter should be used
     * @param note , bool deciding if this Note-parameter should be used
     * @param velocity , bool deciding if this Note-parameter should be used
     * @param pattern, int deciding the complexity of the constructed Regex
     * @return vector<RE> rex
     */
    [[nodiscard]] vector<RE> toRegex(int time_stamp, int duration, int instrument, int note_b, int velocity, int pattern);

    /**
     * \brief Count the amount of notes (per type) of a Song
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @return map<int,unsigned int> map[Note]=occurrences
     */
    [[nodiscard]] map<int,unsigned int> countNotes();

    /**
     * \brief Run all checks on 1 sort of Regex
     * \ENSURE(succes, "Percentage must be between 0 and 1");
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param pair<vector<RE>,vector<RE>> &toCheck
     * @param complement, in case the complements of the 2 songs needs to be checked
     * @param reverse, in case the complements of the 2 songs needs to be checked
     * @return
     */
    vector<double> similar(pair<vector<RE>,vector<RE>> &toCheck,bool complement, bool reverse);

    static vector<double> similar(vector<vector<DFA*>> &dfa1, vector<RE> &toCheck, int count, bool complement, bool reverse);

    /**
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(succes, "Percentage must be between 0 and 1");
     * \brief Run a similarity Check on this Song and a given Song ('song').
     * @param song
     * @param complement, in case the complements of the 2 songs needs to be checked
     * @param reverse, in case the complements of the 2 songs needs to be checked
     * @return percentage, the percentage the Songs are the same
     */
    double similarity(Song* song,bool complement, bool reverse);

    /**
     * \brief Check the occurrences of the Notes and its similarity towards the notes of this object
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(succeed, "Percentage must be between 0 and 1");
     * @return
     */
    [[nodiscard]] double noteCountSimilarity(Song &s);

    /**
     * \brief Check for 100% Equality of Songs
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param rhs
     * @return
     */
    bool operator==(Song* rhs);

    /**
     * \brief Check if songs are not the same
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param rhs
     * @return
     */
    bool operator!=(Song* rhs);

    /**
     * \brief Export the retrieved data by a Song's operations into a .txt file
     * \ENSURE(FileExists(title+"_report.txt"),"No log file has been created");
     * \REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     */
    void output() const;

    /**
     * \brief Get the title of an object
     * \REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @return
     */
    [[nodiscard]] const string &getTitle() const;

    /**
     * \brief Setter
     * \ENSURE(title == name; , "Setter didn't work properly");
     * \REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param name
     */
    void setTitle(const string &name);

    /**
     * \brief Export this Song back to a .mid File
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(FileExists(path),"No file has been created");
     * @param path
     */
    void save(const string &path);

    /**
     * \brief Turn the console output on/off
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     */
    void switchConsoleOutput();
};

#endif //TOG_SONG_H