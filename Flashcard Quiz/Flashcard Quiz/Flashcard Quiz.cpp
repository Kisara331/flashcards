#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

// Flashcard class
class Flashcard {
private:
    string question;
    string answer;

public:
    Flashcard() {
        question = "";
        answer = "";
    }

    Flashcard(string q, string a) {
        question = q;
        answer = a;
    }

    void setQuestion(string q) {
        question = q;
    }

    void setAnswer(string a) {
        answer = a;
    }

    string getQuestion() const{
        return question;
    }

    string getAnswer() const{
        return answer;
    }
};

// Recursive function that gets the questions and answers from the user.
void input(vector<Flashcard>& flashcards, int count) {
    if (count <= 0) return;

    string q, a;
    cout << "\nEnter question: ";
    getline(cin, q);
    cout << "Enter answer: ";
    getline(cin, a);

    Flashcard fc;
    fc.setQuestion(q);
    fc.setAnswer(a);
    flashcards.push_back(fc);

    input(flashcards, count - 1);
}

// Stores the questions and answers entered by the user in the "questions.txt" and "answers.txt" files
void savefc(const vector<Flashcard>& flashcards, bool append = false) {
    ofstream qfile("questions.txt", append ? ios::app : ios::out);
    ofstream afile("answers.txt", append ? ios::app : ios::out);

    if (!qfile || !afile) {
        cerr << "Failed to open one or both files!\n";
        return;
    }

    for (int i = 0; i < flashcards.size(); i++) {
        qfile << flashcards[i].getQuestion() << "\n";
        afile << flashcards[i].getAnswer() << "\n";
    }

    qfile.close();
    afile.close();
}

// Accesses the questions and their matching answers from each file
void loadfc(vector<Flashcard>& flashcards) {
    ifstream qfile("questions.txt");
    ifstream afile("answers.txt");

    if (!qfile || !afile) return;

    string q, a;
    while (getline(qfile, q) && getline(afile, a)) {
        Flashcard fc;
        fc.setQuestion(q);
        fc.setAnswer(a);
        flashcards.push_back(fc);
    }

    qfile.close();
    afile.close();
}

// Shuffles the questions using srand and random_shuffle
void shuffleFlashcards(vector<Flashcard>& flashcards) {
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < flashcards.size(); ++i) {
        int j = rand() % flashcards.size();
        swap(flashcards[i], flashcards[j]);
    }
}

// Outputs the questions and after the user hits ENTER the answer is revealed
// Continues until user enters 'n' when asked to continue or all the flashcards are used
void quiz(vector<Flashcard>& flashcards) {
    if (flashcards.empty()) {
        cout << "No flashcards available.\n";
        return;
    }

    shuffleFlashcards(flashcards);

    for (int i = 0; i < flashcards.size(); ++i) {
        cout << "\nQuestion: " << flashcards[i].getQuestion() << endl;
        cout << "Press Enter to see the answer...";
        cin.get();

        cout << "Answer: " << flashcards[i].getAnswer() << endl;

        if (i < flashcards.size() - 1) {
            cout << "Continue? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore();
            if (tolower(choice) == 'n') break;
        }
    }

    cout << "\nYou have gone thorugh all of the FlashCards, GOOD JOB :)\n";
}

// Empties the memory of the questions and answer files
void clearFile() {
    ofstream qfile("questions.txt", ios::trunc);
    ofstream afile("answers.txt", ios::trunc);
    qfile.close();
    afile.close();
    cout << "All flashcards have been deleted.\n";
}

// Main Function
int main() {
    vector<Flashcard> flashcards;
    loadfc(flashcards);

    cout << "Welcome to the Flashcard App!\n";

    while (true) {
        cout << "\nMenu:\n"
            << "1. Add new flashcards\n"
            << "2. Start flashcard session\n"
            << "3. Clear all flashcards\n"
            << "4. Exit\n"
            << "\nChoose an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            int count;
            cout << "\nHow many flashcards would you like to add?: ";

            while (!(cin >> count)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please enter a number: ";
            }

            cin.ignore();
            cout << "\n";

            vector<Flashcard> newCards;
            input(newCards, count);
            savefc(newCards, true);
            flashcards.insert(flashcards.end(), newCards.begin(), newCards.end());
            cout << "------------------------------------------------------------------------------------------------------------------------";
            break;
        }
        case 2:
            quiz(flashcards);
            cout << "------------------------------------------------------------------------------------------------------------------------";
            break;
        case 3:
            clearFile();
            flashcards.clear();
            cout << "------------------------------------------------------------------------------------------------------------------------";
            break;
        case 4:
            cout << "Thank you for playing the FlashCard Quiz!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
